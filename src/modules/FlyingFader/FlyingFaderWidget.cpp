#include "FlyingFaderWidget.hpp"

const FaderCapColor FlyingFaderWidget::FADER_CAP_COLORS[] = {
	{ 0, "white" },
	{ 1, "grey" },
	{ 2, "black" },
	{ 3, "red" },
	{ 4, "blue" },
	{ 5, "green" },
	{ 6, "brown" },
	{ 7, "orange" },
	{ 8, "pink" },
	{ 9, "purple" }
};

MotorizedFader::MotorizedFader() {
	displayContextMenu = false;
	// oldValue = -1;
	minHandlePos = Vec(0, 230);
	maxHandlePos = Vec(0, 2);
	setBackgroundSvg("res/knobs/MotorizedFaderBackground.svg");
	setHandleSvg("res/knobs/MotorizedFaderHandle_white.svg");
}

void MotorizedFader::onButton(const event::Button& e) {
	if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT && (e.mods & RACK_MOD_MASK) == 0) {
		if (flyingFader) {
			flyingFader->faderDragged = true;
			displayContextMenu = true;
			// oldValue = paramQuantity->getValue();
		}
	}
	ParamWidget::onButton(e);
	
}

void MotorizedFader::onDragStart(const event::DragStart& e) {
	if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
		Knob::onDragStart(e);
		// APP->window->cursorUnlock();
		if (flyingFader) {
			flyingFader->faderDragged = true;
		}
	}
}

void MotorizedFader::onDragEnd(const event::DragEnd& e) {
	if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
		if (flyingFader) {
			flyingFader->faderDragged = false;
		}
		Knob::onDragEnd(e);
	}
}

void MotorizedFader::step() {
	if (flyingFader && displayContextMenu) {
		MenuOverlay* overlay = NULL;
		for (Widget* child : APP->scene->children) {
			overlay = dynamic_cast<MenuOverlay*>(child);
			if (overlay) break;
		}
		if (!overlay) {
			flyingFader->faderDragged = false;
			displayContextMenu = false;
		}
	}
	SvgSlider::step();
}

FlyingFaderWidget::FlyingFaderWidget(FlyingFader* module) {
	setModule(module);
	setPanel("res/FlyingFader.svg");
	setSize(Vec(60, 380));
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, NO_SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);
	faderCapColorIndex = 0;

	fader = dynamic_cast<MotorizedFader*>(createParam<MotorizedFader>(Vec(18, 40.5), module, FlyingFader::FADER_PARAM));
	fader->flyingFader = module;
	addParam(fader);

	addInput(createInputCentered<InPort>(Vec(15.5, 357), module, FlyingFader::CV_INPUT));
	addInput(createInputCentered<InPort>(Vec(44.5, 349.5), module, FlyingFader::AUDIO_INPUT));
	
	addOutput(createOutputCentered<OutPort>(Vec(15.5, 330), module, FlyingFader::CV_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(44.5, 23), module, FlyingFader::AUDIO_OUTPUT));

	// addChild(createLightCentered<SmallLight<RedLight>>(Vec(11, 72), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<RedLight>>(Vec(11, 84.75), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<YellowLight>>(Vec(11, 96.8), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<YellowLight>>(Vec(11, 108.5), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<YellowLight>>(Vec(11, 118.75), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<YellowLight>>(Vec(11, 129.25), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<YellowLight>>(Vec(11, 138.25), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<YellowLight>>(Vec(11, 147.75), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<YellowLight>>(Vec(11, 156.25), module, FlyingFader::VU_LIGHT));
	// // addChild(createLightCentered<SmallLight<YellowLight>>(Vec(11, 164.5), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<YellowLight>>(Vec(11, 171.75), module, FlyingFader::VU_LIGHT));
	// // addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 179.25), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 185.75), module, FlyingFader::VU_LIGHT));
	// // addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 195.5), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 203.75), module, FlyingFader::VU_LIGHT));
	// // addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 212), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 219), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 232), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 242.75), module, FlyingFader::VU_LIGHT));
	// // addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 252), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 259.5), module, FlyingFader::VU_LIGHT));
	// // addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 271.25), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 279.5), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 289.75), module, FlyingFader::VU_LIGHT));
	// addChild(createLightCentered<SmallLight<GreenLight>>(Vec(11, 300), module, FlyingFader::VU_LIGHT));
	
	// // red 308
	// // yellow 283.2, 261.25, 241.5, 223.75, 208.25
	// // green 194.25

}

void FlyingFaderWidget::appendContextMenu(Menu* menu) {
	// TapeRecorder* tapeRecorder = dynamic_cast<TapeRecorder*>(this->module);
	menu->addChild(new MenuEntry);
	// menu->addChild(new TapeNameMenuItem(tapeNameDisplay));
	menu->addChild(new FaderCapColorMenuItem(this, faderCapColorIndex));
}

void FlyingFaderWidget::changeFaderCapColor(int faderCapColorIndex) {
	fader->setHandleSvg("res/knobs/MotorizedFaderHandle_" + FADER_CAP_COLORS[faderCapColorIndex].color + ".svg");
	event::Change eChange;
	fader->onChange(eChange);
	this->faderCapColorIndex = faderCapColorIndex;
}

json_t* FlyingFaderWidget::toJson() {
	json_t* rootJ = ModuleWidget::toJson();
	
	// json_object_set_new(rootJ, "fader-name", json_string(faderDisplay->text.c_str()));
	json_object_set_new(rootJ, "fader-cap-color", json_integer(faderCapColorIndex));
	return rootJ;
}

void FlyingFaderWidget::fromJson(json_t* rootJ) {
	ModuleWidget::fromJson(rootJ);
	
	// json_t* faderNameJ = json_object_get(rootJ, "fader-name");
	// if (faderNameJ) {
		// faderDisplay->text = json_string_value(faderNameJ);
	// }
	json_t* faderCapColorJ = json_object_get(rootJ, "fader-cap-color");
	if (faderCapColorJ) {
		changeFaderCapColor(json_integer_value(faderCapColorJ));
		// faderCapColorIndex = json_integer_value(faderCapColorJ);
	}
	// if (module) {
		// dynamic_cast<FlyingFader*>(this->module)->init();
	// }
}

FaderCapColorValueItem::FaderCapColorValueItem(FlyingFaderWidget* flyingFaderWidget, int faderCapColorIndex) {
	this->flyingFaderWidget = flyingFaderWidget;
	this->faderCapColorIndex = faderCapColorIndex;
	text = FlyingFaderWidget::FADER_CAP_COLORS[faderCapColorIndex].color;
	rightText = CHECKMARK(FlyingFaderWidget::FADER_CAP_COLORS[faderCapColorIndex].index == flyingFaderWidget->faderCapColorIndex);
}

void FaderCapColorValueItem::onAction(const event::Action& e) {
	flyingFaderWidget->changeFaderCapColor(faderCapColorIndex);
}

FaderCapColorMenuItem::FaderCapColorMenuItem(FlyingFaderWidget* flyingFaderWidget, int faderCapColorIndex) {
	this->flyingFaderWidget = flyingFaderWidget;
	this->faderCapColorIndex = faderCapColorIndex;
	text = "Fader Cap Color";
	rightText = FlyingFaderWidget::FADER_CAP_COLORS[faderCapColorIndex].color + " " + RIGHT_ARROW;
}

Menu* FaderCapColorMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	for (auto i = 0; i < FlyingFaderWidget::NUM_FADER_CAP_COLORS; ++i) {
		menu->addChild(new FaderCapColorValueItem(flyingFaderWidget, i));
	}
	return menu;
}

Model* modelFlyingFader = createModel<FlyingFader, FlyingFaderWidget>("FlyingFader");
