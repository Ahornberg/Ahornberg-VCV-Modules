#include "../../Ahornberg.hpp"

struct IntervalNames : ParamQuantity {
	std::string getDisplayValueString() override;
};

struct Interval {
	float value;
	std::string shortName;
	std::string longName;
};

struct EqualDivision : ModuleWithScrews {
	const static Interval INTERVALS[];
	constexpr static int NUM_INTERVALS = 13;
	constexpr static int INIT_STEP = 33;
	constexpr static int INIT_INTERVAL = 9;

	enum ParamIds {
		FINE_PARAM = NUM_MAX_SCREWS,
		STEPS_PARAM,
		INTERVAL_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(PITCH_INPUT, 9),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(PITCH_OUTPUT, 9),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	EqualDivision();
	void process(const ProcessArgs& args) override;
};
