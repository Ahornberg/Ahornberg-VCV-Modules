#include <osdialog.h>
#include "TapeRecorderWidget.hpp"

const Stripe StripeWidget::STRIPES[] = {
	{ "res/stripes/1970.svg",      "1970" },
	{ "res/stripes/Red.svg",       "Red" },
	{ "res/stripes/Green.svg",     "Green" },
	{ "res/stripes/Purple.svg",    "Purple" },
	{ "res/stripes/Brown.svg",     "Brown" },
	{ "res/stripes/Rainbow.svg",   "Rainbow" },
	{ "res/stripes/Stars.svg",     "Stars" },
	{ "res/stripes/Snowflake.svg", "Snowflake" },
	{ "res/stripes/Flowers.svg",   "Flowers" },
	{ "res/stripes/Tape.svg",      "Tape" },
	{ "res/stripes/Chip.svg",      "Chip" },
	{ "res/stripes/Zzz.svg",       "Zzz" }
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

void CueBackwardsSwitch::onButton(const event::Button& e) {
	e.consume(this);
	if (tapeRecorder && tapeRecorder->params[TapeRecorder::CUE_BACKWARDS_PARAM].getValue()) {
		int mods = APP->window->getMods();
		if ((mods & RACK_MOD_MASK) == RACK_MOD_CTRL) {
			tapeRecorder->jumpToTapePosition(TapeRecorder::JUMP_BACKWARDS);
			tapeRecorder->params[TapeRecorder::CUE_FORWARDS_PARAM].setValue(0.);
			tapeRecorder->params[TapeRecorder::CUE_BACKWARDS_PARAM].setValue(0.);
		} else if ((mods & RACK_MOD_MASK) == (RACK_MOD_CTRL | GLFW_MOD_SHIFT)) {
			tapeRecorder->jumpToTapePosition(TapeRecorder::JUMP_TO_BEGIN);
			tapeRecorder->params[TapeRecorder::CUE_FORWARDS_PARAM].setValue(0.);
			tapeRecorder->params[TapeRecorder::CUE_BACKWARDS_PARAM].setValue(0.);
		} else {
			tapeRecorder->params[TapeRecorder::CUE_FORWARDS_PARAM].setValue(0.);
		}	
	}
}

CueForwardsSwitch::CueForwardsSwitch() {
	addFrame("res/switches/CueForwards_off.svg");
	addFrame("res/switches/CueForwards_on.svg");
}

void CueForwardsSwitch::onButton(const event::Button& e) {
	e.consume(this);
	if (tapeRecorder && tapeRecorder->params[TapeRecorder::CUE_FORWARDS_PARAM].getValue()) {
		int mods = APP->window->getMods();
		if ((mods & RACK_MOD_MASK) == RACK_MOD_CTRL) {
			tapeRecorder->jumpToTapePosition(TapeRecorder::JUMP_FORWARDS);
			tapeRecorder->params[TapeRecorder::CUE_FORWARDS_PARAM].setValue(0.);
			tapeRecorder->params[TapeRecorder::CUE_BACKWARDS_PARAM].setValue(0.);
		} else if ((mods & RACK_MOD_MASK) == (RACK_MOD_CTRL | GLFW_MOD_SHIFT)) {
			tapeRecorder->jumpToTapePosition(TapeRecorder::JUMP_TO_END);
			tapeRecorder->params[TapeRecorder::CUE_FORWARDS_PARAM].setValue(0.);
			tapeRecorder->params[TapeRecorder::CUE_BACKWARDS_PARAM].setValue(0.);
		} else {
			tapeRecorder->params[TapeRecorder::CUE_BACKWARDS_PARAM].setValue(0.);
		}
	}
}

PlayBackwardsSwitch::PlayBackwardsSwitch() {
	addFrame("res/switches/PlayBackwards_off.svg");
	addFrame("res/switches/PlayBackwards_on.svg");
}

void PlayBackwardsSwitch::onButton(const event::Button& e) {
	e.consume(this);
	if (tapeRecorder && tapeRecorder->params[TapeRecorder::PLAY_BACKWARDS_PARAM].getValue()) {
		tapeRecorder->params[TapeRecorder::PLAY_FORWARDS_PARAM].setValue(0.);
	}
}

PlayForwardsSwitch::PlayForwardsSwitch() {
	addFrame("res/switches/PlayForwards_off.svg");
	addFrame("res/switches/PlayForwards_on.svg");
}

void PlayForwardsSwitch::onButton(const event::Button& e) {
	e.consume(this);
	if (tapeRecorder && tapeRecorder->params[TapeRecorder::PLAY_FORWARDS_PARAM].getValue()) {
		tapeRecorder->params[TapeRecorder::PLAY_BACKWARDS_PARAM].setValue(0.);
	}
}

// Knobs **********************************************************************

const int KnobWheel::SMEARED_WHEELS_DISTRIBUTION[] = {
	1,
	3,
	5,
	7,
	9,
	11,
	13,
	15,
	17,
	19,
	21,
	23,
	25,
	27,
	29,
	31,
	30,
	28,
	26,
	24,
	22,
	20,
	18,
	16,
	14,
	12,
	10,
	6,
	8,
	4,
	2,
	0
};

KnobWheel::KnobWheel() {
	for (auto i = 0; i < NUM_SMEARED_WHEELS; ++i) {
		smearedWheelsTransform[i] = new widget::TransformWidget;
		fb->addChild(smearedWheelsTransform[i]);
		smearedWheelsSvg[i] = new widget::SvgWidget;
		smearedWheelsTransform[i]->addChild(smearedWheelsSvg[i]);
	}
	setSvgSmeared(APP->window->loadSvg(asset::plugin(pluginInstance, "res/knobs/Wheel.svg")));
	minAngle = -6.f * M_PI;
	maxAngle = 6.f * M_PI;
	shadow->opacity = 0.f;
	shadow->box.pos = Vec(0, 0);
	speed = 0.1f;
	cursorHand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
}

void KnobWheel::setSvgSmeared (std::shared_ptr<Svg> svg) {
	for (auto i = 0; i < NUM_SMEARED_WHEELS; ++i) {
		smearedWheelsSvg[i]->setSvg(svg);
		smearedWheelsTransform[i]->box.size = smearedWheelsSvg[i]->box.size;
	}
	tw->box.size = smearedWheelsSvg[0]->box.size;
	fb->box.size = smearedWheelsSvg[0]->box.size;
	box.size = smearedWheelsSvg[0]->box.size;
	shadow->box.size = smearedWheelsSvg[0]->box.size;
	// Move shadow downward by 10%
	shadow->box.pos = math::Vec(0, smearedWheelsSvg[0]->box.size.y * 0.10);
	// shadow->box = shadow->box.grow(math::Vec(2, 2));
}

float KnobWheel::distance(Vec* p1, Vec* p2) {
	return sqrt(pow(p1->x - p2->x, 2) + pow(p1->y - p2->y, 2));
}

float KnobWheel::calcTouchedWheelForce(float distance, float maxDistance, int mods) {
	if (distance >= maxDistance) {
		glfwSetCursor(APP->window->win, NULL);
		return 1;
	}
	glfwSetCursor(APP->window->win, cursorHand);
	// Drag slower if mod is held
	if ((mods & RACK_MOD_MASK) == RACK_MOD_CTRL) {
		return (maxDistance - (distance * 1.1f)) / maxDistance;
	}
	// Drag even slower if mod+shift is held
	if ((mods & RACK_MOD_MASK) == (RACK_MOD_CTRL | GLFW_MOD_SHIFT)) {
		return (maxDistance - (distance * 1.2f)) / maxDistance;
	}			
	return (maxDistance - distance) / maxDistance;
}

void KnobWheel::onHover(const event::Hover& e) {
	// avoid tooltip
}

void KnobWheel::onButton(const event::Button& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT && e.action == GLFW_PRESS) {
		mousePos = new Vec(e.pos.x, e.pos.y);
		Vec* center = new Vec(box.size.x * 0.5f, box.size.y * 0.5f);
		int mods = APP->window->getMods();
		module->touchedWheelForce = calcTouchedWheelForce(distance(mousePos, center), center->x, mods);
	} else if (e.button == GLFW_MOUSE_BUTTON_RIGHT && e.action == GLFW_PRESS) {
		// avoid context menu
		return;
	}
	SvgKnob::onButton(e);
}

void KnobWheel::onDragHover(const event::DragHover& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT) {
		mousePos = new Vec(e.pos.x, e.pos.y);
		Vec* center = new Vec(box.size.x * 0.5f, box.size.y * 0.5f);
		int mods = APP->window->getMods();
		module->touchedWheelForce = calcTouchedWheelForce(distance(mousePos, center), center->x, mods);
	}
	SvgKnob::onDragHover(e);
}

void KnobWheel::onDragStart(const event::DragStart& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT) {
		module->wheelMovement = 0;
		return;
	}
	SvgKnob::onDragStart(e);
}

void KnobWheel::onDragEnd(const event::DragEnd& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT) {
		module->touchedWheelForce = 1;
		glfwSetCursor(APP->window->win, NULL);
		module->wheelMovement = 0;
		return;
	}
	SvgKnob::onDragEnd(e);
}

void KnobWheel::onDragLeave(const event::DragLeave& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT) {
		module->touchedWheelForce = 1;
		module->wheelMovement = 0;
		return;
	}
	SvgKnob::onDragLeave(e);
}

void KnobWheel::onDragMove(const event::DragMove& e) {
	if (module && e.button == GLFW_MOUSE_BUTTON_LEFT) {
		// mousePos->x += e.mouseDelta.x / exp2(settings::zoom);
		// mousePos->y += e.mouseDelta.y / exp2(settings::zoom);
		mousePos->x += e.mouseDelta.x / exp2(getAbsoluteZoom());
		mousePos->y += e.mouseDelta.y / exp2(getAbsoluteZoom());
		Vec* center = new Vec(box.size.x * 0.5f, box.size.y * 0.5f);
		int mods = APP->window->getMods();
		module->touchedWheelForce = calcTouchedWheelForce(distance(mousePos, center), center->x, mods);
		
		float delta = e.mouseDelta.y;
		delta *= -0.024f;
		// Drag slower if mod is held
		if ((mods & RACK_MOD_MASK) == RACK_MOD_CTRL) {
			delta /= 10.f;
		}
		// Drag even slower if mod+shift is held
		if ((mods & RACK_MOD_MASK) == (RACK_MOD_CTRL | GLFW_MOD_SHIFT)) {
			delta /= 100.f;
		}			
		module->wheelMovement = delta;
		return;
	}
	SvgKnob::onDragMove(e);
}

void KnobWheel::onDoubleClick(const event::DoubleClick& e) {
	// avoid double click
}

void KnobWheel::onChange(const event::Change& e) {
	// Re-transform the widget::TransformWidget
	float clampValue = 1 / (9.f * NUM_SMEARED_WHEELS);
	// float frameRate = APP->window->getLastFrameRate();
	ParamQuantity* paramQuantity = getParamQuantity();
	if (paramQuantity && module) {
		float diff;
		float paramValue;
		float radius;
		float tapeLength = module->tapeLengthParam * 1.3f;
		if (paramQuantity->isBounded()) {
			paramValue = paramQuantity->getScaledValue();
		} else {
			paramValue = paramQuantity->getValue();
		}
		if (paramId == TapeRecorder::WHEEL_LEFT_PARAM) {
			radius = (TapeDisplay::RADIUS_MAX - TapeDisplay::RADIUS_MIN + tapeLength) * module->tapeOnLeftWheel + TapeDisplay::RADIUS_MIN;
		} else {
			radius = (TapeDisplay::RADIUS_MAX - TapeDisplay::RADIUS_MIN + tapeLength) * module->tapeOnRightWheel + TapeDisplay::RADIUS_MIN;
		}
		diff = clamp(pow(module->tapeSpeed * TapeDisplay::RADIUS_MAX / radius, 27) / 1000000000000000.f, clampValue * -1.f, clampValue);
		// diff = 0;
		for (auto i = 0; i < NUM_SMEARED_WHEELS; ++i) {
			smearedWheelsAngle[i] = std::fmod(math::rescale(paramValue - diff * SMEARED_WHEELS_DISTRIBUTION[i], -1.f, 1.f, minAngle, maxAngle), 2 * M_PI);
			smearedWheelsTransform[i]->identity();
			// Rotate SVG
			math::Vec center = smearedWheelsSvg[i]->box.getCenter();
			smearedWheelsTransform[i]->translate(center);
			smearedWheelsTransform[i]->rotate(smearedWheelsAngle[i]);
			smearedWheelsTransform[i]->translate(center.neg());
		}
		fb->dirty = true;
	}
	Knob::onChange(e);
}

// void KnobWheel::draw(const DrawArgs& args) {
	// tw->identity();
	// // Rotate SVG
	// math::Vec center = sw->box.getCenter();
	// tw->translate(center);
	// for (auto i = 0; i < NUM_SMEARED_WHEELS; ++i) {
		// tw->rotate(smearedWheelsAngle[i]);
		// Widget::draw(args);
	// }
	// tw->translate(center.neg());
// }


// Displays *******************************************************************

TapePositionDisplay::TapePositionDisplay(Rect box, TapeRecorder* tapeRecorder) : Display(box) {
	this->tapeRecorder = tapeRecorder;
	tapePosition = 0;
	beatsPerBar = 0;
	loopMode = 0;
	loopStart = 0;
	loopEnd = 0;
	loopStartConnected = false;
	loopEndConnected = false;
	// loopStartOnTapePosition = false;
	// loopEndOnTapePosition = false;
	playStatus = false;
	cueStatus = false;
	playForwardStatus = false;
	cueForwardStatus = false;
	changeTapeInterrupt = false;
	loadProgress = 0;
}

void TapePositionDisplay::drawText(const DrawArgs& disp) {
	if (tapeRecorder) {
		loopMode = tapeRecorder->params[TapeRecorder::LOOP_MODE_PARAM].getValue();
		loopStartConnected = tapeRecorder->inputs[TapeRecorder::LOOP_START_INPUT].isConnected();
		loopStart = tapeRecorder->loopStart;
		loopEndConnected = tapeRecorder->inputs[TapeRecorder::LOOP_END_INPUT].isConnected();
		// loopEndConnected = (tapeRecorder->inputs[TapeRecorder::LOOP_INPUT].isConnected() && tapeRecorder->inputs[TapeRecorder::LOOP_INPUT].getChannels() > 1);
		loopEnd = tapeRecorder->loopEnd;
		beatsPerBar = tapeRecorder->params[TapeRecorder::BEATS_PER_BAR_PARAM].getValue();
		tapePosition = tapeRecorder->tapePosition + 0.5 / beatsPerBar;
		playForwardStatus = tapeRecorder->playForwardStatus;
		playStatus = tapeRecorder->playStatus;
		cueForwardStatus = tapeRecorder->cueForwardStatus;
		cueStatus = tapeRecorder->cueStatus;
		// loopStartOnTapePosition = (tapeRecorder->params[TapeRecorder::LOOP_START_BUTTON_PARAM].getValue() == 1.0f);
		// loopEndOnTapePosition = (tapeRecorder->params[TapeRecorder::LOOP_END_BUTTON_PARAM].getValue() == 1.0f);
		changeTapeInterrupt = tapeRecorder->changeTapeInterrupt;
	}
	if (changeTapeInterrupt) {
		Vec textPos = Vec(4, 14);
		nvgFillColor(disp.vg, textColorLight);
		nvgText(disp.vg, textPos.x, textPos.y, "loading", NULL);
		nvgFontSize(disp.vg, 8);
		textPos = Vec(4, 28);
		std::string text = "";
		for (auto i = 0; i < loadProgress / 30; ++i) {
			text += "-";
		}
		++loadProgress;
		nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
	} else {
		loadProgress = 0;
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
			if (loopMode) {
				nvgText(disp.vg, textPos.x, textPos.y, "P", NULL);
			} else {
				nvgText(disp.vg, textPos.x, textPos.y, "C", NULL);
			}
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
		if ((int) loopStart != loopStart) {
			nvgFillColor(disp.vg, textColorRed);
		} else 
		if (loopStartConnected) {
			nvgFillColor(disp.vg, textColorDark);
		} else {
			nvgFillColor(disp.vg, textColorLight);
		}
		text = std::to_string((int) loopStart);
		if (loopStart < 100) {
			text = " " + text;
		} 
		if (loopStart < 10) {
			text = " " + text;
		}
		nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
		
		textPos = Vec(46, 28);
		if ((int) loopEnd != loopEnd) {
			nvgFillColor(disp.vg, textColorRed);
		} else 
		if (loopEndConnected) {
			nvgFillColor(disp.vg, textColorDark);
		} else {
			nvgFillColor(disp.vg, textColorLight);
		}
		text = std::to_string((int) loopEnd);
		if (loopEnd < 100) {
			text = " " + text;
		} 
		if (loopEnd < 10) {
			text = " " + text;
		}
		nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
	}
}
	
ModuleLinkedWidget::ModuleLinkedWidget(Rect box, TapeRecorder* tapeRecorder) : SizedTransparentWidget(box) {
	this->tapeRecorder = tapeRecorder;
}

TapeDisplay::TapeDisplay(Rect box, TapeRecorder* tapeRecorder) : ModuleLinkedWidget(box, tapeRecorder) {
	tapeColor = COLOR_BROWN;
	tapeOnLeftWheel = 0;
	tapeOnRightWheel = 1;
	tapeLength = 9;
}

void TapeDisplay::draw(const DrawArgs& disp) {
	if (tapeRecorder) {
		tapeOnLeftWheel = tapeRecorder->tapeOnLeftWheel;
		tapeOnRightWheel = tapeRecorder->tapeOnRightWheel;
		tapeLength = tapeRecorder->tapeLengthParam * 1.3f;
	}
	float radiusLeft = (RADIUS_MAX - RADIUS_MIN + tapeLength) * tapeOnLeftWheel + RADIUS_MIN;
	float radiusRight = (RADIUS_MAX - RADIUS_MIN + tapeLength) * tapeOnRightWheel + RADIUS_MIN;
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

void TextOnCassette::drawText(const Widget::DrawArgs& disp, Rect box) {
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
		nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
	}
}

TapeLengthDisplay::TapeLengthDisplay(Rect box, TapeRecorder* tapeRecorder) : ModuleLinkedWidget(box, tapeRecorder) {
	fontPath = asset::plugin(pluginInstance, FONT_SERIF_BOLD);
	textColor = COLOR_BLACK;
	fontSize = 18;
	textAlign = NVG_ALIGN_RIGHT;
	useScissor = false;
	textPos =  Vec(4, 14);
}

void TapeLengthDisplay::draw(const DrawArgs& disp) {
	if (tapeRecorder) {
		text = TapeRecorder::TAPE_LENGTHS[tapeRecorder->tapeLengthParam].name;
	} else {
		// widget without module
		text = "MC 1";
	}
	drawText(disp, box);
}

std::string TrackCountText::createTrackCountText(const int trackCount) {
	if (trackCount == 1) {
		return "Mono";
	}
	if (trackCount == 2) {
		return "Stereo";
	}
	return string::f("%d Tracks", trackCount);
}

TrackCountDisplay::TrackCountDisplay(Rect box, TapeRecorder* tapeRecorder) : ModuleLinkedWidget(box, tapeRecorder) {
	fontPath = asset::plugin(pluginInstance, FONT_SERIF_BOLD);
	textColor = COLOR_BLACK;
	fontSize = 14;
	textAlign = NVG_ALIGN_LEFT;
	useScissor = false;
	textPos =  Vec(4, 14);
}

void TrackCountDisplay::draw(const DrawArgs& disp) {
	if (tapeRecorder) {
		text = createTrackCountText(tapeRecorder->trackCountParam);
	} else {
		// widget without module
		text = createTrackCountText(1);
	}
	drawText(disp, box);
}

TapeNameDisplay::TapeNameDisplay(Rect box, TapeRecorder* tapeRecorder) : SizedTransparentWidget(box) {
	this->tapeRecorder = tapeRecorder;
	fontPath = asset::plugin(pluginInstance, FONT_HANDWRITE);
	textColor = COLOR_BLACK;
	fontSize = 16;
	textAlign = NVG_ALIGN_CENTER;
	useScissor = true;
	textPos =  Vec(0, 11);
}

void TapeNameDisplay::draw(const DrawArgs& disp) {
	if (tapeRecorder) {
		text = tapeRecorder->tapeName;
	} else {
		text = TapeRecorder::INITIAL_TAPE_NAME;
	}
	drawText(disp, box);
}

// Menu ***********************************************************************
	
TapeRecorderMenuItem::TapeRecorderMenuItem(TapeRecorder* tapeRecorder) {
	this->tapeRecorder = tapeRecorder;
}

TapeNameMenuItem::TapeNameMenuItem(TapeRecorder* tapeRecorder) {
	this->tapeRecorder = tapeRecorder;
	if (tapeRecorder) {
		text = tapeRecorder->tapeName;
	} else {
		text = TapeRecorder::INITIAL_TAPE_NAME;
	}
}

void TapeNameMenuItem::onChange(const event::Change& e) {
	TextFieldMenuItem::onChange(e);
	if (tapeRecorder) {
		tapeRecorder->tapeName = text;
	}
}

const std::string LoopModeValueItem::LOOP_MODES[] = {
	"Cycle",
	"Ping-Pong"
};

LoopModeValueItem::LoopModeValueItem(TapeRecorder* tapeRecorder, int loopMode) : TapeRecorderMenuItem(tapeRecorder) {
	this->loopMode = loopMode;
	text = LOOP_MODES[loopMode];
	if (tapeRecorder) {
		rightText = CHECKMARK(loopMode == tapeRecorder->params[TapeRecorder::LOOP_MODE_PARAM].getValue());
	}
}

void LoopModeValueItem::onAction(const event::Action& e) {
	if (tapeRecorder) {
		tapeRecorder->params[TapeRecorder::LOOP_MODE_PARAM].setValue(loopMode);
	}
}

LoopModeMenuItem::LoopModeMenuItem(TapeRecorder* tapeRecorder) : TapeRecorderMenuItem(tapeRecorder) {
	text = "Loop Mode";
	if (tapeRecorder) {
		rightText = LoopModeValueItem::LOOP_MODES[(int) tapeRecorder->params[TapeRecorder::LOOP_MODE_PARAM].getValue()] + " " + RIGHT_ARROW;
	}
}

Menu* LoopModeMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	for (auto i = 0; i < LoopModeValueItem::NUM_LOOP_MODES; ++i) {
		menu->addChild(new LoopModeValueItem(tapeRecorder, i));
	}
	return menu;
}

TrackCountValueItem::TrackCountValueItem(TapeRecorder* tapeRecorder, int trackCount, std::string trackCountText) : TapeRecorderMenuItem(tapeRecorder) {
	this->trackCount = trackCount;
	text = trackCountText;
	rightText = CHECKMARK(tapeRecorder->trackCountParam == trackCount);
}

void TrackCountValueItem::onAction(const event::Action& e) {
	if (tapeRecorder) {
		tapeRecorder->setTrackCount(trackCount);
	}
}

TrackCountMenuItem::TrackCountMenuItem(TapeRecorder* tapeRecorder) : TapeRecorderMenuItem(tapeRecorder) {
	text = "Track Count";
	if (tapeRecorder) {
		rightText = createTrackCountText(tapeRecorder->trackCountParam) + " " + RIGHT_ARROW;
	}
}

Menu* TrackCountMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	for (auto trackCount = 1; trackCount <= TapeRecorder::NUM_MAX_TRACKS; ++trackCount) {
		if (trackCount == 3) {
			// only 1, 2 or 4 tracks
			continue;
		}
		if (trackCount == TapeRecorder::NUM_MAX_TRACKS && tapeRecorder && tapeRecorder->tapeLengthParam == TapeRecorder::NUM_TAPE_LENGTHS - 1) {
			// AudioFile.h can't handle MC 120 with 4 tracks
			break;
		}
		menu->addChild(new TrackCountValueItem(tapeRecorder, trackCount, createTrackCountText(trackCount)));
	}
	return menu;
}

TapeLengthValueItem::TapeLengthValueItem(TapeRecorder* tapeRecorder, int tapeLength) : TapeRecorderMenuItem(tapeRecorder) {
	this->tapeRecorder = tapeRecorder;
	this->tapeLength = tapeLength;
	text = TapeRecorder::TAPE_LENGTHS[tapeLength].name;
	if (tapeRecorder) {
		rightText = CHECKMARK(tapeRecorder->tapeLengthParam == tapeLength);
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
		rightText = TapeRecorder::TAPE_LENGTHS[tapeRecorder->tapeLengthParam].name + " " + RIGHT_ARROW;
	}
}

Menu* TapeLengthMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	for (auto i = 0; i < TapeRecorder::NUM_TAPE_LENGTHS; ++i) {
		if (i == TapeRecorder::NUM_TAPE_LENGTHS - 1 && tapeRecorder && tapeRecorder->trackCountParam == TapeRecorder::NUM_MAX_TRACKS) {
			// AudioFile.h can't handle MC 120 with 4 tracks
			break;
		}
		menu->addChild(new TapeLengthValueItem(tapeRecorder, i));
	}
	return menu;
}

TapeStripesValueItem::TapeStripesValueItem(TapeRecorder* tapeRecorder, int stripe) {
	this->tapeRecorder = tapeRecorder;
	this->stripe = stripe;
	text = StripeWidget::STRIPES[stripe].name;
	if (tapeRecorder) {
		rightText = CHECKMARK(tapeRecorder->stripeIndex == stripe);
	} else {
		rightText = CHECKMARK(tapeRecorder->stripeIndex == 0);
	}
}

void TapeStripesValueItem::onAction(const event::Action& e) {
	if (tapeRecorder) {
		tapeRecorder->stripeIndex = stripe;
	}
	//stripeWidget->setStripe(stripe);
}

TapeStripesMenuItem::TapeStripesMenuItem(TapeRecorder* tapeRecorder) {
	this->tapeRecorder = tapeRecorder;
	text = "Cassette Label";
	if (tapeRecorder) {
		rightText = StripeWidget::STRIPES[tapeRecorder->stripeIndex].name + " " + RIGHT_ARROW;
	} else {
		rightText = StripeWidget::STRIPES[0].name + " " + RIGHT_ARROW;
	}
}

Menu* TapeStripesMenuItem::createChildMenu() {
	Menu* menu = new Menu;
	for (auto i = 0; i < StripeWidget::NUM_STRIPES; ++i) {
		menu->addChild(new TapeStripesValueItem(tapeRecorder, i));
	}
	return menu;
}

EraseTapeMenuItem::EraseTapeMenuItem(TapeRecorder* tapeRecorder) : TapeRecorderMenuItem(tapeRecorder) {
	text = "Erase Tape";
}

void EraseTapeMenuItem::onAction(const event::Action& e) {
	if (tapeRecorder) {
		tapeRecorder->initTapeThread(TapeRecorder::INIT_TAPE_ERASE);
	}
}

// Widget *********************************************************************
	
TapeRecorderWidget::TapeRecorderWidget(TapeRecorder* module) {
	setModule(module);
	setPanel("res/TapeRecorder.svg");
	setWidthInHP(8);

	addParam(createParam<KnobBig>(Vec(      43,  42),   module, TapeRecorder::TEMPO_PARAM));
	addParam(createParam<KnobSmallSnap>(Vec(10,  50),   module, TapeRecorder::BEATS_PER_BAR_PARAM));
	addParam(createParam<KnobSmallSnap>(Vec(10, 118),   module, TapeRecorder::LOOP_START_PARAM));
	addParam(createParam<KnobSmallSnap>(Vec(46, 118),   module, TapeRecorder::LOOP_END_PARAM));
	addParam(createParam<KnobScrew>(Vec(    42, 334.5), module, TapeRecorder::CUE_SPEED_PARAM));
	addParam(createParam<KnobScrew>(Vec(    61, 330),   module, TapeRecorder::CUE_SLEW_PARAM));
	
	KnobWheel* rightWheel = createParam<KnobWheel>(Vec(32, 178), module, TapeRecorder::WHEEL_RIGHT_PARAM);
	rightWheel->module = module;
	rightWheel->paramId = TapeRecorder::WHEEL_RIGHT_PARAM;
	addParam(rightWheel);
	
	KnobWheel* leftWheel = createParam<KnobWheel>(Vec(32, 244), module, TapeRecorder::WHEEL_LEFT_PARAM);
	leftWheel->module = module;
	leftWheel->paramId = TapeRecorder::WHEEL_LEFT_PARAM;
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
	addInput(createInput<InPort>(Vec(94, 122), module, TapeRecorder::LOOP_END_INPUT));
	addInput(createInput<InPort>(Vec(94, 149), module, TapeRecorder::LOOP_START_INPUT));
	addInput(createInput<InPort>(Vec(94, 287), module, TapeRecorder::TRANSPORT_INPUT));
	addInput(createInput<InPort>(Vec( 4, 311), module, TapeRecorder::AUDIO_INPUT));

	addOutput(createOutput<OutPort>(Vec(94,  29), module, TapeRecorder::TEMPO_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(84,  89), module, TapeRecorder::SPEED_OUTPUT));
	// addOutput(createOutput<OutPort>(Vec(94, 149), module, TapeRecorder::LOOP_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(94, 314), module, TapeRecorder::TRANSPORT_OUTPUT));
	addOutput(createOutput<OutPort>(Vec( 4, 338), module, TapeRecorder::AUDIO_OUTPUT));

	addChild(new TapePositionDisplay(Rect(7, 82, 72, 32), module));
	addChild(new TapeDisplay(Rect(42, 217, 16, 24), module));
	addChild(new TapeLengthDisplay(Rect(10, 161, 22, 22), module));
	addChild(new TrackCountDisplay(Rect(10, 306, 22, 22), module));

	addChild(new TapeNameDisplay(Rect(70.5, 230, 122, 18), module));
	// tapeNameDisplay = new TapeNameDisplay(Rect(70.5, 230, 122, 18), module);
	// addChild(tapeNameDisplay);
	
	stripeWidget = new StripeWidget(Vec(38, 154));
	addChild(stripeWidget);
}

void TapeRecorderWidget::contextMenu(Menu* menu) {
	TapeRecorder* tapeRecorder = dynamic_cast<TapeRecorder*>(this->module);
	menu->addChild(new TapeNameMenuItem(tapeRecorder));
	menu->addChild(new TapeStripesMenuItem(tapeRecorder));
	menu->addChild(new TapeLengthMenuItem(tapeRecorder));
	menu->addChild(new TrackCountMenuItem(tapeRecorder));
	menu->addChild(new LoopModeMenuItem(tapeRecorder));
	menu->addChild(new EraseTapeMenuItem(tapeRecorder));
}

void TapeRecorderWidget::step() {
	TapeRecorder* tapeRecorder = dynamic_cast<TapeRecorder*>(module);
	if (tapeRecorder) {
		if (tapeRecorder->stripeIndex != stripeWidget->stripe) {
			stripeWidget->setStripe(tapeRecorder->stripeIndex);
		}
		if (!tapeRecorder->warningString.empty()) {
			osdialog_message(OSDIALOG_WARNING, OSDIALOG_OK, tapeRecorder->warningString.c_str());
			tapeRecorder->warningString = "";
		}
		if (tapeRecorder->callInitTape != TapeRecorder::INIT_TAPE_NOOP) {
			tapeRecorder->initTapeThread(tapeRecorder->callInitTape);
		}
	}
	BaseModuleWidget::step();
}

void TapeRecorderWidget::loadActionCustom(std::string filename) {
	// history::ModuleChange
	history::ModuleChange* h = new history::ModuleChange;
	h->name = "load module preset";
	h->moduleId = module->id;
	h->oldModuleJ = toJson();

	try {
		load(filename);
		// load audio
		std::string audioFilename = system::join(
		system::getDirectory(filename), system::getStem(filename) + ".wav");
		if (system::exists(audioFilename)) {
			dynamic_cast<TapeRecorder*>(module)->loadAudioFile(audioFilename);
		} else {
			dynamic_cast<TapeRecorder*>(module)->loadAudioFile("");
		}
	}
	catch (Exception& e) {
		delete h;
		throw;
	}

	// TODO We can use `moduleJ` here instead to save a toJson() call.
	h->newModuleJ = toJson();
	APP->history->push(h);
}

void TapeRecorderWidget::saveDialogCustom() {
	std::string presetDir = model->getUserPresetDirectory();
	system::createDirectories(presetDir);

	// Delete directories if empty
	DEFER({
		try {
			system::remove(presetDir);
			system::remove(system::getDirectory(presetDir));
		}
		catch (Exception& e) {
			// Ignore exceptions if directory cannot be removed.
		}
	});

	osdialog_filters* filters = osdialog_filters_parse(PRESET_FILTERS);
	DEFER({osdialog_filters_free(filters);});

	char* pathC = osdialog_file(OSDIALOG_SAVE, presetDir.c_str(), "Untitled.vcvm", filters);
	if (!pathC) {
		// No path selected
		return;
	}
	DEFER({std::free(pathC);});

	std::string path = pathC;
	// Automatically append .vcvm extension
	if (system::getExtension(path) != ".vcvm")
		path += ".vcvm";
	
	// save audio
	dynamic_cast<TapeRecorder*>(module)->saveAudioFile(system::join(
		system::getDirectory(path), system::getStem(path) + ".wav"));
	// save template
	save(path);
}

void TapeRecorderWidget::saveTemplateCustom() {
	// save audio
	std::string presetDir = model->getUserPresetDirectory();
	system::createDirectories(presetDir);
	dynamic_cast<TapeRecorder*>(module)->saveAudioFile(system::join(presetDir, "template.wav"));
	// save template	
	saveTemplate();
}

Model* modelTapeRecorder = createModel<TapeRecorder, TapeRecorderWidget>("TapeRecorder");
