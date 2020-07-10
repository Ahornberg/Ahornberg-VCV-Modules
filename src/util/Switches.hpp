struct BasicSwitch : SvgSwitch {
	BasicSwitch();
	void addFrame(const std::string& filename);
};

struct RoundSwitch : BasicSwitch {
	RoundSwitch();
};

struct RoundSwitchRed : BasicSwitch {
	RoundSwitchRed();
};

struct RoundLargeSwitch : BasicSwitch {
	RoundLargeSwitch();
};

struct PauseSwitch : BasicSwitch {
	PauseSwitch();
};
