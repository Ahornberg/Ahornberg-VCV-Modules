#include "Manual.hpp"

struct ManualWidget : ModuleWidgetWithScrews {
	ManualText* lastManualText;

	ManualWidget(Manual* module);
	void step() override;
};
