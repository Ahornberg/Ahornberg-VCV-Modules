#include "../../Ahornberg.hpp"

struct TapeRecorderMixer : ModuleWithScrews {
	enum ParamIds {
		TAPE_VOLUME_PARAM = NUM_MAX_SCREWS,
		// INPUT_VOLUME_PARAM,
		// MUTE_PARAM,
		// RECORD_PARAM,
		// LINK_PARAM,
		// 
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_INPUT,
		AUDIO_FX_RETURN,
		AUDIO_CHAIN_LEFT_INPUT,
		AUDIO_CHAIN_RIGHT_INPUT,
		CV_TAPE_VOLUME_INPUT,
		CV_VOLUME_SLIDER_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AUDIO_OUTPUT,
		AUDIO_FX_SEND,
		AUDIO_CHAIN_LEFT_OUTPUT,
		AUDIO_CHAIN_RIGHT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	
	TapeRecorderMixer();
	void process(const ProcessArgs& args) override;
};
