#include "../../Ahornberg.hpp"

struct MIDIPolyEnvelopeData {
	float noteVolume;
	float notePitch;
	float volume;
	float oldVolume;
	float pitch;
	float modulation;
	float gate;
	float oldGate;
};

struct MIDIPolyEnvelope : BaseModule {
	constexpr static int MIN_MIDI_CHANNEL = 1;
	constexpr static int MAX_MIDI_CHANNEL = 16;
	enum ParamIds {
		MIDI_CHANNEL_FIRST_PARAM = NUM_MAX_SCREWS,
		MIDI_CHANNEL_COUNT_PARAM,
		VOLUME_ATTACK_THRESHOLD_PARAM,
		VOLUME_DECAY_PARAM,
		VOLUME_MIX_PARAM,
		VOLUME_SMOOTH_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		GATE_OUTPUT,
		VOLUME_OUTPUT,
		PITCH_OUTPUT,
		MODULATION_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	MIDIPolyEnvelopeData envelopes[MAX_MIDI_CHANNEL];
	midi::InputQueue midiInput;
	dsp::ExponentialFilter noteVolumeSlews[MAX_MIDI_CHANNEL];
	// dsp::ExponentialSlewLimiter noteVolumeSlews[MAX_MIDI_CHANNEL];
	dsp::ExponentialSlewLimiter volumeSlews[MAX_MIDI_CHANNEL];
	dsp::ExponentialFilter modulationSlews[MAX_MIDI_CHANNEL];

	MIDIPolyEnvelope();
	void process(const ProcessArgs& args) override;
	void processMidiMessage(const midi::Message& msg);
	void onReset() override;
	json_t* dataToJson() override;
	void dataFromJson(json_t* rootJ) override;
};
