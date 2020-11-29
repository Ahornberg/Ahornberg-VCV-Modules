#include "FunWithFramesWidget.hpp"

FunWithFramesWidget::FunWithFramesWidget(FunWithFrames* module) {
	setModule(module);
	setPanel("res/FunWithFrames.svg");
	setSize(Vec(90, 380));
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, NO_SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);

	for (auto i = 0; i < 4; ++i) {
		addInput(createInput<InPort>(Vec(4, 79 + 40 * i), module, FunWithFrames::VALUE_INPUT + i));
		addParam(createParam<RoundSwitchLinearJump>(Vec(33, 79 + 40 * i), module, FunWithFrames::VALUE_CONVERT_MODE + i));
		addOutput(createOutput<OutPort>(Vec(62, 79 + 40 * i), module, FunWithFrames::VALUE_OUTPUT + i));
	}

	addInput(createInput<InPort>(Vec(4, 268),    module, FunWithFrames::NOTE_INPUT));
	addInput(createInput<InPort>(Vec(4, 308),    module, FunWithFrames::GATE_INPUT));
	addParam(createParam<KnobSmallSnap>(Vec(31, 286), module, FunWithFrames::CHORD_SIZE_PARAM));
	addOutput(createOutput<OutPort>(Vec(62, 268),    module, FunWithFrames::NOTE_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(62, 308),    module, FunWithFrames::GATE_OUTPUT));
}

Model* modelFunWithFrames = createModel<FunWithFrames, FunWithFramesWidget>("FunWithFrames");
