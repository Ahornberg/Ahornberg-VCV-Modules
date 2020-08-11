#include "../../Ahornberg.hpp"

struct BpmValues : ParamQuantity {
	std::string getDisplayValueString() override;
};

struct Metronome : ModuleWithScrews {
	const static int BPM_VALUES[];
	constexpr static int NUM_BPM_VALUES = 39;
	constexpr static int INIT_BPM = 10;

	enum ParamIds {
		BPM_VALUE_PARAM = NUM_MAX_SCREWS,
		ENUMS(BPM_VALUE_BUTTON_PARAM, NUM_BPM_VALUES),
		BPM_RESET_VALUE_PARAM,
		PLAY_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		THREE_STEP_FASTER_INPUT,
		TWO_STEP_FASTER_INPUT,
		ONE_STEP_FASTER_INPUT,
		ONE_STEP_SLOWER_INPUT,
		BPM_RESET_INPUT,
		PLAY_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		BPM_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	dsp::PulseGenerator pulse;
	dsp::Timer timer;
	dsp::SchmittTrigger threeStepFasterTrigger;
	dsp::SchmittTrigger twoStepFasterTrigger;
	dsp::SchmittTrigger oneStepFasterTrigger;
	dsp::SchmittTrigger oneStepSlowerTrigger;
	dsp::SchmittTrigger bpmResetTrigger;
	dsp::SchmittTrigger playTrigger;

	Metronome();
	void process(const ProcessArgs& args) override;
};
