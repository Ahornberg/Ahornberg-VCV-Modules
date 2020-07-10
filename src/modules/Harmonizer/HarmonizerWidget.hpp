#include "Harmonizer.hpp"

struct HarmonicsDisplay : Display {
	Harmonizer* harmonizer;
	int channel;
	int harmonics;
	int subharmonics;

	HarmonicsDisplay(Rect box, Harmonizer* harmonizer, int channel);
	void drawText(const DrawArgs &disp) override;
};

struct MixtureSwitch : RoundSwitch {
	Harmonizer* harmonizer;
	int mixture;
	
	void onChange(const event::Change& e) override;
};

struct HarmonizerWidget : ModuleWidgetWithScrews {
	HarmonizerWidget(Harmonizer* module);
};
