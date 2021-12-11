#include "../../Ahornberg.hpp"

struct MIDIPolyExpressionData {
	float noteVolume;
	float notePitch;
	float volume;
	float noteLength;
	float pitch;
	float modulation;
	float gate;
	float oldGate;
};

struct MIDIPolyExpression : BaseModule {
	constexpr static int MIN_MIDI_CHANNEL = 1;
	constexpr static int MAX_MIDI_CHANNEL = 16;
	constexpr static int SLEW_VALUE = 70;
	constexpr static int INITIAL_SLEW_VALUE = 10000;
	enum ParamIds {
		MIDI_CHANNEL_FIRST_PARAM = NUM_MAX_SCREWS,
		MIDI_CHANNEL_COUNT_PARAM,
		GATE_VELOCITY_MODE_PARAM,
		DECAY_PARAM,
		RELEASE_PARAM,
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

	MIDIPolyExpressionData envelopes[MAX_MIDI_CHANNEL];
	midi::InputQueue midiInput;
	dsp::ExponentialSlewLimiter pitchSlews[MAX_MIDI_CHANNEL];
	dsp::ExponentialFilter modulationSlews[MAX_MIDI_CHANNEL];
	dsp::ExponentialSlewLimiter volumeSlews[MAX_MIDI_CHANNEL];

	MIDIPolyExpression();
	void process(const ProcessArgs& args) override;
	void processMidiMessage(const midi::Message& msg);
	void onReset() override;
	json_t* dataToJson() override;
	void dataFromJson(json_t* rootJ) override;
};
