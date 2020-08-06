#include "ManualWidget.hpp"

ManualWidget::ManualWidget(Manual* module) {
	setModule(module);
	setPanel("res/Manual.svg");
	setSize(Vec(90, 380));
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, NO_SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);



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
			DEBUG(manualText->manualTitle.c_str());
			DEBUG(manualText->manualContent.c_str());
		}
	}
	ModuleWidgetWithScrews::step();
}

Model* modelManual = createModel<Manual, ManualWidget>("Manual");
