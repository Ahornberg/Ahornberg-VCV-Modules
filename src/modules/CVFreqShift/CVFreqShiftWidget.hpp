#include "CVFreqShift.hpp"

struct MultiplierKnob : KnobSmallSnap {
	CVFreqShift* module;
	
	void onChange(const event::Change& e) override;
};

struct CVFreqShiftWidget : BaseModuleWidget {
	CVFreqShiftWidget(CVFreqShift* module);
};
