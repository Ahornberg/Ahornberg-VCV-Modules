#include "TrackerWidget.hpp"
#include "../../include/WindowManager.h"


TrackerScreen::TrackerScreen(Rect box, Tracker* module) : SizedTransparentWidget(box) {
	this->module = module;
	fontPath = asset::plugin(pluginInstance, FONT_INCONSOLATA);
}

void TrackerScreen::draw(const DrawArgs& disp) {
	std::shared_ptr<Font> font = APP->window->loadFont(fontPath);
	if (font && font->handle >= 0) {
		nvgBeginPath(disp.vg);
		nvgRect(disp.vg, 0, 0, box.size.x, box.size.y);
		// nvgFillColor(disp.vg, COLOR_BLACK);
		nvgFillColor(disp.vg, nvgTransRGBA(COLOR_BLACK, 0x80));
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
			
			nvgText(disp.vg, 10, i * 1.1875 - 2.25, (std::to_string(i / 10) + "---\u02DCOgOgOgOg").c_str(), NULL);
			nvgText(disp.vg, 10, i * 1.1875 - 2.25, (std::to_string(i / 10) + "---\u02DC\u02C7~\u02DCOgOgOgOg").c_str(), NULL);
			nvgText(disp.vg, 10, i * 1.1875 - 2.25, (std::to_string(i / 10) + "---vpfrOgOgOgOg").c_str(), NULL);
		}
		nvgText(disp.vg, 10, 10, text.c_str(), NULL);
	}
}

void TrackerScreen::onHoverScroll(const HoverScrollEvent& e) {
	if ((APP->window->getMods() & RACK_MOD_MASK) == RACK_MOD_CTRL) {
		// Increase zoom
		float zoomDelta = e.scrollDelta.y / 50 / 4;
		if (settings::invertZoom)
			zoomDelta *= -1;
		float zoom = dynamic_cast<ZoomWidget*>(getParent())->getZoom() * std::pow(2.f, zoomDelta);
		zoom = math::clamp(zoom, std::pow(2.f, -2), std::pow(2.f, 2));
		dynamic_cast<ZoomWidget*>(getParent())->setZoom(zoom);
		e.consume(this);
	}

	// if (e.isConsumed())
		// return;
	// ScrollWidget::onHoverScroll(e);
}

TrackerWidget::TrackerWidget(Tracker* module) {
	setModule(module);
	// setPanel("res/Tracker.svg");
	setWidthInHP(12);
	if (module) {
		WindowManager::getInstance();
		if (APP->scene->browser->getFirstDescendantOfType<TrackerScreen>()) {
			DEBUG("found %f", 0.0);
		}
		trackerScreen = new TrackerScreen(Rect(0, 0, 180, 380), module);
		zoomWidget = new ZoomWidget;
		zoomWidget->setZoom(2.0);
		zoomWidget->setPosition(Vec(10, 40));
		zoomWidget->addChild(trackerScreen);
		
		APP->scene->addChildBelow(zoomWidget, APP->scene->browser);
		trackerScreen->text = "?";
	}
	trackerScreen = new TrackerScreen(Rect(0, 0, 180, 380), module);
	trackerScreen->text = "?";
	addChild(trackerScreen);
}

// TrackerWidget::~TrackerWidget() {
	// if (module) {
		// // zoomWidget->removeChild(trackerScreen);
		// // APP->scene->removeChild(zoomWidget);
		// // delete zoomWidget;
		// // delete trackerScreen;
	// }
// }

void TrackerWidget::step() {
	if (module) {
		trackerScreen->text = std::to_string(zoomWidget->getZoom());
	}
}

Model* modelTracker = createModel<Tracker, TrackerWidget>("Tracker");
