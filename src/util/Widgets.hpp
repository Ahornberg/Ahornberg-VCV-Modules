#define ENUMS_WITH_SCREWS(name, count) name = NUM_MAX_SCREWS, name ## _LAST = name + (count) - 1

static const char SELECTION_FILTERS[] = "VCV Rack module selection (.vcvs):vcvs";

struct SizedTransparentWidget : TransparentWidget {
	SizedTransparentWidget(Rect box);
};

struct TextFieldMenuItem : TextField {
	TextFieldMenuItem();
	void onAction(const event::Action& e) override;
};

// Module with intern params and Widget with screws

constexpr int NUM_MAX_SCREWS = 4;
//constexpr int NUM_MAX_INTERN_PARAM_TYPES = 2;
constexpr bool DISPLAY_SCREWS = false;

struct BaseModule : Module {
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
		SCREW_BOTTOM_LEFT,
		SCREW_BOTTOM_LEFT_INDENTED
	};
	enum ScrewBottomRight {
		NO_SCREW_BOTTOM_RIGHT,
		SCREW_BOTTOM_RIGHT
	};
	
	bool hasScrews;
	
	ModuleWidgetWithScrews();
	void setPanel(const std::string& filename);
	void setWidthInHP(int hp);
	void setScrews(ScrewTopLeft topLeft, ScrewTopRight topRight, ScrewBottomLeft bottomLeft, ScrewBottomRight bottomRight);
	void addScrew(Vec pos, int screwParam);
	void step() override;
	bool isBypassed();
	void appendContextMenu(Menu* menu) override;
	virtual void contextMenu(Menu* menu);
	void loadSelectionDialog();
	void loadSelection(std::string path);
	void onButton(const ButtonEvent& e) override;
	virtual void createCustomContextMenu();
};
