#include "../../Ahornberg.hpp"

struct MIDIOverAudio : ModuleWithScrews {
	enum ParamIds {
		FINE_PARAM = NUM_MAX_SCREWS,
		STEPS_PARAM,
		INTERVAL_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(PITCH_INPUT, 9),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(PITCH_OUTPUT, 9),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	
	int deviceId;
	uint8_t messageData[MIDI_MESSAGE_DATA_MAX_LENGTH];
	int messageDataPos;
	
	MIDIOverAudio();
	~MIDIOverAudio();
	void process(const ProcessArgs& args) override;
};
