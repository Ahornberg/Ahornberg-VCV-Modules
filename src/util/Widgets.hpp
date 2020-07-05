struct SizedTransparentWidget : LightWidget {
	SizedTransparentWidget(Rect box) {
		this->box = box;
	}
};

struct TextFieldMenuItem : TextField {
	TextFieldMenuItem() {
		box.size = Vec(120, 20);
		multiline = false;
	}
	
	void onAction(const event::Action& e) override {
		TextField::onAction(e);
		getAncestorOfType<MenuOverlay>()->requestDelete();
	}
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
	std::string getDisplayValueString() override {
		return SCREW_MESSAGES[(int) getValue()];
	}
};

struct ModuleWithScrews : Module {
	enum Screws {
		ENUMS(SCREW_PARAM, NUM_MAX_SCREWS)
	};
	
	void configScrewParams() {
		for (auto i = 0; i < NUM_MAX_SCREWS; ++i) {
			configParam<ScrewMessage>(SCREW_PARAM + i, 0, 5, 5, "Screw");
		}
	}
	
};

struct ModuleWidgetWithScrews : ModuleWidget {
	void setPanel(const std::string& filename) {
		ModuleWidget::setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
	}
	
	void setScrews(bool topLeft, bool topRight, bool bottomLeft, bool bottomRight) {
		if (topLeft) {
			addScrew(Vec(0, 0), ModuleWithScrews::SCREW_PARAM);
		} else if (module) {
			module->params[ModuleWithScrews::SCREW_PARAM].setValue(0);
		}
		if (topRight) {
			addScrew(Vec(box.size.x - RACK_GRID_WIDTH, 0), ModuleWithScrews::SCREW_PARAM + 1);
		} else if (module) {
			module->params[ModuleWithScrews::SCREW_PARAM + 1].setValue(0);
		}
		if (bottomLeft) {
			addScrew(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), ModuleWithScrews::SCREW_PARAM + 2);
		} else if (module) {
			module->params[ModuleWithScrews::SCREW_PARAM + 2].setValue(0);
		}
		if (bottomRight) {
			addScrew(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), ModuleWithScrews::SCREW_PARAM + 3);
		} else if (module) {
			module->params[ModuleWithScrews::SCREW_PARAM + 3].setValue(0);
		}
	}
	
	void addScrew(Vec pos, int screwParam) {
		KnobScrewMountModule* screw = dynamic_cast<KnobScrewMountModule*>(createParam<KnobScrewMountModule>(pos, module, screwParam));
		screw->module = module;
		screw->param = screwParam;
		addParam(screw);
	}

	void step() override {
		ModuleWidget::step();
		if (module) {
			for (auto i = 0; i < NUM_MAX_SCREWS; ++i) {
				if (module->params[ModuleWithScrews::SCREW_PARAM + i].getValue()) {
					return;
				}
			}
			removeAction();
		}
	}
};