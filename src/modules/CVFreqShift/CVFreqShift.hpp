#include "../../Ahornberg.hpp"

struct FrequencyRanges : ParamQuantity {
	std::string getDisplayValueString() override;
};

struct FrequencyRange {
	int rangeInHz;
	float rangeBasedOnC4;
};

struct CVFreqShift : ModuleWithScrews {
	const static FrequencyRange FREQUENCY_RANGES[];
	constexpr static int MIN_FREQUENCY_RANGE = 10;
	
	enum ParamIds {
		FREQUENCY_PARAM = NUM_MAX_SCREWS,
		FREQUENCY_MODULATION_AMOUNT_PARAM,
		FREQUENCY_RANGE_PARAM,
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
	void process(const ProcessArgs& args) override;
};
