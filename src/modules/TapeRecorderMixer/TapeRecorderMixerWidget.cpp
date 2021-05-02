#include "TapeRecorderMixerWidget.hpp"

VolumeDisplay::VolumeDisplay(Rect box, TapeRecorderMixer* tapeRecorderMixer) : Display(box) {
	this->tapeRecorderMixer = tapeRecorderMixer;
	channelNumber = 0;
	trackName = "";
}

void VolumeDisplay::drawText(const DrawArgs& disp) {
	if (tapeRecorderMixer) {
		channelNumber = tapeRecorderMixer->channelNumber;
	}
	Vec textPos = Vec(30.5, 25);
	nvgFillColor(disp.vg, textColorLight);
	nvgFontSize(disp.vg, 8);
	std::string text = std::to_string(channelNumber);
	
	if (!channelNumber) {
		nvgText(disp.vg, textPos.x, textPos.y, "-", NULL);
	} else if (channelNumber < 10) {
		nvgText(disp.vg, textPos.x, textPos.y, (std::to_string(channelNumber)).c_str(), NULL);
	} else {
		nvgText(disp.vg, textPos.x, textPos.y, (std::to_string(channelNumber - 10)).c_str(), NULL);
		textPos = Vec(26, 25);
		nvgText(disp.vg, textPos.x, textPos.y, "1", NULL);
	}
	
	for (auto i = 0; i < std::min(3, (int) trackName.size()); ++i) {
		textPos = Vec(3 + i * 7, 25);
		nvgText(disp.vg, textPos.x, textPos.y, trackName.substr(i, 1).c_str(), NULL);
	}
	
	
		// textPos = Vec(10, 25);
		// nvgText(disp.vg, textPos.x, textPos.y, trackName.substr(1, 1).c_str(), NULL);
		// textPos = Vec(17, 25);
		// nvgText(disp.vg, textPos.x, textPos.y, trackName.substr(2, 1).c_str(), NULL);
	
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

TapeRecorderMixerMenuItem::TapeRecorderMixerMenuItem(TapeRecorderMixer* tapeRecorderMixer, TapeRecorderMixerWidget* tapeRecorderMixerWidget) {
	this->tapeRecorderMixer = tapeRecorderMixer;
	this->tapeRecorderMixerWidget = tapeRecorderMixerWidget;
}

TrackNameMenuItem::TrackNameMenuItem(VolumeDisplay* volumeDisplay) {
	this->volumeDisplay = volumeDisplay;
	text = volumeDisplay->trackName;
}

void TrackNameMenuItem::onChange(const event::Change& e) {
	TextFieldMenuItem::onChange(e);
	volumeDisplay->trackName = text;
}

ChangeInputMuteModeMenuItem::ChangeInputMuteModeMenuItem(TapeRecorderMixer* tapeRecorderMixer, TapeRecorderMixerWidget* tapeRecorderMixerWidget) : TapeRecorderMixerMenuItem(tapeRecorderMixer, tapeRecorderMixerWidget) {
	text = "Enable Input Mute";
	if (tapeRecorderMixer && tapeRecorderMixer->params[TapeRecorderMixer::INPUT_MUTE_ENABLED_PARAM].getValue()) {
		text = "Disable Input Mute";
	}
}

void ChangeInputMuteModeMenuItem::onAction(const event::Action& e) {
	if (tapeRecorderMixer) {
		toggleParamValue(tapeRecorderMixer->params[TapeRecorderMixer::INPUT_MUTE_ENABLED_PARAM]);
	}
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
	
	volumeDisplay = new VolumeDisplay(Rect(3, 85, 39, 29), module);
	addChild(volumeDisplay);

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

void TapeRecorderMixerWidget::appendContextMenu(Menu* menu) {
	TapeRecorderMixer* tapeRecorderMixer = dynamic_cast<TapeRecorderMixer*>(this->module);
	menu->addChild(new MenuEntry);
	menu->addChild(new TrackNameMenuItem(volumeDisplay));
	menu->addChild(new ChangeInputMuteModeMenuItem(tapeRecorderMixer, this));
}

void TapeRecorderMixerWidget::step() {
	ModuleWidget::step();
	if (module) {
		ParamWidget* inputMuteParamWidget = getParam(TapeRecorderMixer::INPUT_MUTE_PARAM);
		if (module->params[TapeRecorderMixer::INPUT_MUTE_ENABLED_PARAM].getValue()) {
			inputMuteParamWidget->show();
		} else {
			inputMuteParamWidget->hide();
		}
	}
}

json_t* TapeRecorderMixerWidget::toJson() {
	json_t* rootJ = ModuleWidget::toJson();
	
	json_object_set_new(rootJ, "track-name", json_string(volumeDisplay->trackName.c_str()));
	return rootJ;
}

void TapeRecorderMixerWidget::fromJson(json_t* rootJ) {
	ModuleWidget::fromJson(rootJ);
	
	json_t* trackNameJ = json_object_get(rootJ, "track-name");
	if (trackNameJ) {
		volumeDisplay->trackName = json_string_value(trackNameJ);
	}
}

Model* modelTapeRecorderMixer = createModel<TapeRecorderMixer, TapeRecorderMixerWidget>("TapeRecorderMixer");
