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
	minHandlePos = Vec(0, 230);
	maxHandlePos = Vec(0, 2);
	setBackgroundSvg("res/knobs/MotorizedFaderBackground.svg");
	setHandleSvg("res/knobs/MotorizedFaderHandle_white.svg");
}

void MotorizedFader::onDragStart(const event::DragStart& e) {
	if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
		Knob::onDragStart(e);
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

FlyingFaderWidget::FlyingFaderWidget(FlyingFader* module) {
	setModule(module);
	setPanel("res/FlyingFader.svg");
	setSize(Vec(60, 380));
	faderCapColorIndex = 0;

	fader = dynamic_cast<MotorizedFader*>(createParam<MotorizedFader>(Vec(18, 50), module, FlyingFader::FADER_PARAM));
	fader->flyingFader = module;
	addParam(fader);

	addInput(createInputCentered<InPort>(Vec(15, 350), module, FlyingFader::AUDIO_INPUT));
	addInput(createInputCentered<InPort>(Vec(45, 350), module, FlyingFader::CV_INPUT));
	
	addOutput(createOutputCentered<OutPort>(Vec(15, 30), module, FlyingFader::AUDIO_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(45, 30), module, FlyingFader::CV_OUTPUT));
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
