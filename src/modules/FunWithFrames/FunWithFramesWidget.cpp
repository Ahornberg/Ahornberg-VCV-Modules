#include "FunWithFramesWidget.hpp"

FunWithFramesWidget::FunWithFramesWidget(FunWithFrames* module) {
	setModule(module);
	setPanel("res/FunWithFrames.svg");
	setWidthInHP(6);

	for (auto i = 0; i < FunWithFrames::NUM_VALUE_IO; ++i) {
		addInput(createInput<InPort>(Vec(4, 79 + 40 * i), module, FunWithFrames::VALUE_INPUT + i));
		addParam(createParam<RoundSwitchLinearJump>(Vec(33, 79 + 40 * i), module, FunWithFrames::VALUE_CONVERT_MODE + i));
		addOutput(createOutput<OutPort>(Vec(62, 79 + 40 * i), module, FunWithFrames::VALUE_OUTPUT + i));
	}

	addInput(createInput<InPort>(Vec(4, 256),    module, FunWithFrames::NOTE_INPUT));
	addInput(createInput<InPort>(Vec(4, 296),    module, FunWithFrames::GATE_INPUT));
	addParam(createParam<KnobSmallSnapFast>(Vec(31, 274), module, FunWithFrames::MAX_CHORD_SIZE_PARAM));
	addOutput(createOutput<OutPort>(Vec(62, 256),    module, FunWithFrames::NOTE_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(62, 296),    module, FunWithFrames::GATE_OUTPUT));
	addInput(createInput<InPort>(Vec(4, 327),    module, FunWithFrames::CLOCK_INPUT));
}

Model* modelFunWithFrames = createModel<FunWithFrames, FunWithFramesWidget>("FunWithFrames");
