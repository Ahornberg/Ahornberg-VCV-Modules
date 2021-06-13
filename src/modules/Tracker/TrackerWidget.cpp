#include "TrackerWidget.hpp"

TrackerScreen::TrackerScreen(Rect box, Tracker* module) : SizedTransparentWidget(box) {
	this->module = module;
	font = APP->window->loadFont(asset::plugin(pluginInstance, FONT_COMMODORE));
}

void TrackerScreen::draw(const DrawArgs& disp) {
	nvgBeginPath(disp.vg);
	nvgFillColor(disp.vg, COLOR_GREEN_DARK);
	nvgFontSize(disp.vg, 11);
	nvgFontFaceId(disp.vg, font->handle);
	//nvgTextLetterSpacing(disp.vg, 1);
	for (auto i = 10; i <= 380; i += 10) {
		nvgText(disp.vg, 10, i * 1.027 - 1, (std::to_string(i / 10) + " OgOgOgOg").c_str(), NULL);
	}
}

TrackerWidget::TrackerWidget(Tracker* module) {
	setModule(module);
	setPanel("res/Tracker.svg");
	setWidthInHP(12);

	addChild(new TrackerScreen(Rect(0, 0, 180, 380), module));
}

Model* modelTracker = createModel<Tracker, TrackerWidget>("Tracker");
