#include "Metronome.hpp"

struct TempoDisplay : Display {
	Metronome* metronome;
	int bpmIndex;

	TempoDisplay(Rect box, Metronome* metronome);
	void drawText(const DrawArgs& disp) override;
};

struct BpmDisplay : TransparentWidget {
	std::string fontPath;
	NVGcolor textColor;
	int bpmIndex;

	BpmDisplay(Vec pos, int bpmIndex);
	void draw(const DrawArgs& disp) override;
};

struct RadioSwitch : RoundSwitch {
	Metronome* metronome;
	int bpmIndex;
	
	RadioSwitch();
	void onChange(const event::Change& e) override;
};

struct KnobRadioSwitch : KnobLarge {
	Metronome* metronome;
	
	void onChange(const event::Change& e) override;
};

struct MetronomeWidget : ModuleWidgetWithScrews {
	MetronomeWidget(Metronome* module);
	Vec positionOnCircle(int positionInList, Vec center, int radius);
};
