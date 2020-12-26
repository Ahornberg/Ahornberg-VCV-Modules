struct BasicKnob : SvgKnob, ManualText {
	BasicKnob();
	void setSvg(const std::string& filename);
};

struct KnobScrew : BasicKnob {
	KnobScrew();
};

struct KnobScrewMountModule : BasicKnob {
	Module* module;
	int param;

	KnobScrewMountModule();
	void onChange(const event::Change& e) override;
};

struct KnobTiny : BasicKnob {
	KnobTiny();
};

struct KnobSmall : BasicKnob {
	KnobSmall();
};

struct KnobSmallSnap : KnobSmall {
	KnobSmallSnap();
};

struct KnobSmallSnapFast : KnobSmallSnap {
	KnobSmallSnapFast();
};

struct KnobBig : BasicKnob {
	KnobBig();
};

struct KnobLarge : BasicKnob {
	KnobLarge();
};
