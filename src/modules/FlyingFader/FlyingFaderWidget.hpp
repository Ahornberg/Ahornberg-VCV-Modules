#include "FlyingFader.hpp"

struct FaderCapColor {
	int index;
	std::string color;
};

struct MotorizedFader : BasicSlider {
	FlyingFader* flyingFader;
	bool displayContextMenu;
	float oldFaderValueBeforeConnected;
	
	MotorizedFader();
	void setFlyingFader(FlyingFader* flyingFader);
	void onButton(const event::Button& e) override;
	// void onChange(const event::Change& e) override;
	void onDragStart(const event::DragStart& e) override;
	void onDragEnd(const event::DragEnd& e) override;
	void step() override;
};

struct TextOnFaderModule {
	std::shared_ptr<Font> font;
	NVGcolor textColor;
	std::string text;
	int fontSize;
	int textAlign;
	bool useScissor;
	Vec textPos;
	
	void drawText(const Widget::DrawArgs& disp, Rect box);
};

struct FaderNameDisplay : SizedTransparentWidget, TextOnFaderModule {
	FaderNameDisplay(Rect box);
	void draw(const DrawArgs& disp) override;
};

struct FaderNameMenuItem : TextFieldMenuItem {
	FaderNameDisplay* faderNameDisplay;
	
	FaderNameMenuItem(FaderNameDisplay* faderNameDisplay);
	void onChange(const event::Change& e) override;
};

struct FlyingFaderWidget : ModuleWidgetWithScrews {
	const static FaderCapColor FADER_CAP_COLORS[];
	constexpr static int NUM_FADER_CAP_COLORS = 10;
	
	MotorizedFader* fader;
	FaderNameDisplay* faderNameDisplay;
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

struct AudioPolyModeValueItem : MenuItem {
	FlyingFader* flyingFader;
	bool audioPolyMode;
	
	AudioPolyModeValueItem(FlyingFader* flyingFader, bool audioPolyMode);
	void onAction(const event::Action& e) override;
};

struct AudioPolyModeMenuItem : MenuItem {
	FlyingFader* flyingFader;
	
	AudioPolyModeMenuItem(FlyingFader* flyingFader);
	Menu* createChildMenu() override;
};

struct CvScaleModeValueItem : MenuItem {
	FlyingFader* flyingFader;
	bool cvScaleMode;
	
	CvScaleModeValueItem(FlyingFader* flyingFader, bool cvScaleMode);
	void onAction(const event::Action& e) override;
};

struct CvScaleModeMenuItem : MenuItem {
	FlyingFader* flyingFader;
	
	CvScaleModeMenuItem(FlyingFader* flyingFader);
	Menu* createChildMenu() override;
};
