#include "TrackerWidget.hpp"

TrackerScreen::TrackerScreen(Rect box, Tracker* module) : SizedTransparentWidget(box) {
	this->module = module;
	font = APP->window->loadFont(asset::plugin(pluginInstance, FONT_INCONSOLATA));
}

void TrackerScreen::draw(const DrawArgs& disp) {
	nvgBeginPath(disp.vg);
	nvgFillColor(disp.vg, COLOR_GREEN_DARK);
	nvgFontSize(disp.vg, 14);
	nvgFontFaceId(disp.vg, font->handle);
	//nvgTextLetterSpacing(disp.vg, 1);
	for (auto i = 10; i <= 380; i += 10) {
		if (((i - 10) / 10) % 4) {
			nvgFillColor(disp.vg, COLOR_GREEN_DARK);
		} else {
			nvgFillColor(disp.vg, COLOR_YELLOW);
		}
		// nvgText(disp.vg, 10, i * 1.054 - 1, (std::to_string(i / 10) + " OgOgOgOg").c_str(), NULL);
		// 36 lines
		nvgText(disp.vg, 10, i * 1.1875 - 2, (std::to_string(i / 10) + "---OgOgOgOg").c_str(), NULL);
		// nvgText(disp.vg, 10, i * 1.583 - 2, (std::to_string(i / 10) + " OgOgOgOg").c_str(), NULL);
	}
}

TrackerWidget::TrackerWidget(Tracker* module) {
	setModule(module);
	setPanel("res/Tracker.svg");
	setWidthInHP(12);

	addChild(new TrackerScreen(Rect(0, 0, 180, 380), module));
}

Model* modelTracker = createModel<Tracker, TrackerWidget>("Tracker");
