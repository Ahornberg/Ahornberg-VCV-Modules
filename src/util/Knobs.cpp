#include "../Ahornberg.hpp"

BasicKnob::BasicKnob() {
	sw = new ThemedSvgWidget;
	tw->addChild(sw);
	minAngle = -.8f * M_PI;
	maxAngle = .8f * M_PI;
	speed = 0.5f;
	shadow->hide();
}

void BasicKnob::setSvg(const std::string& filename, const std::string& filenameDark) {
	// if (SvgKnob::svg == sw->svg)
		// return;
	
	dynamic_cast<ThemedSvgWidget*>(sw)->setSvg(
		APP->window->loadSvg(asset::plugin(pluginInstance, filename)),
		APP->window->loadSvg(asset::plugin(pluginInstance, filenameDark))
	);
	tw->box.size = sw->box.size;
	fb->box.size = sw->box.size;
	box.size = sw->box.size;
	
	shadow->box.size = sw->box.size;
	// Move shadow downward by 10%
	shadow->box.pos = math::Vec(0, sw->box.size.y * 0.10);
	// shadow->box = shadow->box.grow(math::Vec(2, 2));

	fb->setDirty();
}

void BasicKnob::step() {
	if (previousPreferDarkPanels != settings::preferDarkPanels) {
		fb->setDirty();
		previousPreferDarkPanels = settings::preferDarkPanels;
	}
	SvgKnob::step();
}

KnobScrew::KnobScrew() {
	BasicKnob::setSvg("res/knobs/ScrewWithDot.svg", "res/knobs/ScrewWithDot-dark.svg");
}

KnobScrewSnap::KnobScrewSnap() {
	snap = true;
}

KnobTiny::KnobTiny() {
	BasicKnob::setSvg("res/knobs/Tiny.svg", "res/knobs/Tiny-dark.svg");
}

KnobTinySnap::KnobTinySnap() {
	snap = true;
}

KnobSmall::KnobSmall() {
	BasicKnob::setSvg("res/knobs/Small.svg", "res/knobs/Small-dark.svg");
}

KnobSmallSnap::KnobSmallSnap() {
	snap = true;
}

KnobSmallSnapFast::KnobSmallSnapFast() {
	speed = 0.8f;
}

KnobBig::KnobBig() {
	BasicKnob::setSvg("res/knobs/Big.svg", "res/knobs/Big-dark.svg");
}

KnobLarge::KnobLarge() {
	BasicKnob::setSvg("res/knobs/Large.svg", "res/knobs/Large-dark.svg");
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
