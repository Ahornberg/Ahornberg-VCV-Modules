#include "MIDIPolyEnvelope.hpp"

MIDIPolyEnvelope::MIDIPolyEnvelope() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	
	configParam(MIDI_CHANNEL_FIRST_PARAM, MIN_MIDI_CHANNEL, MAX_MIDI_CHANNEL, MIN_MIDI_CHANNEL, "First MIDI Channel");
	configParam(MIDI_CHANNEL_COUNT_PARAM, MIN_MIDI_CHANNEL, MAX_MIDI_CHANNEL, MAX_MIDI_CHANNEL, "Number Of MIDI Channels");
	configParam(VOLUME_ATTACK_THRESHOLD_PARAM, 0, 100, 80, "Volume Attack Treshold");
	configParam(VOLUME_DECAY_PARAM,   300, 1, 5, "Volume Decay", "", 0, -1, 301);
	configParam(VOLUME_MIX_PARAM, -50, 50, -10, "Velocity Pressure Mix");
	configParam(VOLUME_SMOOTH_PARAM,  100, 1, 70, "Volume Smooth", "", 0, -1, 301);
	configOutput(GATE_OUTPUT, "Gate");
	configOutput(VOLUME_OUTPUT, "Volume");
	configOutput(PITCH_OUTPUT, "Pitch (1V/Octave)");
	configOutput(MODULATION_OUTPUT, "Modulation");
	onReset();
}

void MIDIPolyEnvelope::process(const ProcessArgs& args) {
	midi::Message msg;
	while (midiInput.tryPop(&msg, args.frame)) {
		processMidiMessage(msg);
	}
	int channelOffset = params[MIDI_CHANNEL_FIRST_PARAM].getValue() - 1;
	//int channels = std::max(1, (int) (1 + params[MIDI_CHANNEL_COUNT_PARAM].getValue() - params[MIDI_CHANNEL_FIRST_PARAM].getValue()));
	int channels = std::min((int) params[MIDI_CHANNEL_COUNT_PARAM].getValue(), MAX_MIDI_CHANNEL - channelOffset);
	outputs[GATE_OUTPUT].setChannels(channels);
	outputs[VOLUME_OUTPUT].setChannels(channels);
	outputs[PITCH_OUTPUT].setChannels(channels);
	outputs[MODULATION_OUTPUT].setChannels(channels);
	for (auto i = 0; i < channels; ++i) {
		auto channelWithOffset = i + channelOffset;
		// noteVolumeSlews[channelWithOffset].setRiseFall(10000, params[VOLUME_DECAY_PARAM].getValue());
		noteVolumeSlews[channelWithOffset].setLambda(params[VOLUME_DECAY_PARAM].getValue());
		// volumeSlews[channelWithOffset].setLambda(params[VOLUME_SMOOTH_PARAM].getValue());
		if (envelopes[channelWithOffset].gate && !envelopes[channelWithOffset].oldGate) {
			volumeSlews[channelWithOffset].setRiseFall(params[VOLUME_SMOOTH_PARAM].getValue() * 3.f, params[VOLUME_SMOOTH_PARAM].getValue());
			noteVolumeSlews[channelWithOffset].reset();
			volumeSlews[channelWithOffset].reset();
			noteVolumeSlews[channelWithOffset].out = std::max(0.f, envelopes[channelWithOffset].noteVolume);
			modulationSlews[channelWithOffset].reset();
			modulationSlews[channelWithOffset].out = envelopes[channelWithOffset].modulation;
			// noteVolumeSlews[channelWithOffset].out = 0;
			// noteVolumeSlews[channelWithOffset].process(args.sampleTime, std::max(0.f, envelopes[channelWithOffset].noteVolume));
			envelopes[channelWithOffset].oldVolume = 0;
			envelopes[channelWithOffset].oldGate = envelopes[channelWithOffset].gate;
		} else if (envelopes[channelWithOffset].oldVolume > args.sampleRate / 44) {
			volumeSlews[channelWithOffset].setRiseFall(params[VOLUME_SMOOTH_PARAM].getValue(), params[VOLUME_SMOOTH_PARAM].getValue());

		}
		outputs[GATE_OUTPUT].setVoltage(envelopes[channelWithOffset].gate * 10, i);
		// outputs[VOLUME_OUTPUT].setVoltage(std::max(
			// noteVolumeSlews[channelWithOffset].process(args.sampleTime, 0),
			// volumeSlews[channelWithOffset].process(args.sampleTime, envelopes[channelWithOffset].volume * envelopes[channelWithOffset].gate)), i);
		outputs[VOLUME_OUTPUT].setVoltage(
			noteVolumeSlews[channelWithOffset].process(args.sampleTime, 0) + volumeSlews[channelWithOffset].process(args.sampleTime, envelopes[channelWithOffset].volume), i);
		envelopes[channelWithOffset].oldVolume++;
		outputs[PITCH_OUTPUT].setVoltage(envelopes[channelWithOffset].notePitch + envelopes[channelWithOffset].pitch, i);
		outputs[MODULATION_OUTPUT].setVoltage(modulationSlews[channelWithOffset].process(args.sampleTime, envelopes[channelWithOffset].modulation), i);
	}
}

void MIDIPolyEnvelope::processMidiMessage(const midi::Message& msg) {
	int channel = msg.getChannel();
	if (msg.getStatus() == 0x9 && msg.getValue() > 0) {
		// note on
		envelopes[channel].noteVolume = std::max(0.f, msg.getValue() - (params[VOLUME_MIX_PARAM].getValue() - 50) * 1.27f) * (1 - abs(params[VOLUME_MIX_PARAM].getValue()) * 0.005f) / (12.7f / 0.75f);
		envelopes[channel].notePitch = (msg.getNote() - 60) / 12.f;
		envelopes[channel].gate = 1;
		envelopes[channel].oldGate = 0;
	} else if (msg.getStatus() == 0x9 || msg.getStatus() == 0x8) {
		// note off
		envelopes[channel].noteVolume = msg.getValue() / 12.7f;
		envelopes[channel].gate = 0;
	} else if (msg.getStatus() == 0xa) {
		// poly aftertouch
		envelopes[channel].volume = msg.getValue() * (params[VOLUME_MIX_PARAM].getValue() + 50) * (1 - abs(params[VOLUME_MIX_PARAM].getValue()) * 0.005f) / (1270.f / 0.75f);
	} else if (msg.getStatus() == 0xd) {
		// channel aftertouch
		envelopes[channel].volume = msg.getNote() * (params[VOLUME_MIX_PARAM].getValue() + 50) * (1 - abs(params[VOLUME_MIX_PARAM].getValue()) * 0.005f) / (1270.f / 0.75f);
	} else if (msg.getStatus() == 0xe) {
		// pitch bend
		envelopes[channel].pitch = 4 * ((((uint16_t) msg.getValue() << 7) | msg.getNote()) - 8192) / 8191.f;
	} else if (msg.getStatus() == 0xb && (msg.getNote() == 1 || msg.getNote() == 74)) {
		// modulation
		envelopes[channel].modulation = msg.getValue() / 12.7f;
	}
}

void MIDIPolyEnvelope::onReset() {
	for (auto i = 0; i < MAX_MIDI_CHANNEL; ++i) {
		envelopes[i].noteVolume = 0;
		envelopes[i].notePitch = 0;
		envelopes[i].volume = 0;
		envelopes[i].pitch = 0;
		envelopes[i].modulation = 0;
		envelopes[i].gate = 0;
		envelopes[i].oldGate = 0;
		// noteVolumeSlews[i].setRiseFall(10000, params[VOLUME_DECAY_PARAM].getValue());
		noteVolumeSlews[i].setLambda(params[VOLUME_DECAY_PARAM].getValue());
		// volumeSlews[i].setLambda(params[VOLUME_SMOOTH_PARAM].getValue());
		volumeSlews[i].setRiseFall(params[VOLUME_SMOOTH_PARAM].getValue(), params[VOLUME_SMOOTH_PARAM].getValue());
		modulationSlews[i].setLambda(120);
		noteVolumeSlews[i].reset();
		volumeSlews[i].reset();
		modulationSlews[i].reset();
	}
	midiInput.reset();
}

json_t* MIDIPolyEnvelope::dataToJson() {
	json_t* rootJ = json_object();
	json_object_set_new(rootJ, "midi", midiInput.toJson());
	return rootJ;
}

void MIDIPolyEnvelope::dataFromJson(json_t* rootJ) {
	json_t* midiJ = json_object_get(rootJ, "midi");
	if (midiJ) {
		midiInput.fromJson(midiJ);
	}
}

