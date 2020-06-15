struct BasicSwitch : SvgSwitch {
	BasicSwitch() {
		momentary = false;
		shadow->opacity = 0;
	}

	void addFrame(const std::string& filename) {
		SvgSwitch::addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
	}	
};

struct RoundSwitch : BasicSwitch {
	RoundSwitch() {
		addFrame("res/switches/Round_off.svg");
		addFrame("res/switches/Round_on.svg");
	}
};

struct RoundSwitchRed : BasicSwitch {
	RoundSwitchRed() {
		addFrame("res/switches/Round_off.svg");
		addFrame("res/switches/Round_red.svg");
	}
};

struct RoundLargeSwitch : BasicSwitch {
	RoundLargeSwitch() {
		addFrame("res/switches/RoundLarge_off.svg");
		addFrame("res/switches/RoundLarge_on.svg");
	}
};

struct PauseSwitch : BasicSwitch {
	PauseSwitch() {
		addFrame("res/switches/Pause_off.svg");
		addFrame("res/switches/Pause_on.svg");
	}
};
