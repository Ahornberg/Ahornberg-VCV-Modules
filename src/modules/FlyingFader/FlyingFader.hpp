#include "../../Ahornberg.hpp"

struct FlyingFader : ModuleWithScrews {
	const static std::string INIT_FADER_NAME;
	
	enum ParamIds {
		FADER_PARAM = NUM_MAX_SCREWS,
		CV_INPUT_WAS_CONNECTED,
		FADER_VALUE_BEFORE_CONNECTED,
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_INPUT,
		CV_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AUDIO_OUTPUT,
		CV_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		// VU_LIGHT,
		NUM_LIGHTS
	};

	FlyingFader();
	void process(const ProcessArgs& args) override;
	
	bool faderDragged;
	dsp::ExponentialSlewLimiter audioSlewLimiter;
	
	// test
	// bool last_is_connected = false;
	// float last_input_voltage = 0.f;
};
