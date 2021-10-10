void toggleParamValue(Param& param);
float rescaleInput(Input& port, int channel = 0);

struct BasicSwitch : SvgSwitch, ManualText {
	const static std::vector<std::string> ON_OFF_NAMES;
	bool hasContextMenu;
	
	BasicSwitch();
	void addFrame(const std::string& filename);
	void onButton(const ButtonEvent& e) override;
};

struct BasicRoundSwitch : BasicSwitch {
	BasicRoundSwitch();
	void onHover(const event::Hover& e) override;
	void onButton(const event::Button& e) override;
};

struct RoundSwitch : BasicRoundSwitch {
	RoundSwitch();
};

struct RoundSwitchMedium : BasicRoundSwitch {
	RoundSwitchMedium();
};

struct RoundSwitchRed : BasicRoundSwitch {
	RoundSwitchRed();
};

struct RoundSwitchMediumRed : BasicRoundSwitch {
	RoundSwitchMediumRed();
};

struct RoundSwitchLinearJump : BasicRoundSwitch {
	RoundSwitchLinearJump();
};

struct RoundSwitchLarge : BasicRoundSwitch {
	RoundSwitchLarge();
};

struct PauseSwitch : BasicSwitch {
	PauseSwitch();
};
