#include "../Ahornberg.hpp"

BasicKnob::BasicKnob() {
	minAngle = -.8f * M_PI;
	maxAngle = .8f * M_PI;
	speed = 0.5f;
	shadow->hide();
}

void BasicKnob::setSvg(const std::string& filename) {
	SvgKnob::setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}	

KnobScrew::KnobScrew() {
	BasicKnob::setSvg("res/knobs/ScrewWithDot.svg");
}

KnobScrewMountModule::KnobScrewMountModule() {
	minAngle = -5 * M_PI;
	maxAngle = 5 * M_PI;
	speed = 0.1f;
	shadow->show();
	shadow->box.pos = Vec(0, 0);
	shadow->blurRadius = 3;
	BasicKnob::setSvg("res/knobs/Screw.svg");
	manualTitle = "Screw";
	manualContent = "This is a Screw.";
}

void KnobScrewMountModule::onChange(const event::Change& e) {
	BasicKnob::onChange(e);
	if (module) {
		if (!module->params[param].getValue()) {
			hide();
		}
		shadow->box.pos = Vec(0, 10 - module->params[param].getValue() * 2);
	}
}

KnobTiny::KnobTiny() {
	BasicKnob::setSvg("res/knobs/Tiny.svg");
}

KnobSmall::KnobSmall() {
	BasicKnob::setSvg("res/knobs/Small.svg");
}

KnobSmallSnap::KnobSmallSnap() {
	snap = true;
}

KnobBig::KnobBig() {
	BasicKnob::setSvg("res/knobs/Big.svg");
}

KnobLarge::KnobLarge() {
	BasicKnob::setSvg("res/knobs/Large.svg");
	minAngle = -0.95f * M_PI;
	maxAngle = 0.95f * M_PI;
	snap = true;
}
