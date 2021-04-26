#include "../../Ahornberg.hpp"

struct TapeRecorderMixer : ModuleWithScrews {
	constexpr static float PLUS_6_DB = 1.41253757f;
	
	enum ParamIds {
		RECORD_PARAM = NUM_MAX_SCREWS,
		BYPASS_CHAIN_PARAM,
		TAPE_DUCKING_PARAM,
		TAPE_ERASE_PARAM,
		SOLO_PARAM,
		MUTE_PARAM,
		INPUT_VOLUME_PARAM,
		INPUT_MUTE_PARAM,
		LINK_PARAM,
		// delay compensation
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_INPUT,
		AUDIO_FX_RETURN,
		AUDIO_CHAIN_LEFT_INPUT,
		AUDIO_CHAIN_RIGHT_INPUT,
		CV_RECORD_INPUT,
		CV_BYPASS_CHAIN_INPUT,
		CV_TAPE_DUCKING_INPUT,
		CV_TAPE_ERASE_INPUT,
		CV_SOLO_INPUT,
		CV_MUTE_INPUT,
		CV_INPUT_VOLUME_INPUT,
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
