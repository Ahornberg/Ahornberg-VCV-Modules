#include "TapeRecorder.hpp"

struct Stripe {
	std::string fileName;
	std::string name;
};


struct StripeWidget : widget::SvgWidget {
	const static Stripe STRIPES[];
	constexpr static int NUM_STRIPES = 12;
	
	int stripe;
	
	StripeWidget(Vec pos);
	void setStripe(int stripe);
};

struct TransportSwitch : BasicSwitch {
	TapeRecorder* tapeRecorder;
};

struct TransportCueSwitch : TransportSwitch {
	TapeRecorder* tapeRecorder;
	
	void step() override;
};

struct CueBackwardsSwitch : TransportCueSwitch {
	CueBackwardsSwitch();
	void onChange(const event::Change& e) override;
};


struct CueForwardsSwitch : TransportCueSwitch {
	CueForwardsSwitch();
	void onChange(const event::Change& e) override;
};

struct PlayBackwardsSwitch : TransportSwitch {
	PlayBackwardsSwitch();
	void onChange(const event::Change& e) override;
};

struct PlayForwardsSwitch : TransportSwitch {
	PlayForwardsSwitch();
	void onChange(const event::Change& e) override;
};

struct KnobWheel : SvgKnob {
	TapeRecorder* module;
	Vec* mousePos;
	// Vec* oldMousePos;
	GLFWcursor* cursorHand;
	
	KnobWheel();
	float distance(Vec* p1, Vec* p2);
	float calcTouchedWheelForce(float distance, float maxDistance, int mods);
	void onHover(const event::Hover& e) override;
	void onButton(const event::Button& e) override;
	void onDragHover(const event::DragHover& e) override;
	void onDragStart(const event::DragStart& e) override;
	void onDragEnd(const event::DragEnd& e) override;
	void onDragLeave(const event::DragLeave& e) override;
	void onDragMove(const event::DragMove& e) override;
	void onDoubleClick(const event::DoubleClick& e) override;
};

struct TapePositionDisplay : Display {
	TapeRecorder* tapeRecorder;
	double tapePosition;
	int beatsPerBar;
	int loopStart;
	int loopEnd;
	bool loopStartConnected;
	bool loopEndConnected;
	bool loopStartOnTapePosition;
	bool loopEndOnTapePosition;
	bool playStatus;
	bool cueStatus;
	bool playForwardStatus;
	bool cueForwardStatus;

	TapePositionDisplay(Rect box, TapeRecorder* tapeRecorder);
	void drawText(const DrawArgs& disp) override;
};

struct ModuleLinkedWidget : SizedTransparentWidget {
	TapeRecorder* tapeRecorder;
	
	ModuleLinkedWidget(Rect box, TapeRecorder* tapeRecorder);
};

struct TapeDisplay : ModuleLinkedWidget {
	constexpr static int RADIUS_MIN = 21;
	constexpr static int RADIUS_MAX = 35;

	NVGcolor tapeColor;
	double tapeOnLeftWheel;
	double tapeOnRightWheel;
	
	TapeDisplay(Rect box, TapeRecorder* tapeRecorder);
	void draw(const DrawArgs& disp) override;
};

struct TextOnCassette {
	std::shared_ptr<Font> font;
	NVGcolor textColor;
	std::string text;
	int fontSize;
	int textAlign;
	bool useScissor;
	Vec textPos;
	
	void drawText(const Widget::DrawArgs& disp, Rect box);
};

struct TapeLengthDisplay : ModuleLinkedWidget, TextOnCassette {
	TapeLengthDisplay(Rect box, TapeRecorder* tapeRecorder);
	void draw(const DrawArgs& disp) override;
};

struct TrackCountText {
	std::string createTrackCountText(const int trackCount);
};

struct TrackCountDisplay : ModuleLinkedWidget, TextOnCassette, TrackCountText {
	TrackCountDisplay(Rect box, TapeRecorder* tapeRecorder);
	void draw(const DrawArgs& disp) override;
};

struct TapeNameDisplay : SizedTransparentWidget, TextOnCassette {
	TapeNameDisplay(Rect box);
	void draw(const DrawArgs& disp) override;
};

struct TapeRecorderMenuItem : MenuItem {
	TapeRecorder* tapeRecorder;
	
	TapeRecorderMenuItem(TapeRecorder* tapeRecorder);
};

struct TapeNameMenuItem : TextFieldMenuItem {
	TapeNameDisplay* tapeNameDisplay;
	
	TapeNameMenuItem(TapeNameDisplay* tapeNameDisplay);
	void onChange(const event::Change& e) override;
};

// struct OldSchoolModeMenuItem : TapeRecorderMenuItem {
	// OldSchoolModeMenuItem(TapeRecorder* tapeRecorder);
	// void onAction(const event::Action& e) override;
// };

// struct TrackCountValueItem : TapeRecorderMenuItem {
	// int trackCount;
	
	// TrackCountValueItem(TapeRecorder* tapeRecorder, int trackCount, std::string trackCountText);
	// void onAction(const event::Action& e) override;
// };

// struct TrackCountMenuItem : TapeRecorderMenuItem, TrackCountText {
	// TrackCountMenuItem(TapeRecorder* tapeRecorder);
	// Menu* createChildMenu() override;
// };

// struct TapeLengthValueItem : TapeRecorderMenuItem {
	// int tapeLength;
	
	// TapeLengthValueItem(TapeRecorder* tapeRecorder, int tapeLength);
	// void onAction(const event::Action& e) override;
// };

// struct TapeLengthMenuItem : TapeRecorderMenuItem {
	// TapeLengthMenuItem(TapeRecorder* tapeRecorder);
	// Menu* createChildMenu() override;
// };

struct TapeStripesValueItem : MenuItem {
	StripeWidget* stripeWidget;
	int stripe;
	
	TapeStripesValueItem(StripeWidget* stripeWidget, int stripe);
	void onAction(const event::Action& e) override;
};

struct TapeStripesMenuItem : MenuItem {
	StripeWidget* stripeWidget;
	
	TapeStripesMenuItem(StripeWidget* stripeWidget);
	Menu* createChildMenu() override;
};

struct EraseTapeMenuItem : TapeRecorderMenuItem {
	EraseTapeMenuItem(TapeRecorder* tapeRecorder);
	void onAction(const event::Action& e) override;
};


struct TapeRecorderWidget : ModuleWidgetWithScrews {
	TapeNameDisplay* tapeNameDisplay;
	StripeWidget* stripeWidget;
	TextField* tapeName;
	
	TapeRecorderWidget(TapeRecorder* module);
	void appendContextMenu(Menu* menu) override;
	json_t* toJson() override;
	void fromJson(json_t* rootJ) override;
};
