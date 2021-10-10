#include "../Ahornberg.hpp"

SizedTransparentWidget::SizedTransparentWidget(Rect box) {
	this->box = box;
}

TextFieldMenuItem::TextFieldMenuItem() {
	box.size = Vec(120, 20);
	multiline = false;
}
	
void TextFieldMenuItem::onAction(const event::Action& e) {
	TextField::onAction(e);
	getAncestorOfType<MenuOverlay>()->requestDelete();
}

// Module with intern params and Widget with screws

void BaseModule::configScrewParams() {
	for (auto i = 0; i < NUM_MAX_SCREWS; ++i) {
		configSwitch(SCREW_PARAM + i, 0, 5, 5, "Screw", {
			"I warn you, UNDO won't help you here!",
			"If you continue doing that, the module will fall out of the rack!",
			"Stop doing that!",
			"Don't pull it out!",
			"loose",
			"fixed"
		});
		getParamQuantity(SCREW_PARAM + i)->randomizeEnabled = false;
	}
}

ModuleWidgetWithScrews::ModuleWidgetWithScrews() {
	hasScrews = false;
}

void ModuleWidgetWithScrews::setPanel(const std::string& filename) {
	ModuleWidget::setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}

void ModuleWidgetWithScrews::setScrews(ScrewTopLeft topLeft, ScrewTopRight topRight, ScrewBottomLeft bottomLeft, ScrewBottomRight bottomRight) {
	if (topLeft) {
		addScrew(Vec(0, 0), BaseModule::SCREW_PARAM);
	} else if (module) {
		module->params[BaseModule::SCREW_PARAM].setValue(0);
	}
	if (topRight) {
		addScrew(Vec(box.size.x - RACK_GRID_WIDTH, 0), BaseModule::SCREW_PARAM + 1);
	} else if (module) {
		module->params[BaseModule::SCREW_PARAM + 1].setValue(0);
	}
	if (bottomLeft) {
		if (bottomLeft == SCREW_BOTTOM_LEFT_INDENTED) {
			addScrew(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), BaseModule::SCREW_PARAM + 2);
		} else {
			addScrew(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), BaseModule::SCREW_PARAM + 2);
		}
	} else if (module) {
		module->params[BaseModule::SCREW_PARAM + 2].setValue(0);
	}
	if (bottomRight) {
		addScrew(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), BaseModule::SCREW_PARAM + 3);
	} else if (module) {
		module->params[BaseModule::SCREW_PARAM + 3].setValue(0);
	}
}

void ModuleWidgetWithScrews::setWidthInHP(int hp) {
	setSize(Vec(hp * 15, 380));
}

void ModuleWidgetWithScrews::addScrew(Vec pos, int screwParam) {
	KnobScrewMountModule* screw = dynamic_cast<KnobScrewMountModule*>(createParam<KnobScrewMountModule>(pos, module, screwParam));
	screw->module = module;
	screw->param = screwParam;
	addParam(screw);
	hasScrews = true;
}

void ModuleWidgetWithScrews::step() {
	ModuleWidget::step();
	if (hasScrews && module) {
		for (auto i = 0; i < NUM_MAX_SCREWS; ++i) {
			if (module->params[BaseModule::SCREW_PARAM + i].getValue()) {
				return;
			}
		}
		removeAction();
	}
}
