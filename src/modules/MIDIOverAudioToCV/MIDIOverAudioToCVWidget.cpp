#include "MIDIOverAudioToCVWidget.hpp"

// IntervalDisplay::IntervalDisplay(Rect box, EqualDivision *equalDivision) : Display(box) {
	// this->equalDivision = equalDivision;
	// step = EqualDivision::INIT_STEP;
	// interval = EqualDivision::INIT_INTERVAL;
// }

// void IntervalDisplay::drawText(const DrawArgs& disp) {
	// if (equalDivision) {
		// step = (int) equalDivision->params[EqualDivision::STEPS_PARAM].getValue();
		// interval = (int) equalDivision->params[EqualDivision::INTERVAL_PARAM].getValue();
	// }
	// Vec textPos = Vec(4, 14);
	// nvgFillColor(disp.vg, textColorLight);
	// if (step < 10) {
		// nvgText(disp.vg, textPos.x, textPos.y, (" " + std::to_string(step)).c_str(), NULL);
	// } else {
		// nvgText(disp.vg, textPos.x, textPos.y, (std::to_string(step)).c_str(), NULL);
	// }
	// nvgFillColor(disp.vg, textColorDark);
	// nvgFontSize(disp.vg, 8);
	// nvgText(disp.vg, textPos.x + 21, textPos.y, "ed", NULL);
	// nvgFillColor(disp.vg, textColorLight);
	// nvgFontSize(disp.vg, 10);
	// nvgText(disp.vg, textPos.x + 40, textPos.y, EqualDivision::INTERVALS[interval].shortName.c_str(), NULL);
// }

MIDIOverAudioToCVWidget::MIDIOverAudioToCVWidget(MIDIOverAudioToCV* module) {
	setModule(module);
	setPanel("res/EqualDivision.svg");
	setSize(Vec(90, 380));
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, NO_SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);

	// addParam(createParam<KnobBig>(      Vec(             46.5,  47), module, EqualDivision::FINE_PARAM));
	// addParam(createParam<KnobSmallSnap>(Vec(             10,   124), module, EqualDivision::STEPS_PARAM));
	// addParam(createParam<KnobSmallSnap>(Vec(box.size.x - 37,   124), module, EqualDivision::INTERVAL_PARAM));

	addInput(createInput<InPort>(Vec( 4, 167),    module, MIDIOverAudioToCV::PITCH_INPUT + 0));
	// addInput(createInput<InPort>(Vec(33, 167),    module, EqualDivision::PITCH_INPUT + 1));
	// addInput(createInput<InPort>(Vec(62, 167),    module, EqualDivision::PITCH_INPUT + 2));
	// addInput(createInput<InPort>(Vec( 4, 233.5),  module, EqualDivision::PITCH_INPUT + 3));
	// addInput(createInput<InPort>(Vec(33, 229.75), module, EqualDivision::PITCH_INPUT + 4));
	// addInput(createInput<InPort>(Vec(62, 226),    module, EqualDivision::PITCH_INPUT + 5));
	// addInput(createInput<InPort>(Vec( 4, 300),    module, EqualDivision::PITCH_INPUT + 6));
	// addInput(createInput<InPort>(Vec(33, 292.5),  module, EqualDivision::PITCH_INPUT + 7));
	// addInput(createInput<InPort>(Vec(62, 285),    module, EqualDivision::PITCH_INPUT + 8));

	// addOutput(createOutput<OutPort>(Vec( 4, 194),    module, EqualDivision::PITCH_OUTPUT + 0));
	// addOutput(createOutput<OutPort>(Vec(33, 194),    module, EqualDivision::PITCH_OUTPUT + 1));
	// addOutput(createOutput<OutPort>(Vec(62, 194),    module, EqualDivision::PITCH_OUTPUT + 2));
	// addOutput(createOutput<OutPort>(Vec( 4, 260.5),  module, EqualDivision::PITCH_OUTPUT + 3));
	// addOutput(createOutput<OutPort>(Vec(33, 256.75), module, EqualDivision::PITCH_OUTPUT + 4));
	// addOutput(createOutput<OutPort>(Vec(62, 253),    module, EqualDivision::PITCH_OUTPUT + 5));
	// addOutput(createOutput<OutPort>(Vec( 4, 327),    module, EqualDivision::PITCH_OUTPUT + 6));
	// addOutput(createOutput<OutPort>(Vec(33, 319.5),  module, EqualDivision::PITCH_OUTPUT + 7));
	// addOutput(createOutput<OutPort>(Vec(62, 312),    module, EqualDivision::PITCH_OUTPUT + 8));

	// addChild(new IntervalDisplay(Rect(8.5, 98, 74, 18), module));
}

Model* modelMIDIOverAudioToCV = createModel<MIDIOverAudioToCV, MIDIOverAudioToCVWidget>("MIDIOverAudioToCV");
