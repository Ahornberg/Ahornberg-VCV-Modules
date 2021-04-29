#include "TapeRecorderMixerWidget.hpp"

VolumeDisplay::VolumeDisplay(Rect box, TapeRecorderMixer* tapeRecorderMixer) : Display(box) {
	this->tapeRecorderMixer = tapeRecorderMixer;
	// tapePosition = 0;
	// beatsPerBar = 0;
	// loopMode = 0;
	// loopStart = 0;
	// loopEnd = 0;
	// loopStartConnected = false;
	// loopEndConnected = false;
	// // loopStartOnTapePosition = false;
	// // loopEndOnTapePosition = false;
	// playStatus = false;
	// cueStatus = false;
	// playForwardStatus = false;
	// cueForwardStatus = false;
}

void VolumeDisplay::drawText(const DrawArgs& disp) {
	if (tapeRecorderMixer) {
		// loopMode = tapeRecorder->params[TapeRecorder::LOOP_MODE_PARAM].getValue();
		// loopStartConnected = tapeRecorder->inputs[TapeRecorder::LOOP_START_INPUT].isConnected();
		// loopStart = tapeRecorder->loopStart;
		// loopEndConnected = tapeRecorder->inputs[TapeRecorder::LOOP_END_INPUT].isConnected();
		// // loopEndConnected = (tapeRecorder->inputs[TapeRecorder::LOOP_INPUT].isConnected() && tapeRecorder->inputs[TapeRecorder::LOOP_INPUT].getChannels() > 1);
		// loopEnd = tapeRecorder->loopEnd;
		// beatsPerBar = tapeRecorder->params[TapeRecorder::BEATS_PER_BAR_PARAM].getValue();
		// tapePosition = tapeRecorder->tapePosition;
		// playForwardStatus = tapeRecorder->playForwardStatus;
		// playStatus = tapeRecorder->playStatus;
		// cueForwardStatus = tapeRecorder->cueForwardStatus;
		// cueStatus = tapeRecorder->cueStatus;
		// // loopStartOnTapePosition = (tapeRecorder->params[TapeRecorder::LOOP_START_BUTTON_PARAM].getValue() == 1.0f);
		// // loopEndOnTapePosition = (tapeRecorder->params[TapeRecorder::LOOP_END_BUTTON_PARAM].getValue() == 1.0f);
	}
	// Vec textPos = Vec(4, 14);
	// nvgFillColor(disp.vg, textColorLight);
	// int bar = (int) tapePosition;
	// int beat = (int) ((tapePosition - bar) * beatsPerBar);
	// if (bar > 9999) {
		// bar = 9999;
	// }
	// std::string text = std::to_string(bar);
	// if (bar < 1000) {
		// text = " " + text;
	// } 
	// if (bar < 100) {
		// text = " " + text;
	// } 
	// if (bar < 10) {
		// text = " " + text;
	// }
	// if (beat < 10) {
		// text += "/ " + std::to_string(beat);
	// } else {
		// text += "/" + std::to_string(beat);
	// }
	// nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
	
	// if (loopStart != loopEnd) {
		// textPos = Vec(33, 28);
		// if (loopMode) {
			// nvgText(disp.vg, textPos.x, textPos.y, "P", NULL);
		// } else {
			// nvgText(disp.vg, textPos.x, textPos.y, "C", NULL);
		// }
	// }
	// // if (cueStatus) {
		// // if (cueForwardStatus) {
			// // textPos = Vec(33, 28);
			// // nvgText(disp.vg, textPos.x, textPos.y, ")", NULL);
			// // textPos = Vec(35.5, 28);
			// // nvgText(disp.vg, textPos.x, textPos.y, ")", NULL);
		// // } else {
			// // textPos = Vec(33, 28);
			// // nvgText(disp.vg, textPos.x, textPos.y, "(", NULL);
			// // textPos = Vec(35.5, 28);
			// // nvgText(disp.vg, textPos.x, textPos.y, "(", NULL);
		// // }
	// // } else if (playStatus) {
		// // // if ((playForwardStatus && position < loopEnd) || (!playForwardStatus && position < loopStart)) {
		// // if (playForwardStatus) {
			// // textPos = Vec(33, 28);
			// // nvgText(disp.vg, textPos.x, textPos.y, "-", NULL);
			// // textPos = Vec(35.5, 28);
			// // nvgText(disp.vg, textPos.x, textPos.y, ")", NULL);
		// // } else {
			// // textPos = Vec(33, 28);
			// // nvgText(disp.vg, textPos.x, textPos.y, "(", NULL);
			// // textPos = Vec(35.5, 28);
			// // nvgText(disp.vg, textPos.x, textPos.y, "-", NULL);
		// // }
	// // }
	
	// // textPos = Vec(34, 14);
	// // if (speedConnected) {
		// // nvgFillColor(disp.vg, textColorDark);
	// // } else {
		// // nvgFillColor(disp.vg, textColorLight);
	// // }
	// // nvgText(disp.vg, textPos.x, textPos.y, (std::to_string(tempo)).c_str(), NULL);
	
	// nvgFontSize(disp.vg, 8);

	// textPos = Vec(4, 28);
	// // if (loopStartOnTapePosition) {
		// // nvgFillColor(disp.vg, textColorRed);
	// // } else 
	// if (loopStartConnected) {
		// nvgFillColor(disp.vg, textColorDark);
	// } else {
		// nvgFillColor(disp.vg, textColorLight);
	// }
	// text = std::to_string(loopStart);
	// if (loopStart < 100) {
		// text = " " + text;
	// } 
	// if (loopStart < 10) {
		// text = " " + text;
	// }
	// nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
	
	// textPos = Vec(46, 28);
	// // if (loopEndOnTapePosition) {
		// // nvgFillColor(disp.vg, textColorRed);
	// // } else 
	// if (loopEndConnected) {
		// nvgFillColor(disp.vg, textColorDark);
	// } else {
		// nvgFillColor(disp.vg, textColorLight);
	// }
	// text = std::to_string(loopEnd);
	// if (loopEnd < 100) {
		// text = " " + text;
	// } 
	// if (loopEnd < 10) {
		// text = " " + text;
	// }
	// nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
}

TapeRecorderMixerWidget::TapeRecorderMixerWidget(TapeRecorderMixer* module) {
	setModule(module);
	setPanel("res/TapeRecorderMixer.svg");
	setSize(Vec(45, 380));
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, SCREW_BOTTOM_LEFT_INDENTED, NO_SCREW_BOTTOM_RIGHT);

	addInput(createInputCentered<InPort>(Vec(31, 14), module,  TapeRecorderMixer::AUDIO_INPUT));
	addInput(createInputCentered<InPort>(Vec(14, 71), module,  TapeRecorderMixer::AUDIO_FX_RETURN));
	addInput(createInputCentered<InPortSmall>(Vec(12, 338), module,  TapeRecorderMixer::AUDIO_CHAIN_LEFT_INPUT));
	addInput(createInputCentered<InPortSmall>(Vec(33, 359), module,  TapeRecorderMixer::AUDIO_CHAIN_RIGHT_INPUT));

	addOutput(createOutputCentered<OutPort>(Vec(14, 32), module,  TapeRecorderMixer::AUDIO_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(31, 53), module,  TapeRecorderMixer::AUDIO_FX_SEND));
	addOutput(createOutputCentered<OutPortSmall>(Vec(33, 338), module,  TapeRecorderMixer::AUDIO_CHAIN_RIGHT_OUTPUT));
	addOutput(createOutputCentered<OutPortSmall>(Vec(12, 359), module,  TapeRecorderMixer::AUDIO_CHAIN_LEFT_OUTPUT));
	
	addChild(new VolumeDisplay(Rect(3, 85, 39, 29), module));

	addParam(createParamCentered<RoundSwitchMediumRed>(Vec(12, 136), module, TapeRecorderMixer::RECORD_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(12, 157), module, TapeRecorderMixer::CV_RECORD_INPUT));
	addParam(createParamCentered<RoundSwitchMedium>(Vec(33, 143), module, TapeRecorderMixer::BYPASS_CHAIN_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(33, 164), module, TapeRecorderMixer::CV_BYPASS_CHAIN_INPUT));

	addParam(createParamCentered<KnobTiny>(Vec(12, 189), module, TapeRecorderMixer::TAPE_DUCKING_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(12, 210), module, TapeRecorderMixer::CV_TAPE_DUCKING_INPUT));
	addParam(createParamCentered<KnobTiny>(Vec(33, 196), module, TapeRecorderMixer::TAPE_ERASE_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(33, 217), module, TapeRecorderMixer::CV_TAPE_ERASE_INPUT));
	
	addParam(createParamCentered<RoundSwitchMedium>(Vec(12, 242), module, TapeRecorderMixer::SOLO_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(12, 263), module, TapeRecorderMixer::CV_SOLO_INPUT));
	addParam(createParamCentered<RoundSwitchMedium>(Vec(33, 249), module, TapeRecorderMixer::MUTE_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(33, 270), module, TapeRecorderMixer::CV_MUTE_INPUT));


	addParam(createParamCentered<KnobSmall>(Vec(16, 289), module, TapeRecorderMixer::INPUT_VOLUME_PARAM));
	addParam(createParamCentered<RoundSwitch>(Vec(16, 289), module, TapeRecorderMixer::INPUT_MUTE_PARAM));
	// addParam(createParamCentered<RoundSwitch>(Vec(12, 189), module, TapeRecorderMixer::INPUT_MUTE_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(33, 307), module, TapeRecorderMixer::CV_INPUT_VOLUME_INPUT));

	addParam(createParamCentered<RoundSwitchMediumLink>(Vec(12, 316), module, TapeRecorderMixer::LINK_PARAM));


}

Model* modelTapeRecorderMixer = createModel<TapeRecorderMixer, TapeRecorderMixerWidget>("TapeRecorderMixer");
