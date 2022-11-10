#include "../../Ahornberg.hpp"

struct MIDIControllerOutput : midi::Output {
	constexpr static int64_t SEND_MIDI_MESSAGE_NOW = -1;

	int64_t frame;

	MIDIControllerOutput();
	void reset();
	void setValue(uint8_t cc, uint8_t value);
	void setFrame(int64_t frame);
};

struct MIDIController : Module {
	constexpr static int MAX_MIDI_CC = 128;
	constexpr static int CC_NO_VALUE = -1;
	constexpr static int SLEW_VALUE = 20000;
	enum ParamIds {
		MOVEMENT_ADJUST_PARAM,
		TEST_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		TEST_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		TEST_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	int ccData[MAX_MIDI_CC];
	int ccDataOld[MAX_MIDI_CC];
	float cvData[MAX_MIDI_CC];
	int64_t lastFrame[MAX_MIDI_CC];
	bool controllerCorrected[MAX_MIDI_CC];
	midi::InputQueue midiInput;
	MIDIControllerOutput midiOutput;
	dsp::SlewLimiter cvSlews[MAX_MIDI_CC];

	MIDIController();
	void process(const ProcessArgs& args) override;
	void processMidiMessage(const midi::Message& msg);
	void onReset() override;
	json_t* dataToJson() override;
	void dataFromJson(json_t* rootJ) override;
};
