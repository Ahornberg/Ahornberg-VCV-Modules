#include "../../Ahornberg.hpp"

struct Manual : BaseModule {
	enum ParamIds {
		NUM_PARAMS = NUM_MAX_SCREWS
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

	Manual();
};
