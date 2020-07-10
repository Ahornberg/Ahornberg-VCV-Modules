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
	void setPanel(const std::string& filename);
	void setScrews(bool topLeft, bool topRight, bool bottomLeft, bool bottomRight);
	void addScrew(Vec pos, int screwParam);
	void step() override;
};
