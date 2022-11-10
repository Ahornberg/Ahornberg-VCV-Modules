#include "CVFreqShiftWidget.hpp"

void MultiplierKnob::onChange(const event::Change& e) {
	KnobSmallSnap::onChange(e);
	if (module) {
		module->paramQuantities[CVFreqShift::FREQUENCY_PARAM]->displayMultiplier = CVFreqShift::FREQUENCY_RANGES[(int) module->params[CVFreqShift::FREQUENCY_RANGE_PARAM].getValue()].rangeInHz;
	}
}

CVFreqShiftWidget::CVFreqShiftWidget(CVFreqShift* module) {
	setModule(module);
	setPanel("res/CVFreqShift.svg");
	setWidthInHP(6);

	MultiplierKnob* multiplierKnob = dynamic_cast<MultiplierKnob*>(createParam<MultiplierKnob>(Vec(10, 50), module, CVFreqShift::FREQUENCY_RANGE_PARAM));
	multiplierKnob->module = module;
	addParam(multiplierKnob);
	addParam(createParam<KnobBig>(Vec(      46.5,  42), module, CVFreqShift::FREQUENCY_PARAM));
	addParam(createParam<KnobSmall>(Vec(    50,   105), module, CVFreqShift::FREQUENCY_MODULATION_AMOUNT_PARAM));

	addInput(createInput<InPort>(Vec( 12.5, 107.5),    module, CVFreqShift::FREQUENCY_MODULATION_INPUT));

	addInput(createInput<InPort>(Vec( 4, 167),    module, CVFreqShift::PITCH_INPUT + 0));
	addInput(createInput<InPort>(Vec(33, 167),    module, CVFreqShift::PITCH_INPUT + 1));
	addInput(createInput<InPort>(Vec(62, 167),    module, CVFreqShift::PITCH_INPUT + 2));
	addInput(createInput<InPort>(Vec( 4, 233.5),  module, CVFreqShift::PITCH_INPUT + 3));
	addInput(createInput<InPort>(Vec(33, 229.75), module, CVFreqShift::PITCH_INPUT + 4));
	addInput(createInput<InPort>(Vec(62, 226),    module, CVFreqShift::PITCH_INPUT + 5));
	addInput(createInput<InPort>(Vec( 4, 300),    module, CVFreqShift::PITCH_INPUT + 6));
	addInput(createInput<InPort>(Vec(33, 292.5),  module, CVFreqShift::PITCH_INPUT + 7));
	addInput(createInput<InPort>(Vec(62, 285),    module, CVFreqShift::PITCH_INPUT + 8));

	addOutput(createOutput<OutPort>(Vec( 4, 194),    module, CVFreqShift::PITCH_OUTPUT + 0));
	addOutput(createOutput<OutPort>(Vec(33, 194),    module, CVFreqShift::PITCH_OUTPUT + 1));
	addOutput(createOutput<OutPort>(Vec(62, 194),    module, CVFreqShift::PITCH_OUTPUT + 2));
	addOutput(createOutput<OutPort>(Vec( 4, 260.5),  module, CVFreqShift::PITCH_OUTPUT + 3));
	addOutput(createOutput<OutPort>(Vec(33, 256.75), module, CVFreqShift::PITCH_OUTPUT + 4));
	addOutput(createOutput<OutPort>(Vec(62, 253),    module, CVFreqShift::PITCH_OUTPUT + 5));
	addOutput(createOutput<OutPort>(Vec( 4, 327),    module, CVFreqShift::PITCH_OUTPUT + 6));
	addOutput(createOutput<OutPort>(Vec(33, 319.5),  module, CVFreqShift::PITCH_OUTPUT + 7));
	addOutput(createOutput<OutPort>(Vec(62, 312),    module, CVFreqShift::PITCH_OUTPUT + 8));

	// addChild(new IntervalDisplay(Rect(8.5, 98, 74, 18), module));
}

Model* modelCVFreqShift = createModel<CVFreqShift, CVFreqShiftWidget>("CVFreqShift");
