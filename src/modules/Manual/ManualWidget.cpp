#include "ManualWidget.hpp"

ManualWidget::ManualWidget(Manual* module) {
	setModule(module);
	setPanel("res/Manual.svg");
	setWidthInHP(6);

	// addChild(new IntervalDisplay(Rect(8.5, 98, 74, 18), module));
}

void ManualWidget::step() {
	Widget* widget = APP->event->getHoveredWidget();
	if (widget) {
		ManualText* manualText = dynamic_cast<ManualText*>(widget);
		if (!manualText) {
			manualText = widget->getAncestorOfType<ManualText>();
		}
		if (manualText && manualText != lastManualText && !manualText->manualTitle.empty()) {
			lastManualText = manualText;
			DEBUG("%s", manualText->manualTitle.c_str());
			DEBUG("%s", manualText->manualContent.c_str());
		}
	}
	BaseModuleWidget::step();
}

Model* modelManual = createModel<Manual, ManualWidget>("Manual");
