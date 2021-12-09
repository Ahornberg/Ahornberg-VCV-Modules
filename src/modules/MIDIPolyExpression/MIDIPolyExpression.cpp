#include "MIDIPolyExpression.hpp"

MIDIPolyExpression::MIDIPolyExpression() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	
	configParam(MIDI_CHANNEL_FIRST_PARAM, MIN_MIDI_CHANNEL, MAX_MIDI_CHANNEL, MIN_MIDI_CHANNEL, "First MIDI Channel");
	configParam(MIDI_CHANNEL_COUNT_PARAM, MIN_MIDI_CHANNEL, MAX_MIDI_CHANNEL, MAX_MIDI_CHANNEL, "Number Of MIDI Channels");
	configParam(GATE_VELOCITY_MODE_PARAM,  0, 1, 0, "Gate Velocity Mode");
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
			volumeSlews[channelWithOffset].setRiseFall(10000, 400);
			volumeSlews[channelWithOffset].reset();
			modulationSlews[channelWithOffset].reset();
			modulationSlews[channelWithOffset].out = envelopes[channelWithOffset].modulation;
			envelopes[channelWithOffset].oldVolume = 0;
			envelopes[channelWithOffset].oldGate = envelopes[channelWithOffset].gate;
		} else if (envelopes[channelWithOffset].oldVolume > args.sampleRate / 44) {
			volumeSlews[channelWithOffset].setRiseFall(400, 400);
		}
		if (params[GATE_VELOCITY_MODE_PARAM].getValue()) {
			// if (envelopes[channelWithOffset].gate) {
				outputs[GATE_OUTPUT].setVoltage(envelopes[channelWithOffset].noteVolume, i);
			// } else {
				// outputs[GATE_OUTPUT].setVoltage(envelopes[channelWithOffset].noteVolume * -10, i);
			// }
		} else {
			outputs[GATE_OUTPUT].setVoltage(envelopes[channelWithOffset].gate * 10, i);
		}
		outputs[VOLUME_OUTPUT].setVoltage(volumeSlews[channelWithOffset].process(args.sampleTime, envelopes[channelWithOffset].volume), i);
		envelopes[channelWithOffset].oldVolume++;
		outputs[PITCH_OUTPUT].setVoltage(envelopes[channelWithOffset].notePitch + envelopes[channelWithOffset].pitch, i);
		outputs[MODULATION_OUTPUT].setVoltage(modulationSlews[channelWithOffset].process(args.sampleTime, envelopes[channelWithOffset].modulation), i);
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
		volumeSlews[i].setRiseFall(10000, 400);
		modulationSlews[i].setLambda(120);
		volumeSlews[i].reset();
		modulationSlews[i].reset();
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

