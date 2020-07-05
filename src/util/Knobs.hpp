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
		BasicKnob::setSvg("res/knobs/ScrewWithDot.svg");
	}
};

struct KnobScrewMountModule : BasicKnob {
	Module* module;
	int param;

	KnobScrewMountModule() {
		minAngle = -5 * M_PI;
		maxAngle = 5 * M_PI;
		speed = 0.1f;
		shadow->show();
		shadow->box.pos = Vec(0, 0);
		shadow->blurRadius = 3;
		BasicKnob::setSvg("res/knobs/Screw.svg");
	}
	
	void onChange(const event::Change& e) override {
		BasicKnob::onChange(e);
		if (module) {
			if (!module->params[param].getValue()) {
				hide();
			}
			shadow->box.pos = Vec(0, 10 - module->params[param].getValue() * 2);
		}
	}
};

struct KnobTiny : BasicKnob {
	KnobTiny() {
		BasicKnob::setSvg("res/knobs/Tiny.svg");
	}
};

struct KnobSmall : BasicKnob {
	KnobSmall() {
		BasicKnob::setSvg("res/knobs/Small.svg");
	}
};

struct KnobSmallSnap : KnobSmall {
	KnobSmallSnap() {
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
