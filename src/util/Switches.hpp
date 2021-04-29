void toggleParamValue(Param& param);
float rescaleInput(Input& port, int channel = 0);

struct OnOff : ParamQuantity {
	std::string getDisplayValueString() override;
};

struct BasicSwitch : SvgSwitch, ManualText {
	BasicSwitch();
	void addFrame(const std::string& filename);
};

struct BasicRoundSwitch : BasicSwitch {
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

struct RoundSwitchMediumLink : BasicRoundSwitch {
	RoundSwitchMediumLink();
};

struct RoundLargeSwitch : BasicRoundSwitch {
	RoundLargeSwitch();
};

struct PauseSwitch : BasicSwitch {
	PauseSwitch();
};
