void toggleParamValue(Param& param);
float rescaleInput(Input& port, int channel = 0);

struct BasicSwitch : SvgSwitch {
	const static std::vector<std::string> ON_OFF_NAMES;
	bool previousPreferDarkPanels;
	bool hasContextMenu;
	
	BasicSwitch();
	void addFrame(const std::string& filename, const std::string& filenameDark);
	void onDragStart(const DragStartEvent& e) override;
	void onDragEnd(const DragEndEvent& e) override;
	void onChange(const ChangeEvent& e) override;
	void onButton(const ButtonEvent& e) override;
	void step() override;
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

struct MidiButtonSmall : BasicSwitch {
    MidiButtonSmall();
	midi::Port* port;
	void setMidiPort(midi::Port* port);
	void onDragStart(const DragStartEvent& e) override;
};
 