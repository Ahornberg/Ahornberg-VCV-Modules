#include "Tracker.hpp"

struct TrackerScreen : SizedTransparentWidget {
	std::string fontPath;
	Tracker* module;
		
	
	TrackerScreen(Rect box, Tracker* module);
	void draw(const DrawArgs& disp) override;
};

struct TrackerWidget : ModuleWidgetWithScrews {
	TrackerWidget(Tracker* module);
};
