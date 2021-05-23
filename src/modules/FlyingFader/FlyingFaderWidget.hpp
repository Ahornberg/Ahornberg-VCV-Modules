#include "FlyingFader.hpp"

struct FaderCapColor {
	int index;
	std::string color;
};

struct MotorizedFader : BasicSlider {
	FlyingFader* flyingFader;
	bool displayContextMenu;
	// float oldValue;
	
	MotorizedFader();
	void onButton(const event::Button& e) override;
	// void onChange(const event::Change& e) override;
	void onDragStart(const event::DragStart& e) override;
	void onDragEnd(const event::DragEnd& e) override;
	void step() override;
};

struct FlyingFaderWidget : ModuleWidgetWithScrews {
	const static FaderCapColor FADER_CAP_COLORS[];
	constexpr static int NUM_FADER_CAP_COLORS = 10;
	
	MotorizedFader* fader;
	// FaderDisplay* faderDisplay;
	int faderCapColorIndex;
	
	FlyingFaderWidget(FlyingFader* module);
	void changeFaderCapColor(int faderCapColorIndex);
	void appendContextMenu(Menu* menu) override;
	json_t* toJson() override;
	void fromJson(json_t* rootJ) override;
};

struct FaderCapColorValueItem : MenuItem {
	FlyingFaderWidget* flyingFaderWidget;
	int faderCapColorIndex;
	
	FaderCapColorValueItem(FlyingFaderWidget* flyingFaderWidget, int faderCapColorIndex);
	void onAction(const event::Action& e) override;
};

struct FaderCapColorMenuItem : MenuItem {
	FlyingFaderWidget* flyingFaderWidget;
	int faderCapColorIndex;
	
	FaderCapColorMenuItem(FlyingFaderWidget* flyingFaderWidget, int faderCapColorIndex);
	Menu* createChildMenu() override;
};
