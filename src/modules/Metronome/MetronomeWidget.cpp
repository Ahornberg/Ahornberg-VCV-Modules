#include "MetronomeWidget.hpp"

TempoDisplay::TempoDisplay(Rect box, Metronome* metronome) : Display(box) {
	this->metronome = metronome;
	bpmIndex = Metronome::INIT_BPM;
}

void TempoDisplay::drawText(const DrawArgs &disp) {
	if (metronome) {
		bpmIndex = metronome->params[Metronome::BPM_VALUE_PARAM].getValue();
	}
	Vec textPos = Vec(4, 14);
	nvgFillColor(disp.vg, textColorLight);
	if (Metronome::BPM_VALUES[bpmIndex] < 100) {
		nvgText(disp.vg, textPos.x, textPos.y, string::f(" %d", Metronome::BPM_VALUES[bpmIndex]).c_str(), NULL);
	} else {
		nvgText(disp.vg, textPos.x, textPos.y, string::f("%d", Metronome::BPM_VALUES[bpmIndex]).c_str(), NULL);
	}
}

BpmDisplay::BpmDisplay(Vec pos, int bpmIndex) {
	box.pos = pos;
	font = APP->window->loadFont(asset::plugin(pluginInstance, FONT_SERIF_BOLD));
	textColor = COLOR_BLACK;
	this->bpmIndex = bpmIndex;
}

void BpmDisplay::draw(const DrawArgs &disp) {
	nvgBeginPath(disp.vg);
	nvgFontSize(disp.vg, 9);
	nvgFontFaceId(disp.vg, font->handle);
	nvgFillColor(disp.vg, textColor);
	nvgTextAlign(disp.vg, NVG_ALIGN_CENTER);
	if (bpmIndex == 22 || bpmIndex == 37) {
		nvgText(disp.vg, -1, 2.5f, string::f("%d", Metronome::BPM_VALUES[bpmIndex]).c_str(), NULL);
	} else if (bpmIndex == 38) {
		nvgText(disp.vg, -4, 3, string::f("%d", Metronome::BPM_VALUES[bpmIndex]).c_str(), NULL);
	} else {
		nvgText(disp.vg, -1, 2, string::f("%d", Metronome::BPM_VALUES[bpmIndex]).c_str(), NULL);
	}
}

void RadioSwitch::onChange(const event::Change& e) {
	RoundSwitch::onChange(e);
	if (metronome) {
		if (metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + bpmIndex].getValue()) {
			for (auto i = 0; i < Metronome::NUM_BPM_VALUES; ++i) {
				if (i == bpmIndex) {
					if (!metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + i].getValue()) {
						metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + i].setValue(1);
					}
				} else {
					if (metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + i].getValue()) {
						metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + i].setValue(0);
					}
				}
			}
			metronome->params[Metronome::BPM_VALUE_PARAM].setValue(bpmIndex);
		} else {
			metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + metronome->params[Metronome::BPM_VALUE_PARAM].getValue()].setValue(1);
		}
	}
}

void KnobRadioSwitch::onChange(const event::Change& e) {
	KnobLarge::onChange(e);
	if (metronome) {
		for (auto i = 0; i < Metronome::NUM_BPM_VALUES; ++i) {
			int bpmIndex = metronome->params[Metronome::BPM_VALUE_PARAM].getValue();
			if (i == bpmIndex) {
				if (metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + i].getValue() != 1) {
					metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + i].setValue(1);
				}
			} else {
				if (metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + i].getValue() != 0) {
					metronome->params[Metronome::BPM_VALUE_BUTTON_PARAM + i].setValue(0);
				}
			}
		}
	}
}

MetronomeWidget::MetronomeWidget(Metronome* module) {
	setModule(module);
	setPanel("res/Metronome.svg");
	setSize(Vec(210, 380));
	setScrews(true, true, true, true);

	Vec center = Vec(105, 160);
	KnobRadioSwitch* knobRadioSwitch = dynamic_cast<KnobRadioSwitch*>(createParamCentered<KnobRadioSwitch>(center, module, Metronome::BPM_VALUE_PARAM));
	knobRadioSwitch->metronome = module;
	addParam(knobRadioSwitch);
	for (auto i = 0; i < Metronome::NUM_BPM_VALUES; ++i) {
		RadioSwitch* radioSwitch = dynamic_cast<RadioSwitch*>(createParamCentered<RadioSwitch>(positionOnCircle(i, center, 95), module, Metronome::BPM_VALUE_BUTTON_PARAM + i));
		radioSwitch->bpmIndex = i;
		radioSwitch->metronome = module;
		addParam(radioSwitch);
		
		addChild(new BpmDisplay(positionOnCircle(i, center, 80), i));
	}
	addParam(createParamCentered<RoundLargeSwitch>(center, module, Metronome::PLAY_PARAM));
	addParam(createParamCentered<KnobSmallSnap>(Vec(180, 266), module, Metronome::BPM_RESET_VALUE_PARAM));

	addInput(createInputCentered<InPort>(Vec( 30,   310), module, Metronome::PLAY_INPUT));
	addInput(createInputCentered<InPort>(Vec( 63,   310), module, Metronome::ONE_STEP_SLOWER_INPUT));
	addInput(createInputCentered<InPort>(Vec( 96,   310), module, Metronome::ONE_STEP_FASTER_INPUT));
	addInput(createInputCentered<InPort>(Vec(121.5, 310), module, Metronome::TWO_STEP_FASTER_INPUT));
	addInput(createInputCentered<InPort>(Vec(147,   310), module, Metronome::THREE_STEP_FASTER_INPUT));
	addInput(createInputCentered<InPort>(Vec(180,   310), module, Metronome::BPM_RESET_INPUT));

	addOutput(createOutputCentered<OutPort>(Vec(30, 350), module, Metronome::BPM_OUTPUT));

	addChild(new TempoDisplay(Rect(163, 40, 34, 18), module));
	// addChild(createLight<TempoDisplay>(Vec(7.5f, 38.0f), module, Metronome::BPM_VALUE_BUTTON_PARAM));
}

Vec MetronomeWidget::positionOnCircle(int positionInList, Vec center, int radius) {
	float angle = (-1.45 + positionInList * 0.05) * M_PI;
	return Vec(cos(angle) * radius + center.x, sin(angle) * radius + center.y);
}

Model* modelMetronome = createModel<Metronome, MetronomeWidget>("Metronome");
