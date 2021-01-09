#include "../../Ahornberg.hpp"

struct MIDIOverAudioMessage {
	uint8_t messageData[MIDI_MESSAGE_DATA_MAX_LENGTH];
	int messageDataPos;
};

struct MIDIOverAudio : ModuleWithScrews {
	enum ParamIds {
		FINE_PARAM = NUM_MAX_SCREWS,
		STEPS_PARAM,
		INTERVAL_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(MIDI_OVER_AUDIO_INPUT, MIDI_OVER_AUDIO_MAX_DEVICES),
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	
	MIDIOverAudioMessage midiOverAudioMessages[MIDI_OVER_AUDIO_MAX_DEVICES];
	
	MIDIOverAudio();
	void process(const ProcessArgs& args) override;
	void resetMessageData(int DeviceId);
};
