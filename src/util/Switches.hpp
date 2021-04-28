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

struct RoundSwitchLinearJump : BasicRoundSwitch {
	RoundSwitchLinearJump();
};

struct RoundLargeSwitch : BasicRoundSwitch {
	RoundLargeSwitch();
};

struct PauseSwitch : BasicRoundSwitch {
	PauseSwitch();
};
