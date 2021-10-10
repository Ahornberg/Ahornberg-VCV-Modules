#include "TapeRecorderMixerWidget.hpp"

VolumeDisplay::VolumeDisplay(Rect box, TapeRecorderMixer* tapeRecorderMixer) : Display(box) {
	this->tapeRecorderMixer = tapeRecorderMixer;
	vuMeterFontPath = asset::plugin(pluginInstance, FONT_VU_METER);
	channelNumber = 0;
	trackName = "";
	vuMeter = 0;
}

void VolumeDisplay::drawText(const DrawArgs& disp) {
	nvgFillColor(disp.vg, textColorLight);
	if (tapeRecorderMixer) {
		channelNumber = tapeRecorderMixer->channelNumber;
		vuMeter = std::min(tapeRecorderMixer->vuMeter * 4.8f, 23.f);
		if (tapeRecorderMixer->params[TapeRecorderMixer::RECORD_PARAM].getValue()) {
			nvgFillColor(disp.vg, textColorRed);
		}
	}
	nvgFontSize(disp.vg, 8);
	Vec textPos = Vec(45.25, 28);
	if (trackName.size() <= 5) {
		std::string text = std::to_string(channelNumber);
		if (!channelNumber) {
			nvgText(disp.vg, textPos.x, textPos.y, "-", NULL);
		} else if (channelNumber < 10) {
			nvgText(disp.vg, textPos.x, textPos.y, (std::to_string(channelNumber)).c_str(), NULL);
		} else {
			nvgText(disp.vg, textPos.x, textPos.y, (std::to_string(channelNumber - 10)).c_str(), NULL);
			textPos = Vec(40.75, 28);
			nvgText(disp.vg, textPos.x, textPos.y, "1", NULL);
		}
	}
	for (auto i = 0; i < std::min(7, (int) trackName.size()); ++i) {
		textPos = Vec(3.25 + i * 7, 28);
		nvgText(disp.vg, textPos.x, textPos.y, trackName.substr(i, 1).c_str(), NULL);
	}
	std::shared_ptr<Font> vuMeterFont = APP->window->loadFont(vuMeterFontPath);
	if (vuMeterFont) {
		nvgFontFaceId(disp.vg, vuMeterFont->handle);
		nvgFontSize(disp.vg, 51.25);
		nvgFillColor(disp.vg, textColorDark);
		textPos = Vec(2, 25.5);
		for (auto i = 0; i < 24; ++i) {
			if (i > 15) {
				nvgFillColor(disp.vg, nvgRGB(0xff, 0x00, 0x00));
			} else if (i > 7) {
				nvgFillColor(disp.vg, textColorLight);
			} else {
				nvgFillColor(disp.vg, textColorDark);
			}
			if (i == vuMeter) {
				nvgFillColor(disp.vg, COLOR_WHITE);
			}
			nvgText(disp.vg, textPos.x, textPos.y, std::string(1, 97 + i).c_str(), NULL);
		}
		
		nvgFillColor(disp.vg, COLOR_WHITE);
		nvgText(disp.vg, textPos.x, textPos.y, std::string(1, 65 + vuMeter).c_str(), NULL);
	}
}

RoundSwitchMediumLink::RoundSwitchMediumLink() {
	addFrame("res/switches/RoundMedium_unlinked.svg");
	addFrame("res/switches/RoundMedium_linked_to_right.svg");
	addFrame("res/switches/RoundMedium_unlinked_linked_from_left.svg");
}

void RoundSwitchMediumLink::step() {
	// BasicRoundSwitch::step();
	if (tapeRecorderMixer && !tapeRecorderMixer->params[TapeRecorderMixer::LINK_PARAM].getValue()) {
		if (tapeRecorderMixer->linked) {
			sw->setSvg(frames[2]);
			fb->dirty = true;
		} else {
			sw->setSvg(frames[0]);
			fb->dirty = true;
			
		}
	}
	BasicRoundSwitch::step();
}

void RoundSwitchMediumLink::onChange(const event::Change& e) {
	ParamQuantity* paramQuantity = getParamQuantity();
	if (!frames.empty() && paramQuantity) {
		int index = (int) std::round(paramQuantity->getValue() - paramQuantity->getMinValue());
		index = math::clamp(index, 0, (int) frames.size() - 1);
		if (tapeRecorderMixer && !index) {
			if (tapeRecorderMixer->linked) {
				sw->setSvg(frames[2]);
				fb->dirty = true;
			} else {
				sw->setSvg(frames[0]);
				fb->dirty = true;
				
			}
		} else {
			sw->setSvg(frames[index]);
			fb->dirty = true;

		}
	}
	ParamWidget::onChange(e);
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
	setWidthInHP(4);
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, SCREW_BOTTOM_LEFT_INDENTED, NO_SCREW_BOTTOM_RIGHT);

	addInput(createInputCentered<InPort>(Vec(31, 14), module,  TapeRecorderMixer::AUDIO_INPUT));
	addInput(createInputCentered<InPort>(Vec(14, 71), module,  TapeRecorderMixer::AUDIO_FX_RETURN));
	addInput(createInputCentered<InPortSmall>(Vec(12, 338), module,  TapeRecorderMixer::AUDIO_CHAIN_TO_TAPE_INPUT));
	addInput(createInputCentered<InPortSmall>(Vec(33, 359), module,  TapeRecorderMixer::AUDIO_CHAIN_FROM_TAPE_INPUT));

	addOutput(createOutputCentered<OutPort>(Vec(14, 32), module,  TapeRecorderMixer::AUDIO_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(31, 53), module,  TapeRecorderMixer::AUDIO_FX_SEND));
	addOutput(createOutputCentered<OutPortSmall>(Vec(33, 338), module,  TapeRecorderMixer::AUDIO_CHAIN_TO_TAPE_OUTPUT));
	addOutput(createOutputCentered<OutPortSmall>(Vec(12, 359), module,  TapeRecorderMixer::AUDIO_CHAIN_FROM_TAPE_OUTPUT));
	
	volumeDisplay = new VolumeDisplay(Rect(3, 82, 54, 32), module);
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
	addInput(createInputCentered<InPortSmall>(Vec(33, 307), module, TapeRecorderMixer::CV_INPUT_VOLUME_INPUT));

	RoundSwitchMediumLink* roundSwitchMediumLink = dynamic_cast<RoundSwitchMediumLink*>(createParamCentered<RoundSwitchMediumLink>(Vec(12, 316), module, TapeRecorderMixer::LINK_PARAM));
	roundSwitchMediumLink->tapeRecorderMixer = module;
	addParam(roundSwitchMediumLink);


}

void TapeRecorderMixerWidget::appendContextMenu(Menu* menu) {
	TapeRecorderMixer* tapeRecorderMixer = dynamic_cast<TapeRecorderMixer*>(this->module);
	menu->addChild(new MenuSeparator);
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
