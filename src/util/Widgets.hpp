#define ENUMS_WITH_SCREWS(name, count) name = NUM_MAX_SCREWS, name ## _LAST = name + (count) - 1

struct SizedTransparentWidget : LightWidget {
	SizedTransparentWidget(Rect box);
};

struct TextFieldMenuItem : TextField {
	TextFieldMenuItem();
	void onAction(const event::Action& e) override;
};

// Module and Widget with Screws

static const std::string SCREW_MESSAGES[] = {
	"I warn you, UNDO won't help you here!",
	"If you continue doing that, the module will fall out of the rack!",
	"Stop doing that!",
	"Don't pull it out!",
	"loose",
	"fixed"
};

constexpr int NUM_MAX_SCREWS = 4;

struct ScrewMessage : ParamQuantity {
	std::string getDisplayValueString() override;
};

struct ModuleWithScrews : Module {
	enum Screws {
		ENUMS(SCREW_PARAM, NUM_MAX_SCREWS)
	};
	
	void configScrewParams();
};

struct ModuleWidgetWithScrews : ModuleWidget {
	enum ScrewTopLeft {
		NO_SCREW_TOP_LEFT,
		SCREW_TOP_LEFT
	};
	enum ScrewTopRight {
		NO_SCREW_TOP_RIGHT,
		SCREW_TOP_RIGHT
	};
	enum ScrewBottomLeft {
		NO_SCREW_BOTTOM_LEFT,
		SCREW_BOTTOM_LEFT
	};
	enum ScrewBottomRight {
		NO_SCREW_BOTTOM_RIGHT,
		SCREW_BOTTOM_RIGHT
	};
	
	void setPanel(const std::string& filename);
	void setScrews(ScrewTopLeft topLeft, ScrewTopRight topRight, ScrewBottomLeft bottomLeft, ScrewBottomRight bottomRight);
	void addScrew(Vec pos, int screwParam);
	void step() override;
};
