#include "../../Ahornberg.hpp"

struct FunWithFrames : ModuleWithScrews {
	enum ParamIds {
		CHORD_SIZE_PARAM = NUM_MAX_SCREWS,
		ENUMS(VALUE_CONVERT_MODE, 4),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(VALUE_INPUT, 4),
		NOTE_INPUT,
		GATE_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(VALUE_OUTPUT, 4),
		NOTE_OUTPUT,
		GATE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	FunWithFrames();
	void process(const ProcessArgs& args) override;
};
