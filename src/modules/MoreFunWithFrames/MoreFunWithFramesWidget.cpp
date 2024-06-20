#include "MoreFunWithFramesWidget.hpp"

MoreFunWithFramesWidget::MoreFunWithFramesWidget(MoreFunWithFrames* module) {
	setModule(module);
	setPanel("res/MoreFunWithFrames.svg");
	setWidthInHP(6);

	for (auto i = 0; i < MoreFunWithFrames::NUM_VALUE_IO; ++i) {
		addInput(createInput<InPort>(Vec(4, 79 + 40 * i), module, MoreFunWithFrames::VALUE_INPUT + i));
		addParam(createParam<RoundSwitchLinearJump>(Vec(33, 79 + 40 * i), module, MoreFunWithFrames::VALUE_CONVERT_MODE + i));
		addOutput(createOutput<OutPort>(Vec(62, 79 + 40 * i), module, MoreFunWithFrames::VALUE_OUTPUT + i));
	}

	addInput(createInput<InPort>(Vec(4, 256),    module, MoreFunWithFrames::NOTE_INPUT));
	addInput(createInput<InPort>(Vec(4, 296),    module, MoreFunWithFrames::GATE_INPUT));
	addParam(createParam<KnobSmallSnapFast>(Vec(31, 274), module, MoreFunWithFrames::MAX_CHORD_SIZE_PARAM));
	addOutput(createOutput<OutPort>(Vec(62, 256),    module, MoreFunWithFrames::NOTE_OUTPUT));
	addOutput(createOutput<OutPort>(Vec(62, 296),    module, MoreFunWithFrames::GATE_OUTPUT));
	addInput(createInput<InPort>(Vec(4, 327),    module, MoreFunWithFrames::CLOCK_INPUT));
}

Model* modelMoreFunWithFrames = createModel<MoreFunWithFrames, MoreFunWithFramesWidget>("MoreFunWithFrames");
