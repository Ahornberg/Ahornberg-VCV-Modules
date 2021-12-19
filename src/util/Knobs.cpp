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

KnobScrewSnap::KnobScrewSnap() {
	snap = true;
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

void KnobScrewMountModule::onButton(const ButtonEvent& e) {
	if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT && (e.mods & RACK_MOD_MASK) == 0) {
		BasicKnob::onButton(e);
	}
}

KnobTiny::KnobTiny() {
	BasicKnob::setSvg("res/knobs/Tiny.svg");
}

KnobTinySnap::KnobTinySnap() {
	snap = true;
}

KnobSmall::KnobSmall() {
	BasicKnob::setSvg("res/knobs/Small.svg");
}

KnobSmallSnap::KnobSmallSnap() {
	snap = true;
}

KnobSmallSnapFast::KnobSmallSnapFast() {
	speed = 0.8f;
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

void BasicSlider::setBackgroundSvg(const std::string& filename) {
	SvgSlider::setBackgroundSvg(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}	

void BasicSlider::setHandleSvg(const std::string& filename) {
	// SvgSlider::setHandleSvg(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
	handle->setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}
