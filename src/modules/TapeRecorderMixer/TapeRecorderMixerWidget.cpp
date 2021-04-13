#include "TapeRecorderMixerWidget.hpp"

TapeRecorderMixerWidget::TapeRecorderMixerWidget(TapeRecorderMixer* module) {
	setModule(module);
	setPanel("res/TapeRecorderMixer.svg");
	setSize(Vec(45, 380));
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, SCREW_BOTTOM_LEFT_INDENTED, NO_SCREW_BOTTOM_RIGHT);

	addParam(createParamCentered<KnobSmall>(Vec(22.5, 124), module, TapeRecorderMixer::TAPE_VOLUME_PARAM));

	addInput(createInputCentered<InPort>(Vec(31, 14), module,  TapeRecorderMixer::AUDIO_INPUT));
	addInput(createInputCentered<InPort>(Vec(14, 71), module,  TapeRecorderMixer::AUDIO_FX_RETURN));
	addInput(createInputCentered<InPortSmall>(Vec(33, 242), module,  TapeRecorderMixer::CV_VOLUME_SLIDER_INPUT));
	addInput(createInputCentered<InPortSmall>(Vec(12, 338), module,  TapeRecorderMixer::AUDIO_CHAIN_LEFT_INPUT));
	addInput(createInputCentered<InPortSmall>(Vec(33, 359), module,  TapeRecorderMixer::AUDIO_CHAIN_RIGHT_INPUT));

	addOutput(createOutputCentered<OutPort>(Vec(14, 32), module,  TapeRecorderMixer::AUDIO_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(31, 53), module,  TapeRecorderMixer::AUDIO_FX_SEND));
	addOutput(createOutputCentered<OutPortSmall>(Vec(33, 338), module,  TapeRecorderMixer::AUDIO_CHAIN_RIGHT_OUTPUT));
	addOutput(createOutputCentered<OutPortSmall>(Vec(12, 359), module,  TapeRecorderMixer::AUDIO_CHAIN_LEFT_OUTPUT));
}

Model* modelTapeRecorderMixer = createModel<TapeRecorderMixer, TapeRecorderMixerWidget>("TapeRecorderMixer");
