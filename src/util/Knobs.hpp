struct BasicKnob : SvgKnob {
	BasicKnob() {
		minAngle = -.8f * M_PI;
		maxAngle = .8f * M_PI;
		speed = 0.5f;
		shadow->hide();
	}

	void setSvg(const std::string& filename) {
		SvgKnob::setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
	}	
};

struct KnobScrew : BasicKnob {
	KnobScrew() {
		BasicKnob::setSvg("res/knobs/Screw.svg");
	}
};

struct KnobSmall : BasicKnob {
	KnobSmall() {
		BasicKnob::setSvg("res/knobs/Small.svg");
		snap = true;
	}
};

struct KnobBig : BasicKnob {
	KnobBig() {
		BasicKnob::setSvg("res/knobs/Big.svg");
	}
};

struct KnobLarge : BasicKnob {
	KnobLarge() {
		BasicKnob::setSvg("res/knobs/Large.svg");
		minAngle = -0.95f * M_PI;
		maxAngle = 0.95f * M_PI;
		snap = true;
	}
};
