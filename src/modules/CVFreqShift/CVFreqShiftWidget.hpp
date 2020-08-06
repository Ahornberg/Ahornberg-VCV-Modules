#include "CVFreqShift.hpp"

struct MultiplierKnob : KnobSmallSnap {
	CVFreqShift* module;
	
	void onChange(const event::Change& e) override;
};

struct CVFreqShiftWidget : ModuleWidgetWithScrews {
	CVFreqShiftWidget(CVFreqShift* module);
};
