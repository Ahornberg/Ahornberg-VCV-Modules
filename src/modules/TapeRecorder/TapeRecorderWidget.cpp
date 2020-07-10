#include "TapeRecorderWidget.hpp"

const Stripe StripeWidget::STRIPES[] = {
	{ "res/stripes/1970.svg",    "1970" },
	{ "res/stripes/Red.svg",     "Red" },
	{ "res/stripes/Green.svg",   "Green" },
	{ "res/stripes/Purple.svg",  "Purple" },
	{ "res/stripes/Brown.svg",   "Brown" },
	{ "res/stripes/Rainbow.svg", "Rainbow" },
	{ "res/stripes/Stars.svg",   "Stars" }
};

StripeWidget::StripeWidget(Vec pos) {
	box.pos = pos;
	setStripe((int) (random::uniform() * NUM_STRIPES));
}

void StripeWidget::setStripe(int stripe) {
	this->stripe = stripe;
	setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, STRIPES[stripe].fileName)));
}

// Switches *******************************************************************

void TransportCueSwitch::step() {
	if (tapeRecorder) {
		momentary = tapeRecorder->isTransportCueSwitchMomentary();
	}
	TransportSwitch::step();
}

CueBackwardsSwitch::CueBackwardsSwitch() {
	addFrame("res/switches/CueBackwards_off.svg");
	addFrame("res/switches/CueBackwards_on.svg");
}

void CueBackwardsSwitch::onChange(const event::Change& e) {
	TransportCueSwitch::onChange(e);
	if (tapeRecorder && tapeRecorder->params[TapeRecorder::CUE_BACKWARDS_PARAM].getValue()) {
		tapeRecorder->params[TapeRecorder::CUE_FORWARDS_PARAM].setValue(0);
	}
}

CueForwardsSwitch::CueForwardsSwitch() {
	addFrame("res/switches/CueForwards_off.svg");
	addFrame("res/switches/CueForwards_on.svg");
}

void CueForwardsSwitch::onChange(const event::Change& e) {
	TransportCueSwitch::onChange(e);
	if (tapeRecorder && tapeRecorder->params[TapeRecorder::CUE_FORWARDS_PARAM].getValue()) {
		tapeRecorder->params[TapeRecorder::CUE_BACKWARDS_PARAM].setValue(0);
	}
}

PlayBackwardsSwitch::PlayBackwardsSwitch() {
	addFrame("res/switches/PlayBackwards_off.svg");
	addFrame("res/switches/PlayBackwards_on.svg");
}

void PlayBackwardsSwitch::onChange(const event::Change& e) {
	TransportSwitch::onChange(e);
	if (tapeRecorder && tapeRecorder->params[TapeRecorder::PLAY_BACKWARDS_PARAM].getValue()) {
		tapeRecorder->params[TapeRecorder::PLAY_FORWARDS_PARAM].setValue(0);
	}
}

PlayForwardsSwitch::PlayForwardsSwitch() {
	addFrame("res/switches/PlayForwards_off.svg");
	addFrame("res/switches/PlayForwards_on.svg");
}

void PlayForwardsSwitch::onChange(const event::Change& e) {
	TransportSwitch::onChange(e);
	if (tapeRecorder && tapeRecorder->params[TapeRecorder::PLAY_FORWARDS_PARAM].getValue()) {
		tapeRecorder->params[TapeRecorder::PLAY_BACKWARDS_PARAM].setValue(0);
	}
}

// Knobs **********************************************************************
	
KnobWheel::KnobWheel() {
	setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/Wheel.svg")));
	minAngle = -6.f * M_PI;
	maxAngle = 6.f * M_PI;
	shadow->opacity = 0.f;
	shadow->box.pos = Vec(0, 0);
	speed = 0.1f;
	cursorHand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
}

float KnobWheel::distance(Vec* p1, Vec* p2) {
	return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}

float KnobWheel::calcTouchedWheelForce(float distance, float maxDistance) {
	if (distance >= maxDistance) {
		glfwSetCursor(APP->window->win, NULL);
		return 1;
	}
	glfwSetCursor(APP->window->win, cursorHand);
	return (maxDistance - distance) / maxDistance;
}

void KnobWheel::onButton(const event::Button& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT && e.action == GLFW_PRESS && module->speed != 0.f) {
		mousePos = new Vec(e.pos.x, e.pos.y);
		Vec* center = new Vec(box.size.x * 0.5f, box.size.y * 0.5f);
		module->touchedWheelForce = calcTouchedWheelForce(distance(mousePos, center), center->x);
		// glfwSetCursor(APP->window->win, cursorHand);
	}
	SvgKnob::onButton(e);
}

void KnobWheel::onDragHover(const event::DragHover& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT && module->speed != 0.f) {
		mousePos = new Vec(e.pos.x, e.pos.y);
		Vec* center = new Vec(box.size.x * 0.5f, box.size.y * 0.5f);
		module->touchedWheelForce = calcTouchedWheelForce(distance(mousePos, center), center->x);
		// glfwSetCursor(APP->window->win, cursorHand);
	}
	SvgKnob::onDragHover(e);
}

void KnobWheel::onDragStart(const event::DragStart& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT && module->speed != 0.f) {
		// glfwSetCursor(APP->window->win, cursorHand);
		return;
	}
	SvgKnob::onDragStart(e);
}

void KnobWheel::onDragEnd(const event::DragEnd& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT && module->speed != 0.f) {
		module->touchedWheelForce = 1;
		glfwSetCursor(APP->window->win, NULL);
		return;
	}
	SvgKnob::onDragEnd(e);
}

void KnobWheel::onDragLeave(const event::DragLeave& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT && module->speed != 0.f) {
		module->touchedWheelForce = 1;
		// glfwSetCursor(APP->window->win, NULL);
		return;
	}
	SvgKnob::onDragLeave(e);
}

void KnobWheel::onDragMove(const event::DragMove& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT && module->speed != 0.f) {
		mousePos->x += e.mouseDelta.x / exp2(settings::zoom);
		mousePos->y += e.mouseDelta.y / exp2(settings::zoom);
		Vec* center = new Vec(box.size.x * 0.5f, box.size.y * 0.5f);
		module->touchedWheelForce = calcTouchedWheelForce(distance(mousePos, center), center->x);
		// glfwSetCursor(APP->window->win, cursorHand);
		return;
	}
	SvgKnob::onDragMove(e);
}

// Displays *******************************************************************

TapePositionDisplay::TapePositionDisplay(Rect box, TapeRecorder* tapeRecorder) : Display(box) {
	this->tapeRecorder = tapeRecorder;
	tapePosition = 0;
	beatsPerBar = 0;
	loopStart = 0;
	loopEnd = 0;
	loopStartConnected = false;
	loopEndConnected = false;
	loopStartOnTapePosition = false;
	loopEndOnTapePosition = false;
	playStatus = false;
	cueStatus = false;
	playForwardStatus = false;
	cueForwardStatus = false;
}

void TapePositionDisplay::drawText(const DrawArgs &disp) {
	if (tapeRecorder) {
		loopStartConnected = tapeRecorder->inputs[TapeRecorder::LOOP_INPUT].isConnected();
		loopStart = tapeRecorder->loopStart;
		loopEndConnected = (tapeRecorder->inputs[TapeRecorder::LOOP_INPUT].isConnected() && tapeRecorder->inputs[TapeRecorder::LOOP_INPUT].getChannels() > 1);
		loopEnd = tapeRecorder->loopEnd;
		beatsPerBar = tapeRecorder->params[TapeRecorder::BEATS_PER_BAR_PARAM].getValue();
		tapePosition = tapeRecorder->tapePosition;
		playForwardStatus = tapeRecorder->playForwardStatus;
		playStatus = tapeRecorder->playStatus;
		cueForwardStatus = tapeRecorder->cueForwardStatus;
		cueStatus = tapeRecorder->cueStatus;
		loopStartOnTapePosition = (tapeRecorder->params[TapeRecorder::LOOP_START_BUTTON_PARAM].getValue() == 1.0f);
		loopEndOnTapePosition = (tapeRecorder->params[TapeRecorder::LOOP_END_BUTTON_PARAM].getValue() == 1.0f);
	}
	Vec textPos = Vec(4, 14);
	nvgFillColor(disp.vg, textColorLight);
	int bar = (int) tapePosition;
	int beat = (int) ((tapePosition - bar) * beatsPerBar);
	if (bar > 9999) {
		bar = 9999;
	}
	std::string text = std::to_string(bar);
	if (bar < 1000) {
		text = " " + text;
	} 
	if (bar < 100) {
		text = " " + text;
	} 
	if (bar < 10) {
		text = " " + text;
	}
	if (beat < 10) {
		text += "/ " + std::to_string(beat);
	} else {
		text += "/" + std::to_string(beat);
	}
	nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
	
	if (loopStart != loopEnd) {
		textPos = Vec(33, 28);
		nvgText(disp.vg, textPos.x, textPos.y, "L", NULL);
	}
	// if (cueStatus) {
		// if (cueForwardStatus) {
			// textPos = Vec(33, 28);
			// nvgText(disp.vg, textPos.x, textPos.y, ")", NULL);
			// textPos = Vec(35.5, 28);
			// nvgText(disp.vg, textPos.x, textPos.y, ")", NULL);
		// } else {
			// textPos = Vec(33, 28);
			// nvgText(disp.vg, textPos.x, textPos.y, "(", NULL);
			// textPos = Vec(35.5, 28);
			// nvgText(disp.vg, textPos.x, textPos.y, "(", NULL);
		// }
	// } else if (playStatus) {
		// // if ((playForwardStatus && position < loopEnd) || (!playForwardStatus && position < loopStart)) {
		// if (playForwardStatus) {
			// textPos = Vec(33, 28);
			// nvgText(disp.vg, textPos.x, textPos.y, "-", NULL);
			// textPos = Vec(35.5, 28);
			// nvgText(disp.vg, textPos.x, textPos.y, ")", NULL);
		// } else {
			// textPos = Vec(33, 28);
			// nvgText(disp.vg, textPos.x, textPos.y, "(", NULL);
			// textPos = Vec(35.5, 28);
			// nvgText(disp.vg, textPos.x, textPos.y, "-", NULL);
		// }
	// }
	
	// textPos = Vec(34, 14);
	// if (speedConnected) {
		// nvgFillColor(disp.vg, textColorDark);
	// } else {
		// nvgFillColor(disp.vg, textColorLight);
	// }
	// nvgText(disp.vg, textPos.x, textPos.y, (std::to_string(tempo)).c_str(), NULL);
	
	nvgFontSize(disp.vg, 8);

	textPos = Vec(4, 28);
	if (loopStartOnTapePosition) {
		nvgFillColor(disp.vg, textColorRed);
	} else if (loopStartConnected) {
		nvgFillColor(disp.vg, textColorDark);
	} else {
		nvgFillColor(disp.vg, textColorLight);
	}
	text = std::to_string(loopStart);
	if (loopStart < 100) {
		text = " " + text;
	} 
	if (loopStart < 10) {
		text = " " + text;
	}
	nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
	
	textPos = Vec(46, 28);
	if (loopEndOnTapePosition) {
		nvgFillColor(disp.vg, textColorRed);
	} else if (loopEndConnected) {
		nvgFillColor(disp.vg, textColorDark);
	} else {
		nvgFillColor(disp.vg, textColorLight);
	}
	text = std::to_string(loopEnd);
	if (loopEnd < 100) {
		text = " " + text;
	} 
	if (loopEnd < 10) {
		text = " " + text;
	}
	nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
}
	
ModuleLinkedWidget::ModuleLinkedWidget(Rect box, TapeRecorder* tapeRecorder) : SizedTransparentWidget(box) {
	this->tapeRecorder = tapeRecorder;
}

TapeDisplay::TapeDisplay(Rect box, TapeRecorder* tapeRecorder) : ModuleLinkedWidget(box, tapeRecorder) {
	tapeColor = COLOR_BROWN;
	tapeOnLeftWheel = 0;
	tapeOnRightWheel = 1;
}

void TapeDisplay::draw(const DrawArgs &disp) {
	if (tapeRecorder) {
		tapeOnLeftWheel = tapeRecorder->tapeOnLeftWheel;
		tapeOnRightWheel = tapeRecorder->tapeOnRightWheel;
	}
	float radiusLeft = (RADIUS_MAX - RADIUS_MIN) * tapeOnLeftWheel + RADIUS_MIN;
	float radiusRight = (RADIUS_MAX - RADIUS_MIN) * tapeOnRightWheel + RADIUS_MIN;
	nvgFillColor(disp.vg, tapeColor);
	nvgScissor(disp.vg, 0, 0, box.size.x, box.size.y);
	nvgBeginPath(disp.vg);
	nvgCircle(disp.vg, box.size.x / 2.f, -21, radiusRight);
	nvgClosePath(disp.vg);
	nvgFill(disp.vg);
	nvgBeginPath(disp.vg);
	nvgCircle(disp.vg, box.size.x / 2.f, box.size.y + 21, radiusLeft);
	nvgClosePath(disp.vg);
	nvgFill(disp.vg);
}

void TextOnCassette::drawText(const Widget::DrawArgs &disp, Rect box) {
	nvgBeginPath(disp.vg);
	nvgRotate(disp.vg, (-90 * NVG_PI) / 180);
	if (useScissor) {
		nvgScissor(disp.vg, box.size.x * -0.5, 0, box.size.x, box.size.y);
	}
	nvgFontSize(disp.vg, fontSize);
	nvgFontFaceId(disp.vg, font->handle);
	nvgFillColor(disp.vg, textColor);
	nvgTextAlign(disp.vg, textAlign);
	nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
}

TapeLengthDisplay::TapeLengthDisplay(Rect box, TapeRecorder* tapeRecorder) : ModuleLinkedWidget(box, tapeRecorder) {
	font = APP->window->loadFont(asset::plugin(pluginInstance, FONT_SERIF_BOLD));
	textColor = COLOR_BLACK;
	fontSize = 18;
	textAlign = NVG_ALIGN_RIGHT;
	useScissor = false;
	textPos =  Vec(4, 14);
}

void TapeLengthDisplay::draw(const DrawArgs &disp) {
	if (tapeRecorder) {
		text = TapeRecorder::TAPE_LENGTHS[(int) tapeRecorder->params[TapeRecorder::TAPE_LENGTH_PARAM].getValue()].name;
	}
	drawText(disp, box);
}

std::string trackCountText(int trackCount) {
	if (trackCount == 1) {
		return "Mono";
	}
	if (trackCount == 2) {
		return "Stereo";
	}
	return string::f("%d Tracks", trackCount);
}

TrackCountDisplay::TrackCountDisplay(Rect box, TapeRecorder* tapeRecorder) : ModuleLinkedWidget(box, tapeRecorder) {
	font = APP->window->loadFont(asset::plugin(pluginInstance, FONT_SERIF_BOLD));
	textColor = COLOR_BLACK;
	fontSize = 14;
	textAlign = NVG_ALIGN_LEFT;
	useScissor = false;
	textPos =  Vec(4, 14);
}

void TrackCountDisplay::draw(const DrawArgs &disp) {
	if (tapeRecorder) {
		text = trackCountText(tapeRecorder->params[TapeRecorder::TRACK_COUNT_PARAM].getValue());
	}
	drawText(disp, box);
}

TapeNameDisplay::TapeNameDisplay(Rect box) : SizedTransparentWidget(box) {
	font = APP->window->loadFont(asset::plugin(pluginInstance, FONT_HANDWRITE));
	textColor = COLOR_BLACK;
	text = TapeRecorder::INIT_TAPE_NAME;
	fontSize = 16;
	textAlign = NVG_ALIGN_CENTER;
	useScissor = true;
	textPos =  Vec(0, 11);
}

void TapeNameDisplay::draw(const DrawArgs &disp) {
	drawText(disp, box);
}

// Menu ***********************************************************************
	
TapeRecorderMenuItem::TapeRecorderMenuItem(TapeRecorder* tapeRecorder) {
	this->tapeRecorder = tapeRecorder;
}

TapeNameMenuItem::TapeNameMenuItem(TapeNameDisplay* tapeNameDisplay) {
	this->tapeNameDisplay = tapeNameDisplay;
	text = tapeNameDisplay->text;
}

void TapeNameMenuItem::onChange(const event::Change& e) {
	TextFieldMenuItem::onChange(e);
	tapeNameDisplay->text = text;
}

OldSchoolModeMenuItem::OldSchoolModeMenuItem(TapeRecorder* tapeRecorder) : TapeRecorderMenuItem(tapeRecorder) {
	text = "Old School Mode";
	if (tapeRecorder) {
		rightText = CHECKMARK(tapeRecorder->params[TapeRecorder::OLD_SCHOOL_MODE_PARAM].getValue());
	}
}

void OldSchoolModeMenuItem::onAction(const event::Action& e) {
	if (tapeRecorder) {
		tapeRecorder->setOldSchoolMode(!tapeRecorder->params[TapeRecorder::OLD_SCHOOL_MODE_PARAM].getValue());
	}
}

TrackCountValueItem::TrackCountValueItem(TapeRecorder* tapeRecorder, int trackCount, std::string trackCountText) : TapeRecorderMenuItem(tapeRecorder) {
	this->trackCount = trackCount;
	text = trackCountText;
	rightText = CHECKMARK(tapeRecorder->params[TapeRecorder::TRACK_COUNT_PARAM].getValue() == trackCount);
}

void TrackCountValueItem::onAction(const event::Action& e) {
	if (tapeRecorder) {
		tapeRecorder->setTrackCount(trackCount);
	}
}

TrackCountMenuItem::TrackCountMenuItem(TapeRecorder* tapeRecorder) : TapeRecorderMenuItem(tapeRecorder) {
	text = "Tape Layout";
	if (tapeRecorder) {
		rightText = trackCountText(tapeRecorder->params[TapeRecorder::TRACK_COUNT_PARAM].getValue()) + " " + RIGHT_ARROW;
	}
}

Menu* TrackCountMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	for (auto trackCount = 1; trackCount <= 4; ++trackCount) {
		if (trackCount == 3) {
			continue;
		}
		menu->addChild(new TrackCountValueItem(tapeRecorder, trackCount, trackCountText(trackCount)));
	}
	return menu;
}

TapeLengthValueItem::TapeLengthValueItem(TapeRecorder* tapeRecorder, int tapeLength) : TapeRecorderMenuItem(tapeRecorder) {
	this->tapeRecorder = tapeRecorder;
	this->tapeLength = tapeLength;
	text = TapeRecorder::TAPE_LENGTHS[tapeLength].name;
	if (tapeRecorder) {
		rightText = CHECKMARK((sizeof(*tapeRecorder->audioBuffer) / sizeof(float)) == TapeRecorder::TAPE_LENGTHS[tapeLength].value);
	}
}

void TapeLengthValueItem::onAction(const event::Action& e) {
	if (tapeRecorder) {
		tapeRecorder->setTapeLength(tapeLength);
	}
}

TapeLengthMenuItem::TapeLengthMenuItem(TapeRecorder* tapeRecorder) : TapeRecorderMenuItem(tapeRecorder) {
	text = "Tape Length";
	if (tapeRecorder) {
		rightText = TapeRecorder::TAPE_LENGTHS[(int) tapeRecorder->params[TapeRecorder::TAPE_LENGTH_PARAM].getValue()].name + " " + RIGHT_ARROW;
	}
}

Menu* TapeLengthMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	for (auto i = 0; i < TapeRecorder::NUM_TAPE_LENGTHS; ++i) {
		menu->addChild(new TapeLengthValueItem(tapeRecorder, i));
	}
	return menu;
}

TapeStripesValueItem::TapeStripesValueItem(StripeWidget* stripeWidget, int stripe) {
	this->stripeWidget = stripeWidget;
	this->stripe = stripe;
	text = StripeWidget::STRIPES[stripe].name;
	rightText = CHECKMARK(stripeWidget->stripe == stripe);
}

void TapeStripesValueItem::onAction(const event::Action& e) {
	stripeWidget->setStripe(stripe);
}

TapeStripesMenuItem::TapeStripesMenuItem(StripeWidget* stripeWidget) {
	this->stripeWidget = stripeWidget;
	text = "Tape Color";
	rightText = StripeWidget::STRIPES[stripeWidget->stripe].name + " " + RIGHT_ARROW;
}

Menu* TapeStripesMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	for (auto i = 0; i < StripeWidget::NUM_STRIPES; ++i) {
		menu->addChild(new TapeStripesValueItem(stripeWidget, i));
	}
	return menu;
}

// Widget *********************************************************************
	
TapeRecorderWidget::TapeRecorderWidget(TapeRecorder* module) {
	setModule(module);
	setPanel("res/TapeRecorder.svg");
	setSize(Vec(120, 380));
	setScrews(true, true, false, true);

	addParam(createParam<KnobBig>(Vec(      43,  42),   module, TapeRecorder::TEMPO_PARAM));
	addParam(createParam<KnobSmallSnap>(Vec(10,  50),   module, TapeRecorder::BEATS_PER_BAR_PARAM));
	addParam(createParam<KnobSmallSnap>(Vec(10, 118),   module, TapeRecorder::LOOP_START_PARAM));
	addParam(createParam<KnobSmallSnap>(Vec(46, 118),   module, TapeRecorder::LOOP_END_PARAM));
	addParam(createParam<KnobScrew>(Vec(    42, 334.5), module, TapeRecorder::CUE_SPEED_PARAM));
	addParam(createParam<KnobScrew>(Vec(    61, 330),   module, TapeRecorder::CUE_SLEW_PARAM));
	
	KnobWheel* rightWheel = createParam<KnobWheel>(Vec(32, 178), module, TapeRecorder::WHEEL_RIGHT_PARAM);
	rightWheel->module = module;
	// rightWheel->paramId = TapeRecorder::WHEEL_RIGHT_PARAM;
	addParam(rightWheel);
	
	KnobWheel* leftWheel = createParam<KnobWheel>(Vec(32, 244), module, TapeRecorder::WHEEL_LEFT_PARAM);
	leftWheel->module = module;
	// leftWheel->paramId = TapeRecorder::WHEEL_LEFT_PARAM;
	addParam(leftWheel);
	
	CueBackwardsSwitch* cueBackwardsSwitch = dynamic_cast<CueBackwardsSwitch*>(createParam<CueBackwardsSwitch>(Vec(94, 179.5), module, TapeRecorder::CUE_BACKWARDS_PARAM));
	cueBackwardsSwitch->tapeRecorder = module;
	addParam(cueBackwardsSwitch);
	
	PlayBackwardsSwitch* playBackwardsSwitch = dynamic_cast<PlayBackwardsSwitch*>(createParam<PlayBackwardsSwitch>(Vec(94, 199), module, TapeRecorder::PLAY_BACKWARDS_PARAM));
	playBackwardsSwitch->tapeRecorder = module;
	addParam(playBackwardsSwitch);
	
	addParam(createParam<PauseSwitch>(Vec(94, 218.5), module, TapeRecorder::PAUSE_PARAM));
	
	PlayForwardsSwitch* playForwardsSwitch = dynamic_cast<PlayForwardsSwitch*>(createParam<PlayForwardsSwitch>(Vec(94, 238), module, TapeRecorder::PLAY_FORWARDS_PARAM));
	playForwardsSwitch->tapeRecorder = module;
	addParam(playForwardsSwitch);
	
	CueForwardsSwitch* cueForwardsSwitch = dynamic_cast<CueForwardsSwitch*>(createParam<CueForwardsSwitch>(Vec(94, 261.5), module, TapeRecorder::CUE_FORWARDS_PARAM));
	cueForwardsSwitch->tapeRecorder = module;
	addParam(cueForwardsSwitch);
	
	addParam(createParam<RoundSwitchRed>(Vec(18, 126), module, TapeRecorder::LOOP_START_BUTTON_PARAM));
	addParam(createParam<RoundSwitchRed>(Vec(54, 126), module, TapeRecorder::LOOP_END_BUTTON_PARAM));
	
	addInput(createInput<InPort>(Vec(84,  62), module, TapeRecorder::SPEED_INPUT));
	addInput(createInput<InPort>(Vec(94, 122), module, TapeRecorder::LOOP_INPUT));
	addInput(createInput<InPort>(Vec(94, 287), module, TapeRecorder::TRANSPORT_INPUT));
	addInput(createInput<InPort>(Vec( 4, 311), module, TapeRecorder::AUDIO_INPUT));

	addOutput(createOutput<OutPort>(Vec(94,  29), module, TapeRecorder::TEMPO_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(84,  89), module, TapeRecorder::SPEED_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(94, 149), module, TapeRecorder::LOOP_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(94, 314), module, TapeRecorder::TRANSPORT_OUTPUT));
	addOutput(createOutput<OutPort>(Vec( 4, 338), module, TapeRecorder::AUDIO_OUTPUT));

	addChild(new TapePositionDisplay(Rect(7, 82, 72, 32), module));
	addChild(new TapeDisplay(Rect(42, 217, 16, 24), module));
	addChild(new TapeLengthDisplay(Rect(10, 161, 22, 22), module));
	addChild(new TrackCountDisplay(Rect(10, 306, 22, 22), module));
	
	tapeNameDisplay = new TapeNameDisplay(Rect(70.5, 230, 122, 18));
	addChild(tapeNameDisplay);
	
	stripeWidget = new StripeWidget(Vec(38, 154));
	addChild(stripeWidget);
}

void TapeRecorderWidget::appendContextMenu(Menu* menu) {
	TapeRecorder* tapeRecorder = dynamic_cast<TapeRecorder*>(this->module);
	menu->addChild(new MenuEntry);
	menu->addChild(new TapeNameMenuItem(tapeNameDisplay));
	menu->addChild(new OldSchoolModeMenuItem(tapeRecorder));
	menu->addChild(new TapeLengthMenuItem(tapeRecorder));
	menu->addChild(new TrackCountMenuItem(tapeRecorder));
	menu->addChild(new TapeStripesMenuItem(stripeWidget));
}

json_t* TapeRecorderWidget::toJson() {
	json_t* rootJ = ModuleWidget::toJson();
	
	json_object_set_new(rootJ, "tape-name", json_string(tapeNameDisplay->text.c_str()));
	json_object_set_new(rootJ, "tape-stripe", json_integer(stripeWidget->stripe));
	return rootJ;
}

void TapeRecorderWidget::fromJson(json_t* rootJ) {
	ModuleWidget::fromJson(rootJ);

	json_t* tapeNameJ = json_object_get(rootJ, "tape-name");
	if (tapeNameJ) {
		tapeNameDisplay->text = json_string_value(tapeNameJ);
	}
	json_t* tapeStripeJ = json_object_get(rootJ, "tape-stripe");
	if (tapeStripeJ) {
		stripeWidget->setStripe(json_integer_value(tapeStripeJ));
	}
}

Model* modelTapeRecorder = createModel<TapeRecorder, TapeRecorderWidget>("TapeRecorder");
