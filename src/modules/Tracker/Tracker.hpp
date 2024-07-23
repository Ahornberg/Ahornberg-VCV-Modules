#include "../../Ahornberg.hpp"

struct Tracker : Module {
	constexpr static int NUM_MAX_PATTERNS = 256;
	constexpr static int NUM_MAX_LINES = 16;
	
	enum ParamIds {
		CURRENT_PATTERN_PARAM,
		CURRENT_LINE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		NOTE_VALUE_OUTPUT,
		GATE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	
	enum PatternType {
		TYPE_NOTE,
		TYPE_VALUE
	};

	Tracker();
	void process(const ProcessArgs& args) override;
};
