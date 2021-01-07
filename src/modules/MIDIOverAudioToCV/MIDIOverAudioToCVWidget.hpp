#include "MIDIOverAudioToCV.hpp"

// struct IntervalDisplay : Display {
	// EqualDivision *equalDivision;
	// int step;
	// int interval;

	// IntervalDisplay(Rect box, EqualDivision *equalDivision);
	// void drawText(const DrawArgs& disp) override;
// };

struct MIDIOverAudioToCVWidget : ModuleWidgetWithScrews {
	MIDIOverAudioToCVWidget(MIDIOverAudioToCV* module);
};
