#include "FlyingFader.hpp"

struct MotorizedFader : BasicSlider {
	FlyingFader* flyingFader;
	
	MotorizedFader();
	void onDragStart(const event::DragStart& e) override;
	void onDragEnd(const event::DragEnd& e) override;
};

struct FlyingFaderWidget : ModuleWidgetWithScrews {
	MotorizedFader* fader;
	
	FlyingFaderWidget(FlyingFader* module);
};
