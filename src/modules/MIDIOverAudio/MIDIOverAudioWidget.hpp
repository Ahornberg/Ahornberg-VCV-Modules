#include "MIDIOverAudio.hpp"

// struct IntervalDisplay : Display {
	// EqualDivision *equalDivision;
	// int step;
	// int interval;

	// IntervalDisplay(Rect box, EqualDivision *equalDivision);
	// void drawText(const DrawArgs& disp) override;
// };

struct MIDIOverAudioWidget : ModuleWidgetWithScrews {
	MIDIOverAudioWidget(MIDIOverAudio* module);
};
