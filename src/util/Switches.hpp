struct BasicSwitch : SvgSwitch, ManualText {
	BasicSwitch();
	void addFrame(const std::string& filename);
};

struct RoundSwitch : BasicSwitch {
	RoundSwitch();
};

struct RoundSwitchMedium : BasicSwitch {
	RoundSwitchMedium();
};

struct RoundSwitchRed : BasicSwitch {
	RoundSwitchRed();
};

struct RoundSwitchLinearJump : BasicSwitch {
	RoundSwitchLinearJump();
};

struct RoundLargeSwitch : BasicSwitch {
	RoundLargeSwitch();
};

struct PauseSwitch : BasicSwitch {
	PauseSwitch();
};
