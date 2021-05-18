#include "../../Ahornberg.hpp"

struct TapeRecorderMixer : ModuleWithScrews {
	constexpr static float AUDIO_MUTE_SLEW = 350;
	constexpr static int TRACK_NUMBER_BITMASK = 15;
	constexpr static int LINK_BITMASK = 16;
	constexpr static int RECORD_BITMASK = 32;
	constexpr static int BYPASS_CHAIN_BITMASK = 64;
	constexpr static int SOLO_BITMASK = 128;
	constexpr static int MUTE_BITMASK = 256;
	constexpr static int INPUT_MUTE_BITMASK = 512;
	constexpr static int CHANNEL_16_CONVERT_FACTOR = 8388608;
	
	enum ParamIds {
		RECORD_PARAM = NUM_MAX_SCREWS,
		BYPASS_CHAIN_PARAM,
		TAPE_DUCKING_PARAM,
		TAPE_ERASE_PARAM,
		SOLO_PARAM,
		MUTE_PARAM,
		INPUT_VOLUME_PARAM,
		INPUT_MUTE_PARAM,
		INPUT_MUTE_ENABLED_PARAM,
		LINK_PARAM,
		// delay compensation
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_INPUT,
		AUDIO_FX_RETURN,
		AUDIO_CHAIN_TO_TAPE_INPUT,
		AUDIO_CHAIN_FROM_TAPE_INPUT,
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
		AUDIO_CHAIN_FROM_TAPE_OUTPUT,
		AUDIO_CHAIN_TO_TAPE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	
	int channelNumber;
	int maxChannels;
	float vuMeter;
	bool linked;

	dsp::ExponentialSlewLimiter muteSlewLimiter;
	dsp::ExponentialSlewLimiter inputMuteSlewLimiter;
	dsp::ExponentialSlewLimiter fxReturnSlewLimiter;
	dsp::ExponentialSlewLimiter fxBypassSlewLimiter;
	dsp::ExponentialSlewLimiter cvInputVolumeSlewLimiter;
	dsp::SlewLimiter vuMeterSlewLimiter;
	dsp::SchmittTrigger recordInputTrigger;
	dsp::SchmittTrigger fxBypassInputTrigger;
	dsp::SchmittTrigger soloInputTrigger;
	dsp::SchmittTrigger muteInputTrigger;
	
	// dsp::ClockDivider testDivider;
	
	TapeRecorderMixer();
	void process(const ProcessArgs& args) override;
};
