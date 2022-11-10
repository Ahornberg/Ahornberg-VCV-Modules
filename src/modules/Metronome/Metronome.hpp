#include "../../Ahornberg.hpp"

struct Metronome : Module {
	const static int BPM_VALUES[];
	static std::vector<std::string> BPM_LABELS;
	constexpr static int NUM_BPM_VALUES = 39;
	constexpr static int INIT_BPM = 11;

	enum ParamIds {
		BPM_VALUE_PARAM,
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
