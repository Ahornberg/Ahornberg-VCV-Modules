#include "../../Ahornberg.hpp"

struct CopyPastePreset : Module {
	enum ParamIds {
		DO_COPY_PASTE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		LINK_CV_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		LINK_CV_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	
	float oldParamValue;
	float oldInputValue;
	dsp::PulseGenerator pulse;
	
	CopyPastePreset();
	void process(const ProcessArgs& args) override;
};
