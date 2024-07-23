#include "Tracker.hpp"


struct TrackerScreen : SizedTransparentWidget {
	std::string fontPath;
	Tracker* module;
	std::string text;
		
	TrackerScreen(Rect box, Tracker* module);
	void draw(const DrawArgs& disp) override;
	void onHoverScroll(const HoverScrollEvent& e) override;
};

struct TrackerWidget : BaseModuleWidget {
	// ZoomWidget* zoomWidget;
	TrackerScreen* trackerScreen;
	
	TrackerWidget(Tracker* module);
	//~TrackerWidget();
	void step() override;
};
