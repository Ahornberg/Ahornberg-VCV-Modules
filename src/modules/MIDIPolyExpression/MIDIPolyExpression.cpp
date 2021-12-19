#include "MIDIPolyExpression.hpp"

MIDIPolyExpression::MIDIPolyExpression() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	
	configParam(MIDI_CHANNEL_FIRST_PARAM, MIN_MIDI_CHANNEL, MAX_MIDI_CHANNEL, MIN_MIDI_CHANNEL, "First MIDI Channel");
	configParam(MIDI_CHANNEL_COUNT_PARAM, MIN_MIDI_CHANNEL, MAX_MIDI_CHANNEL, MAX_MIDI_CHANNEL, "Number Of MIDI Channels");
	configParam(GATE_VELOCITY_MODE_PARAM,  0, 1, 0, "Gate Velocity Mode");
	configParam(DECAY_PARAM,  0, 40, 8, "Decay");
	configParam(RELEASE_PARAM,  0, 4, 0, "Release");
	configParam(PITCH_SHAPE_PARAM,  0, 1, 0, "Pitch Shape");
	configOutput(GATE_OUTPUT, "Gate");
	configOutput(VOLUME_OUTPUT, "Volume");
	configOutput(PITCH_OUTPUT, "Pitch (1V/Octave)");
	configOutput(MODULATION_OUTPUT, "Modulation");
	onReset();
}

void MIDIPolyExpression::process(const ProcessArgs& args) {
	midi::Message msg;
	while (midiInput.tryPop(&msg, args.frame)) {
		processMidiMessage(msg);
	}
	int channelOffset = params[MIDI_CHANNEL_FIRST_PARAM].getValue() - 1;
	int channels = std::min((int) params[MIDI_CHANNEL_COUNT_PARAM].getValue(), MAX_MIDI_CHANNEL - channelOffset);
	outputs[GATE_OUTPUT].setChannels(channels);
	outputs[VOLUME_OUTPUT].setChannels(channels);
	outputs[PITCH_OUTPUT].setChannels(channels);
	outputs[MODULATION_OUTPUT].setChannels(channels);
	for (auto i = 0; i < channels; ++i) {
		auto channelWithOffset = i + channelOffset;
		if (envelopes[channelWithOffset].gate && !envelopes[channelWithOffset].oldGate) {
			// Note on / Attack
			pitchSlews[channelWithOffset].setRiseFall(INITIAL_SLEW_VALUE, SLEW_VALUE);
			pitchSlews[channelWithOffset].reset();
			modulationSlews[channelWithOffset].reset();
			modulationSlews[channelWithOffset].out = envelopes[channelWithOffset].modulation;
			volumeSlews[channelWithOffset].setRiseFall(std::max(0.f, (float) pow(envelopes[channelWithOffset].noteVolume - 1.0f, 4.2f) + 70.f), SLEW_VALUE);
			volumeSlews[channelWithOffset].reset();
			envelopes[channelWithOffset].noteLength = 0;
			envelopes[channelWithOffset].oldGate = envelopes[channelWithOffset].gate;
		} else if (!envelopes[channelWithOffset].gate && envelopes[channelWithOffset].oldGate) {
			float noteLength = envelopes[channelWithOffset].noteLength * 8.f / args.sampleRate;
			// Note off / Release
			if (noteLength > 1.5f) {
				// Release
				volumeSlews[channelWithOffset].setRiseFall(SLEW_VALUE, SLEW_VALUE / ((params[RELEASE_PARAM].getValue() + 1) * (2.f + envelopes[channelWithOffset].noteVolume / 10.f)));
			} else if (noteLength > 1.f) {
				// Decay to Release
				volumeSlews[channelWithOffset].setRiseFall(SLEW_VALUE, SLEW_VALUE / ((params[RELEASE_PARAM].getValue() + 1) * (noteLength - 1.f) + (params[DECAY_PARAM].getValue() + 1) * (1.5f - noteLength)) / 2.);	
			} else {
				// Decay for short notes
				volumeSlews[channelWithOffset].setRiseFall(SLEW_VALUE, SLEW_VALUE / (params[DECAY_PARAM].getValue() + 1));
			}				
			envelopes[channelWithOffset].oldGate = envelopes[channelWithOffset].gate;	
		} else if (envelopes[channelWithOffset].gate && envelopes[channelWithOffset].noteLength > args.sampleRate / 44) {
			// Note on / Decay & Sustain
			pitchSlews[channelWithOffset].setRiseFall(SLEW_VALUE, SLEW_VALUE);
			volumeSlews[channelWithOffset].setRiseFall(SLEW_VALUE, SLEW_VALUE);
		}
		if (!envelopes[channelWithOffset].gate) {
			// prevent hanging notes
			envelopes[channelWithOffset].volume = 0;
		}
		if (params[GATE_VELOCITY_MODE_PARAM].getValue()) {
			// W Gate Velocity Mode off
			outputs[GATE_OUTPUT].setVoltage(envelopes[channelWithOffset].noteVolume, i);
		} else {
			// W Gate Velocity Mode on
			outputs[GATE_OUTPUT].setVoltage(envelopes[channelWithOffset].gate * 10, i);
		}
		// X
		outputs[PITCH_OUTPUT].setVoltage(envelopes[channelWithOffset].notePitch + pitchSlews[channelWithOffset].process(args.sampleTime, envelopes[channelWithOffset].pitch), i);
		// Y
		outputs[MODULATION_OUTPUT].setVoltage(modulationSlews[channelWithOffset].process(args.sampleTime, envelopes[channelWithOffset].modulation), i);
		// Z
		outputs[VOLUME_OUTPUT].setVoltage(volumeSlews[channelWithOffset].process(args.sampleTime, envelopes[channelWithOffset].volume), i);
		envelopes[channelWithOffset].noteLength++;
	}
}

void MIDIPolyExpression::processMidiMessage(const midi::Message& msg) {
	int channel = msg.getChannel();
	if (msg.getStatus() == 0x9 && msg.getValue() > 0) {
		// note on
		envelopes[channel].noteVolume = msg.getValue() / 12.7f;
		envelopes[channel].notePitch = (msg.getNote() - 60) / 12.f;
		envelopes[channel].gate = 1;
		envelopes[channel].oldGate = 0;
	} else if (msg.getStatus() == 0x9 || msg.getStatus() == 0x8) {
		// note off
		envelopes[channel].noteVolume = msg.getValue() / -12.7f;
		envelopes[channel].gate = 0;
	} else if (msg.getStatus() == 0xa) {
		// poly aftertouch
		envelopes[channel].volume = msg.getValue() / 12.7f;
	} else if (msg.getStatus() == 0xd) {
		// channel aftertouch
		envelopes[channel].volume = msg.getNote() / 12.7f;
	} else if (msg.getStatus() == 0xe) {
		// pitch bend
		envelopes[channel].pitch = 4 * ((((uint16_t) msg.getValue() << 7) | msg.getNote()) - 8192) / 8191.f;
		if (params[PITCH_SHAPE_PARAM].getValue() > 0.f) {
			// Pitch Shape
			int semitones = floor(envelopes[channel].pitch * 12.f - 0.5f);
			float microtones = envelopes[channel].pitch * 12.f - 0.5f - semitones;
			float shapedMicrotones;
			if (microtones > 0.5) {
				shapedMicrotones= std::max((microtones - 0.5f) * (1.f - params[PITCH_SHAPE_PARAM].getValue()) + 0.5f, (microtones - params[PITCH_SHAPE_PARAM].getValue()) / (1.f - params[PITCH_SHAPE_PARAM].getValue()));
			} else {
				shapedMicrotones= std::min((microtones - 0.5f) * (1.f - params[PITCH_SHAPE_PARAM].getValue()) + 0.5f, microtones / (1.f - params[PITCH_SHAPE_PARAM].getValue()));
			}
			envelopes[channel].pitch = (semitones + shapedMicrotones + 0.5f) / 12.f;
			//DEBUG(" %i, %f", semitones, microtones - shapedMicrotones);
		}
	} else if (msg.getStatus() == 0xb && (msg.getNote() == 1 || msg.getNote() == 74)) {
		// modulation
		envelopes[channel].modulation = msg.getValue() / 12.7f;
	}
}

void MIDIPolyExpression::onReset() {
	for (auto i = 0; i < MAX_MIDI_CHANNEL; ++i) {
		envelopes[i].noteVolume = 0;
		envelopes[i].notePitch = 0;
		envelopes[i].volume = 0;
		envelopes[i].pitch = 0;
		envelopes[i].modulation = 0;
		envelopes[i].gate = 0;
		envelopes[i].oldGate = 0;
		pitchSlews[i].setRiseFall(INITIAL_SLEW_VALUE, SLEW_VALUE);
		pitchSlews[i].reset();
		modulationSlews[i].reset();
		modulationSlews[i].setLambda(SLEW_VALUE);
		volumeSlews[i].setRiseFall(INITIAL_SLEW_VALUE, SLEW_VALUE);
		volumeSlews[i].reset();
	}
	midiInput.reset();
}

json_t* MIDIPolyExpression::dataToJson() {
	json_t* rootJ = json_object();
	json_object_set_new(rootJ, "midi", midiInput.toJson());
	return rootJ;
}

void MIDIPolyExpression::dataFromJson(json_t* rootJ) {
	json_t* midiJ = json_object_get(rootJ, "midi");
	if (midiJ) {
		midiInput.fromJson(midiJ);
	}
}

