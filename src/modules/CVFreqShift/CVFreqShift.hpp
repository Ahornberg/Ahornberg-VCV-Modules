#include "../../Ahornberg.hpp"

struct CVFreqShift : ModuleWithScrews {
	enum ParamIds {
		FREQUENCY_PARAM = NUM_MAX_SCREWS,
		FREQUENCY_MODULATION_AMOUNT_PARAM,
		ROOT_OCTAVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(PITCH_INPUT, 9),
		FREQUENCY_MODULATION_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(PITCH_OUTPUT, 9),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	CVFreqShift();
	void process(const ProcessArgs &args) override;
};
