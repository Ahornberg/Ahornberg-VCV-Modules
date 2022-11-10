#include "../../Ahornberg.hpp"

struct FrequencyRange {
	int rangeInHz;
	float rangeBasedOnC4;
};

struct CVFreqShift : Module {
	const static FrequencyRange FREQUENCY_RANGES[];
	const static std::vector<std::string> FREQUENCY_RANGE_LABELS;
	constexpr static int MIN_FREQUENCY_RANGE = 10;
	
	enum ParamIds {
		FREQUENCY_PARAM,
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
