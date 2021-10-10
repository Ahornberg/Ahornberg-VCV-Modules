#include "../../Ahornberg.hpp"

struct MIDIOverAudioMessage {
	uint8_t messageData[MIDI_MESSAGE_DATA_MAX_LENGTH];
	int messageDataPos;
};

struct MIDIOverAudio : BaseModule {
	enum ParamIds {
		ENUMS_WITH_SCREWS(TRIM_PARAM, MIDI_OVER_AUDIO_MAX_DEVICES),
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
		ENUMS(RGB_LIGHT, MIDI_OVER_AUDIO_MAX_DEVICES * 3),
		NUM_LIGHTS
	};
	
	MIDIOverAudioMessage midiOverAudioMessages[MIDI_OVER_AUDIO_MAX_DEVICES];
	dsp::ExponentialSlewLimiter lightsSlewLimiter[MIDI_OVER_AUDIO_MAX_DEVICES * 3];
	
	MIDIOverAudio();
	void process(const ProcessArgs& args) override;
	void resetMessageData(int DeviceId);
	void panic();
};
