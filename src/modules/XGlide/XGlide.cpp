#include "XGlide.hpp"

const std::vector<std::string> XGlide::NOTE_NAMES = {
	"C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
	"C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
	"C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
	"C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
	"C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
	"C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
	"C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
	"C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7",
	"C8", "C#8", "D8", "D#8", "E8", "F8", "F#8", "G8", "G#8", "A8", "A#8", "B8",
	"C9", "C#9", "D9", "D#9", "E9", "F9", "F#9", "G9", "G#9", "A9", "A#9", "B9",
	"C10", "C#10", "D10", "D#10", "E10", "F10", "F#10", "G10",
};

XGlide::XGlide() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	
	configParam(MIN_INTERVAL_PARAM, 1, 127, 1, "Minimum Interval", " semitones");
	configParam(MAX_INTERVAL_PARAM, 1, 127, 127, "Maximum Interval", " semitones");
	configSwitch(MIN_NOTE_PARAM, 0, 127, 0, "Lowest MIDI note", NOTE_NAMES);
	configSwitch(MAX_NOTE_PARAM, 0, 127, 127, "Highest MIDI note", NOTE_NAMES);
	configInput(VOLUME_INPUT, "Volume");
	configInput(PITCH_INPUT, "Pitch (1V/Octave)");
	configInput(PEDAL_INPUT, "Pedal/Gate");
	configInput(MODULATION_INPUT, "Modulation");
	configOutput(VOLUME_OUTPUT, "Volume");
	configOutput(PITCH_OUTPUT, "Pitch (1V/Octave)");
	configOutput(MODULATION_OUTPUT, "Modulation");
	isResetChannelMapping = false;
	resetChannelMapping();
}

void XGlide::process(const ProcessArgs& args) {
	if (inputs[VOLUME_INPUT].isConnected() &&
			((inputs[PITCH_INPUT].isConnected() && outputs[PITCH_OUTPUT].isConnected()) ||
			(inputs[MODULATION_INPUT].isConnected() && outputs[MODULATION_OUTPUT].isConnected()))) {
		int usedChannels = inputs[VOLUME_INPUT].getChannels();
		outputs[VOLUME_OUTPUT].setChannels(usedChannels);
		outputs[PITCH_OUTPUT].setChannels(usedChannels);
		outputs[MODULATION_OUTPUT].setChannels(usedChannels);
		for (auto i = 0; i < usedChannels; ++i) {
			channelMapping[i].volume = clamp(inputs[VOLUME_INPUT].getVoltage(i), 0.f, 100.f);
			channelMapping[i].pitch = clamp(inputs[PITCH_INPUT].getVoltage(i), -100.f, 100.f);
			channelMapping[i].modulation = clamp(inputs[MODULATION_INPUT].getVoltage(i), -100.f, 100.f);
		}
		for (auto i = 0; i < usedChannels; ++i) {
			if (channelMapping[i].volume > 0) {
				if (channelMapping[i].glideTarget == NO_GLIDE && channelMapping[i].oldVolume == 0) {
					if (inputs[PEDAL_INPUT].isConnected()) {
						if (inputs[PEDAL_INPUT].getVoltage() > 0) {
							remapToGlideSourceChannelOut(i, usedChannels);
						}
					} else {
						remapToGlideSourceChannelOut(i, usedChannels);
					}
				}
			} else {
				if (channelMapping[i].glideTarget != NO_GLIDE && channelMapping[i].oldVolume > 0) {
					remapToFirstUnusedChannelOut(i, usedChannels);
				}
			}
			channelMapping[i].oldVolume = channelMapping[i].volume;
		}
		for (auto i = 0; i < usedChannels; ++i) {
			outputs[VOLUME_OUTPUT].setVoltage(0, i);
		}
		for (auto i = 0; i < usedChannels; ++i) {
			int channel = channelMapping[i].channelOut;
			int glideTarget = channelMapping[i].glideTarget;
			float volume = channelMapping[i].volume;
			outputs[VOLUME_OUTPUT].setVoltage(outputs[VOLUME_OUTPUT].getVoltage(channel) + volume, channel);
			if (glideTarget == GLIDE_TARGET) {
				continue;
			}
			if (glideTarget == NO_GLIDE) {
				outputs[PITCH_OUTPUT].setVoltage(channelMapping[i].pitch, channel);
				outputs[MODULATION_OUTPUT].setVoltage(channelMapping[i].modulation, channel);
			} else {
				float glideAmount = channelMapping[glideTarget].volume / (channelMapping[i].volume + channelMapping[glideTarget].volume);
				outputs[PITCH_OUTPUT].setVoltage(channelMapping[i].pitch + (channelMapping[glideTarget].pitch - channelMapping[i].pitch) * glideAmount, channel);
				outputs[MODULATION_OUTPUT].setVoltage(channelMapping[i].modulation + (channelMapping[glideTarget].modulation - channelMapping[i].modulation) * glideAmount, channel);
			}
		}		
	} else {
		// when no inputs are connected, reset the mapping
		resetChannelMapping();
		outputs[VOLUME_OUTPUT].setChannels(0);
		outputs[PITCH_OUTPUT].setChannels(0);
	}
	
	if (params[MIN_INTERVAL_PARAM].getValue() < params[MAX_INTERVAL_PARAM].getValue()) {
		lights[EXCLUDE_INTERVALS_LIGHT].setBrightness(0);
	} else {
		lights[EXCLUDE_INTERVALS_LIGHT].setBrightness(1);
	}
	if (params[MIN_NOTE_PARAM].getValue() < params[MAX_NOTE_PARAM].getValue()) {
		lights[EXCLUDE_NOTES_LIGHT].setBrightness(0);
	} else {
		lights[EXCLUDE_NOTES_LIGHT].setBrightness(1);
	}
}

bool XGlide::checkRange(int channelA, int channelB) {
	// if minNote >= maxNote, note range is outside
	float minNote = params[MIN_NOTE_PARAM].getValue();
	float maxNote = params[MAX_NOTE_PARAM].getValue();
	if (minNote < maxNote) {
		minNote -= 60.5f;
		maxNote -= 59.5f;
		minNote /= 12.f;
		maxNote /= 12.f;
		if ((channelMapping[channelA].pitch < minNote || channelMapping[channelA].pitch > maxNote) || (channelMapping[channelB].pitch < minNote || channelMapping[channelB].pitch > maxNote)) {
			return false;
		}
	} else {
		minNote -= 59.5f;
		maxNote -= 60.5f;
		minNote /= 12.f;
		maxNote /= 12.f;
		if ((channelMapping[channelA].pitch < minNote && channelMapping[channelA].pitch > maxNote) || (channelMapping[channelB].pitch < minNote && channelMapping[channelB].pitch > maxNote)) {
			return false;
		}		
	}
	// if negative, interval range is outside
	float minInterval = params[MIN_INTERVAL_PARAM].getValue();
	float maxInterval = params[MAX_INTERVAL_PARAM].getValue();
	if (minInterval < maxInterval) {
		minInterval -= 0.5f;
		maxInterval += 0.5f;
		minInterval /= 12.f;
		maxInterval /= 12.f;
		if (abs(channelMapping[channelA].pitch - channelMapping[channelB].pitch) < minInterval || abs(channelMapping[channelA].pitch - channelMapping[channelB].pitch) > maxInterval) {
			return false;
		}
	} else {
		minInterval += 0.5f;
		maxInterval -= 0.5f;
		minInterval /= 12.f;
		maxInterval /= 12.f;
		if (abs(channelMapping[channelA].pitch - channelMapping[channelB].pitch) < minInterval && abs(channelMapping[channelA].pitch - channelMapping[channelB].pitch) > maxInterval) {
			return false;
		}
	}
	return true;
}

void XGlide::remapToGlideSourceChannelOut(int channel, int maxUsedChannels) {
	for (auto i = 0; i < maxUsedChannels; ++i) {
		if (i != channel && channelMapping[i].volume > 0 && channelMapping[i].glideTarget == NO_GLIDE && checkRange(i, channel)) {
			channelMapping[channel].channelOut = channelMapping[i].channelOut;
			channelMapping[channel].glideTarget = i;
			channelMapping[i].glideTarget = GLIDE_TARGET;
			isResetChannelMapping = false;
			break;
		}
	}
}

void XGlide::remapToFirstUnusedChannelOut(int channel, int maxUsedChannels) {
	for (auto i = 0; i < maxUsedChannels; ++i) {
		if (i != channel && channelMapping[i].channelOut == channelMapping[channel].channelOut && channelMapping[i].glideTarget != NO_GLIDE) {
			channelMapping[i].glideTarget = NO_GLIDE;
			channelMapping[channel].glideTarget = NO_GLIDE;
			bool usedChannels[maxUsedChannels];
			for (auto j = 0; j < maxUsedChannels; ++j) {
				usedChannels[j] = false;
			}
			for (auto j = 0; j < maxUsedChannels; ++j) {
				usedChannels[channelMapping[j].channelOut] = true;
			}
			for (auto j = 0; j < maxUsedChannels; ++j) {
				if(!usedChannels[j]) {
					channelMapping[channel].channelOut = j;
					isResetChannelMapping = false;
					break;
				}	
			}
			break;
		}
	}
}

void XGlide::resetChannelMapping() {
	if (!isResetChannelMapping) {
		for (auto i = 0; i < MAX_CHANNELS; ++i) {
			channelMapping[i].volume = 0;
			channelMapping[i].pitch = 0;
			channelMapping[i].modulation = 0;
			channelMapping[i].channelOut = i;
			channelMapping[i].glideTarget = NO_GLIDE;
			channelMapping[i].oldVolume = 0;
		}
		isResetChannelMapping = true;
	}
}


