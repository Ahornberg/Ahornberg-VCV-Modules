#include "CopyPastePreset.hpp"

struct CopyPastePresetWidget : ModuleWidgetWithScrews {
	constexpr static int WAIT_COUNTER = 1;
	
	int waitCounter;
	
	CopyPastePresetWidget(CopyPastePreset* module);
	void step() override;
};
