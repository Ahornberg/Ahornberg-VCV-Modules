struct RoundSwitch : SvgSwitch {
	RoundSwitch() {
		momentary = false;
		shadow->opacity = 0;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/Round_off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/Round_on.svg")));
	}
};

struct RoundSwitchRed : SvgSwitch {
	RoundSwitchRed() {
		momentary = false;
		shadow->opacity = 0;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/Round_off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/Round_red.svg")));
	}
};

struct RoundLargeSwitch : SvgSwitch {
	RoundLargeSwitch() {
		momentary = false;
		shadow->opacity = 0;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/RoundLarge_off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/RoundLarge_on.svg")));
	}
};

struct PlayBackwardsSwitch : SvgSwitch {
	PlayBackwardsSwitch() {
		momentary = false;
		shadow->opacity = 0.f;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/PlayBackwards_off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/PlayBackwards_on.svg")));
	}
};

struct PauseSwitch : SvgSwitch {
	PauseSwitch() {
		momentary = false;
		shadow->opacity = 0.f;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/Pause_off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/Pause_on.svg")));
	}
};

struct PlayForwardsSwitch : SvgSwitch {
	PlayForwardsSwitch() {
		momentary = false;
		shadow->opacity = 0.f;
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/PlayForwards_off.svg")));
		addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/switches/PlayForwards_on.svg")));
	}
};
