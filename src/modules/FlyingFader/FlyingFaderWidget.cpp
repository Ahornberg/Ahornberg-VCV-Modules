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
	minHandlePos = Vec(0, 230);
	maxHandlePos = Vec(0, 2);
	setBackgroundSvg("res/knobs/MotorizedFaderBackground.svg");
	fb->dirty = true;
	faderCapColorIndex = 0;
	faderCapColorIndex = 0;
}

void MotorizedFader::setFlyingFader(FlyingFader* flyingFader) {
	this->flyingFader = flyingFader;
	if (flyingFader) {
		setHandleSvg("res/knobs/MotorizedFaderHandle_" + FlyingFaderWidget::FADER_CAP_COLORS[flyingFader->faderCapColorIndex].color + ".svg");
		faderCapColorIndex = flyingFader->faderCapColorIndex;
	} else {
		setHandleSvg("res/knobs/MotorizedFaderHandle_" + FlyingFaderWidget::FADER_CAP_COLORS[(int) (random::uniform() * FlyingFaderWidget::NUM_FADER_CAP_COLORS)].color + ".svg");
		handle->box.pos = Vec(0, 68.5);
	}
	fb->dirty = true;
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
		APP->window->cursorLock();
		ParamQuantity* paramQuantity = getParamQuantity();
		if (paramQuantity && flyingFader) {
			oldValue = paramQuantity->getSmoothValue();
			oldFaderValueBeforeConnected = flyingFader->params[FlyingFader::FADER_VALUE_BEFORE_CONNECTED].getValue();
			flyingFader->faderDragged = true;
		}
	}
}

void MotorizedFader::onDragEnd(const event::DragEnd& e) {
	if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
		ParamQuantity* paramQuantity = getParamQuantity();
		if (paramQuantity && flyingFader) {
			flyingFader->faderDragged = false;
			float newValue = paramQuantity->getSmoothValue();
			float newFaderValueBeforeConnected = flyingFader->params[FlyingFader::FADER_VALUE_BEFORE_CONNECTED].getValue();
			if (oldValue != newValue) {
				// Push ParamChange history action
				history::ComplexAction* complexAction = new history::ComplexAction;
				complexAction->name = "move slider";
				history::ParamChange* oldFaderValue = new history::ParamChange;
				oldFaderValue->name = "move slider";
				oldFaderValue->moduleId = paramQuantity->module->id;
				oldFaderValue->paramId = FlyingFader::FADER_VALUE_BEFORE_CONNECTED;
				oldFaderValue->oldValue = oldFaderValueBeforeConnected;
				oldFaderValue->newValue = newFaderValueBeforeConnected;
				complexAction->push(oldFaderValue);
				history::ParamChange* faderMove = new history::ParamChange;
				faderMove->name = "move slider";
				faderMove->moduleId = paramQuantity->module->id;
				faderMove->paramId = paramQuantity->paramId;
				faderMove->oldValue = oldValue;
				faderMove->newValue = newValue;
				complexAction->push(faderMove);
				APP->history->push(complexAction);
			}
		}
		APP->window->cursorUnlock();
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
	if (flyingFader && flyingFader->faderCapColorIndex != faderCapColorIndex) {
		setHandleSvg("res/knobs/MotorizedFaderHandle_" + FlyingFaderWidget::FADER_CAP_COLORS[flyingFader->faderCapColorIndex].color + ".svg");
		faderCapColorIndex = flyingFader->faderCapColorIndex;
	}
	SvgSlider::step();
}

void TextOnFaderModule::drawText(const Widget::DrawArgs& disp, Rect box) {
	std::shared_ptr<Font> font = APP->window->loadFont(fontPath);
	if (font && font->handle >= 0) {
		nvgBeginPath(disp.vg);
		nvgRotate(disp.vg, (-90 * NVG_PI) / 180);
		if (useScissor) {
			nvgScissor(disp.vg, box.size.x * -0.5, 0, box.size.x, box.size.y);
		}
		nvgFontSize(disp.vg, fontSize);
		nvgFontFaceId(disp.vg, font->handle);
		nvgFillColor(disp.vg, textColor);
		nvgTextAlign(disp.vg, textAlign);
		if (flyingFader) {
			text = flyingFader->faderName;
		} else {
			text = FlyingFader::INIT_FADER_NAME;
		}
		nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
	}
}

FaderNameDisplay::FaderNameDisplay(FlyingFader* flyingFader, Rect box) : SizedTransparentWidget(box) {
	this->flyingFader = flyingFader;
	fontPath = asset::plugin(pluginInstance, FONT_HANDWRITE);
	textColor = COLOR_BLACK;
	//text = FlyingFader::INIT_FADER_NAME;
	fontSize = 16;
	textAlign = NVG_ALIGN_CENTER;
	useScissor = true;
	textPos =  Vec(0, 11);
}

void FaderNameDisplay::draw(const DrawArgs& disp) {
	drawText(disp, box);
}

FaderNameMenuItem::FaderNameMenuItem(FlyingFader* flyingFader) {
	this->flyingFader = flyingFader;
	if (flyingFader) {
		text = flyingFader->faderName;
	} else {
		text = FlyingFader::INIT_FADER_NAME;
	}
}

void FaderNameMenuItem::onChange(const event::Change& e) {
	TextFieldMenuItem::onChange(e);
	if (flyingFader) {
		flyingFader->faderName = text;
	}
}

FlyingFaderWidget::FlyingFaderWidget(FlyingFader* module) {
	setModule(module);
	setPanel("res/FlyingFader.svg");
	setWidthInHP(4);
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, NO_SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);
	//faderCapColorIndex = 0;

	faderNameDisplay = new FaderNameDisplay(module, Rect(6.5, 214.5, 153, 18));
	addChild(faderNameDisplay);

	fader = dynamic_cast<MotorizedFader*>(createParam<MotorizedFader>(Vec(18, 40.5), module, FlyingFader::FADER_PARAM));
	fader->setFlyingFader(module);
	addParam(fader);

	addInput(createInputCentered<InPort>(Vec(15.5, 357), module, FlyingFader::CV_INPUT));
	addInput(createInputCentered<InPort>(Vec(44.5, 349.5), module, FlyingFader::AUDIO_INPUT));
	
	addOutput(createOutputCentered<OutPort>(Vec(15.5, 330), module, FlyingFader::CV_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(44.5, 23), module, FlyingFader::AUDIO_OUTPUT));
}

void FlyingFaderWidget::appendContextMenu(Menu* menu) {
	FlyingFader* flyingFader = dynamic_cast<FlyingFader*>(this->module);
	menu->addChild(new MenuSeparator);
	menu->addChild(new FaderNameMenuItem(flyingFader));
	menu->addChild(new FaderCapColorMenuItem(this, flyingFader->faderCapColorIndex));
	menu->addChild(new AudioPolyModeMenuItem(flyingFader));
	menu->addChild(new CvScaleModeMenuItem(flyingFader));
}

void FlyingFaderWidget::changeFaderCapColor(int faderCapColorIndex) {
	fader->setHandleSvg("res/knobs/MotorizedFaderHandle_" + FADER_CAP_COLORS[faderCapColorIndex].color + ".svg");
	event::Change eChange;
	fader->onChange(eChange);
	//this->faderCapColorIndex = faderCapColorIndex;
	if (module) {
		dynamic_cast<FlyingFader*>(module)->faderCapColorIndex = faderCapColorIndex;
	}
}

void FlyingFaderWidget::step() {
	FlyingFader* flyingFader = dynamic_cast<FlyingFader*>(this->module);
	if (flyingFader && flyingFader->faderCapColorIndex != fader->faderCapColorIndex) {
		changeFaderCapColor(flyingFader->faderCapColorIndex);
	}
	ModuleWidgetWithScrews::step();
}

FaderCapColorValueItem::FaderCapColorValueItem(FlyingFaderWidget* flyingFaderWidget, int faderCapColorIndex) {
	this->flyingFaderWidget = flyingFaderWidget;
	this->faderCapColorIndex = faderCapColorIndex;
	text = FlyingFaderWidget::FADER_CAP_COLORS[faderCapColorIndex].color;
	if (flyingFaderWidget->module) {
		FlyingFader* flyingFader = dynamic_cast<FlyingFader*>(flyingFaderWidget->module);
		rightText = CHECKMARK(FlyingFaderWidget::FADER_CAP_COLORS[faderCapColorIndex].index == flyingFader->faderCapColorIndex);
	} else {
		rightText = CHECKMARK(FlyingFaderWidget::FADER_CAP_COLORS[faderCapColorIndex].index == 0);
	}
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

AudioPolyModeValueItem::AudioPolyModeValueItem(FlyingFader* flyingFader, bool audioPolyMode) {
	this->flyingFader = flyingFader;
	this->audioPolyMode = audioPolyMode;
	if (audioPolyMode) {
		text = "Poly In - Mono Out";
	} else {
		text = "Poly In - Poly Out";
	}
	if (flyingFader) {
		rightText = CHECKMARK(audioPolyMode == flyingFader->params[FlyingFader::AUDIO_POLY_MODE].getValue());
	}
}

void AudioPolyModeValueItem::onAction(const event::Action& e) {
	if (flyingFader) {
		flyingFader->params[FlyingFader::AUDIO_POLY_MODE].setValue(audioPolyMode);
	}
}

AudioPolyModeMenuItem::AudioPolyModeMenuItem(FlyingFader* flyingFader) {
	this->flyingFader = flyingFader;
	text = "Audio Polyphony Mode";
	if (flyingFader) {
		if (flyingFader->params[FlyingFader::AUDIO_POLY_MODE].getValue()) {
			rightText = "Poly In - Mono Out ";
		} else {
			rightText = "Poly In - Poly Out ";
		}
		rightText += RIGHT_ARROW;
	}
}

Menu* AudioPolyModeMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	menu->addChild(new AudioPolyModeValueItem(flyingFader, 0));
	menu->addChild(new AudioPolyModeValueItem(flyingFader, 1));
	return menu;
}

CvScaleModeValueItem::CvScaleModeValueItem(FlyingFader* flyingFader, bool cvScaleMode) {
	this->flyingFader = flyingFader;
	this->cvScaleMode = cvScaleMode;
	if (cvScaleMode) {
		text = "MindMeld MixMaster";
	} else {
		text = "VCV Standard";
	}
	if (flyingFader) {
		rightText = CHECKMARK(cvScaleMode == flyingFader->params[FlyingFader::CV_SCALE_MODE].getValue());
	}
}

void CvScaleModeValueItem::onAction(const event::Action& e) {
	if (flyingFader) {
		flyingFader->params[FlyingFader::CV_SCALE_MODE].setValue(cvScaleMode);
	}
}

CvScaleModeMenuItem::CvScaleModeMenuItem(FlyingFader* flyingFader) {
	this->flyingFader = flyingFader;
	text = "CV-Scale Mode";
	if (flyingFader) {
		if (flyingFader->params[FlyingFader::CV_SCALE_MODE].getValue()) {
			rightText = "MindMeld MixMaster ";
		} else {
			rightText = "VCV Standard ";
		}
		rightText += RIGHT_ARROW;
	}
}

Menu* CvScaleModeMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	menu->addChild(new CvScaleModeValueItem(flyingFader, 0));
	menu->addChild(new CvScaleModeValueItem(flyingFader, 1));
	return menu;
}

Model* modelFlyingFader = createModel<FlyingFader, FlyingFaderWidget>("FlyingFader");
