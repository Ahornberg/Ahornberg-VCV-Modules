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
	void onButton(const event::Button& e) override;
};


struct CueForwardsSwitch : TransportCueSwitch {
	CueForwardsSwitch();
	void onButton(const event::Button& e) override;
};

struct PlayBackwardsSwitch : TransportSwitch {
	PlayBackwardsSwitch();
	void onButton(const event::Button& e) override;
};

struct PlayForwardsSwitch : TransportSwitch {
	PlayForwardsSwitch();
	void onButton(const event::Button& e) override;
};

struct KnobWheel : SvgKnob {
	const static int SMEARED_WHEELS_DISTRIBUTION[];
	
	TapeRecorder* module;
	int paramId;
	Vec* mousePos;
	// Vec* oldMousePos;
	GLFWcursor* cursorHand;
	constexpr static int NUM_SMEARED_WHEELS = 32;
	widget::TransformWidget* smearedWheelsTransform[NUM_SMEARED_WHEELS];
	widget::SvgWidget* smearedWheelsSvg[NUM_SMEARED_WHEELS];
	float smearedWheelsAngle[NUM_SMEARED_WHEELS];
	
	KnobWheel();
	void setSvgSmeared (std::shared_ptr<Svg> svg);
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
	void onChange(const event::Change& e) override;
	// void draw(const DrawArgs& args) override;
};

struct TapePositionDisplay : Display {
	TapeRecorder* tapeRecorder;
	double tapePosition;
	int beatsPerBar;
	int loopMode;
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
	bool changeTapeInterrupt;
	int loadProgress;

	TapePositionDisplay(Rect box, TapeRecorder* tapeRecorder);
	void drawText(const DrawArgs& disp) override;
};

struct ModuleLinkedWidget : SizedTransparentWidget {
	TapeRecorder* tapeRecorder;
	
	ModuleLinkedWidget(Rect box, TapeRecorder* tapeRecorder);
};

struct TapeDisplay : ModuleLinkedWidget {
	constexpr static int RADIUS_MIN = 21;
	constexpr static int RADIUS_MAX = 24;

	NVGcolor tapeColor;
	double tapeOnLeftWheel;
	double tapeOnRightWheel;
	float tapeLength;
	
	TapeDisplay(Rect box, TapeRecorder* tapeRecorder);
	void draw(const DrawArgs& disp) override;
};

struct TextOnCassette {
	std::string fontPath;
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
	TapeRecorder* tapeRecorder;
	
	TapeNameDisplay(Rect box, TapeRecorder* tapeRecorder);
	void draw(const DrawArgs& disp) override;
};

struct TapeRecorderMenuItem : MenuItem {
	TapeRecorder* tapeRecorder;
	
	TapeRecorderMenuItem(TapeRecorder* tapeRecorder);
};

struct TapeNameMenuItem : TextFieldMenuItem {
	TapeRecorder* tapeRecorder;
	
	TapeNameMenuItem(TapeRecorder* tapeRecorder);
	void onChange(const event::Change& e) override;
};

struct LoopModeValueItem : TapeRecorderMenuItem {
	const static std::string LOOP_MODES[];
	constexpr static int NUM_LOOP_MODES = 2;
	
	int loopMode;
	
	LoopModeValueItem(TapeRecorder* tapeRecorder, int loopMode);
	void onAction(const event::Action& e) override;
};

struct LoopModeMenuItem : TapeRecorderMenuItem {
	LoopModeMenuItem(TapeRecorder* tapeRecorder);
	Menu* createChildMenu() override;
};

struct TrackCountValueItem : TapeRecorderMenuItem {
	int trackCount;
	
	TrackCountValueItem(TapeRecorder* tapeRecorder, int trackCount, std::string trackCountText);
	void onAction(const event::Action& e) override;
};

struct TrackCountMenuItem : TapeRecorderMenuItem, TrackCountText {
	TrackCountMenuItem(TapeRecorder* tapeRecorder);
	Menu* createChildMenu() override;
};

struct TapeLengthValueItem : TapeRecorderMenuItem {
	int tapeLength;
	
	TapeLengthValueItem(TapeRecorder* tapeRecorder, int tapeLength);
	void onAction(const event::Action& e) override;
};

struct TapeLengthMenuItem : TapeRecorderMenuItem {
	TapeLengthMenuItem(TapeRecorder* tapeRecorder);
	Menu* createChildMenu() override;
};

struct TapeStripesValueItem : MenuItem {
	TapeRecorder* tapeRecorder;
	int stripe;
	
	TapeStripesValueItem(TapeRecorder* tapeRecorder, int stripe);
	void onAction(const event::Action& e) override;
};

struct TapeStripesMenuItem : MenuItem {
	TapeRecorder* tapeRecorder;
	
	TapeStripesMenuItem(TapeRecorder* tapeRecorder);
	Menu* createChildMenu() override;
};

struct EraseTapeMenuItem : TapeRecorderMenuItem {
	EraseTapeMenuItem(TapeRecorder* tapeRecorder);
	void onAction(const event::Action& e) override;
};

struct TapeRecorderWidget : BaseModuleWidget {
	// TapeNameDisplay* tapeNameDisplay;
	StripeWidget* stripeWidget;
	// TextField* tapeName;
	
	TapeRecorderWidget(TapeRecorder* module);
	void contextMenu(Menu* menu) override;
	void step() override;
};
