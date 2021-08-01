#include "TrackerWidget.hpp"

TrackerScreen::TrackerScreen(Rect box, Tracker* module) : SizedTransparentWidget(box) {
	this->module = module;
	font = APP->window->loadFont(asset::plugin(pluginInstance, FONT_INCONSOLATA));
}

void TrackerScreen::draw(const DrawArgs& disp) {
	nvgBeginPath(disp.vg);
	nvgRect(disp.vg, 0, 0, box.size.x, box.size.y);
	nvgFillColor(disp.vg, COLOR_BLACK);
	nvgFill(disp.vg);
	nvgBeginPath(disp.vg);
	nvgRect(disp.vg, 0, 0, box.size.x, 11.875);
	nvgFillColor(disp.vg, COLOR_BROWN);
	nvgFill(disp.vg);
	nvgFillColor(disp.vg, COLOR_GREEN_DARK);
	nvgFontSize(disp.vg, 12);
	nvgFontFaceId(disp.vg, font->handle);
	//nvgTextLetterSpacing(disp.vg, 1);
	for (auto i = 10; i <= 380; i += 10) {
		if (((i - 10) / 10) % 4) {
			nvgFillColor(disp.vg, COLOR_GREEN_DARK);
		} else {
			nvgFillColor(disp.vg, COLOR_YELLOW);
		}
		// nvgText(disp.vg, 10, i * 1.1875 - 2.25, (std::to_string(i / 10) + "---\u02DCOgOgOgOg").c_str(), NULL);
		nvgText(disp.vg, 10, i * 1.1875 - 2.25, (std::to_string(i / 10) + "---\u02DC\u02C7~\u02DCOgOgOgOg").c_str(), NULL);
		nvgText(disp.vg, 10, i * 1.1875 - 2.25, (std::to_string(i / 10) + "---vpfrOgOgOgOg").c_str(), NULL);
	}
}

TrackerWidget::TrackerWidget(Tracker* module) {
	setModule(module);
	// setPanel("res/Tracker.svg");
	setWidthInHP(12);

	addChild(new TrackerScreen(Rect(0, 0, 180, 380), module));
}

Model* modelTracker = createModel<Tracker, TrackerWidget>("Tracker");
