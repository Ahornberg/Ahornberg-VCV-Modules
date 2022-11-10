#include "Manual.hpp"

struct ManualWidget : BaseModuleWidget {
	ManualText* lastManualText;

	ManualWidget(Manual* module);
	void step() override;
};
