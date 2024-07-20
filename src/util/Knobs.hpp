constexpr static float PLUS_6_DB = 1.41253757f;

struct BasicKnob : SvgKnob {
	bool previousPreferDarkPanels;
	
	BasicKnob();
	void setSvg(const std::string& filename, const std::string& filenameDark);
	void step() override;
};

struct KnobScrew : BasicKnob {
	KnobScrew();
};

struct KnobScrewSnap : KnobScrew {
	KnobScrewSnap();
};

struct KnobTiny : BasicKnob {
	KnobTiny();
};

struct KnobTinySnap : KnobTiny {
	KnobTinySnap();
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

struct BasicSlider : SvgSlider {
	void setBackgroundSvg(const std::string& filename);
	void setHandleSvg(const std::string& filename);
};
