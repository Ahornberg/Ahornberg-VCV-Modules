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

// Module and Widget with Screws

std::string ScrewMessage::getDisplayValueString() {
	return SCREW_MESSAGES[(int) getValue()];
}

void ModuleWithScrews::configScrewParams() {
	for (auto i = 0; i < NUM_MAX_SCREWS; ++i) {
		configParam<ScrewMessage>(SCREW_PARAM + i, 0, 5, 5, "Screw");
	}
}

void ModuleWidgetWithScrews::setPanel(const std::string& filename) {
	ModuleWidget::setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}

void ModuleWidgetWithScrews::setScrews(ScrewTopLeft topLeft, ScrewTopRight topRight, ScrewBottomLeft bottomLeft, ScrewBottomRight bottomRight) {
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
		if (bottomLeft == SCREW_BOTTOM_LEFT_INDENTED) {
			addScrew(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), ModuleWithScrews::SCREW_PARAM + 2);
		} else {
			addScrew(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), ModuleWithScrews::SCREW_PARAM + 2);
		}
	} else if (module) {
		module->params[ModuleWithScrews::SCREW_PARAM + 2].setValue(0);
	}
	if (bottomRight) {
		addScrew(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), ModuleWithScrews::SCREW_PARAM + 3);
	} else if (module) {
		module->params[ModuleWithScrews::SCREW_PARAM + 3].setValue(0);
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
}

void ModuleWidgetWithScrews::step() {
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
