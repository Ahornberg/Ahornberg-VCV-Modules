#include "../../Ahornberg.hpp"

struct Tracker : BaseModule {
	enum ParamIds {
		NO_PARAM = NUM_MAX_SCREWS,
		NUM_PARAMS
	};
	enum InputIds {
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Tracker();
	void process(const ProcessArgs& args) override;
};
