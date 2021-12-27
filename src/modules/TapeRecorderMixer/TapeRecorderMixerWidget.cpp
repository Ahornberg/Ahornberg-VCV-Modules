#include "TapeRecorderMixerWidget.hpp"

VolumeDisplay::VolumeDisplay(Rect box, TapeRecorderMixer* tapeRecorderMixer) : Display(box) {
	this->tapeRecorderMixer = tapeRecorderMixer;
	vuMeterFontPath = asset::plugin(pluginInstance, FONT_VU_METER);
	channelNumber = 0;
	trackName = TapeRecorderMixer::INIT_TRACK_NAME;
	vuMeter = 0;
}

void VolumeDisplay::drawText(const DrawArgs& disp) {
	nvgFillColor(disp.vg, textColorLight);
	if (tapeRecorderMixer) {
		channelNumber = tapeRecorderMixer->channelNumber;
		vuMeter = std::min(tapeRecorderMixer->vuMeter * 3.3f, 23.f);
		if (tapeRecorderMixer->params[TapeRecorderMixer::RECORD_PARAM].getValue()) {
			nvgFillColor(disp.vg, textColorRed);
		}
		trackName = tapeRecorderMixer->trackName;
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
			if (tapeRecorderMixer && tapeRecorderMixer->params[TapeRecorderMixer::MUTE_PARAM].getValue()) {
				nvgFillColor(disp.vg, COLOR_GREY);
			} else {
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
			}
			nvgText(disp.vg, textPos.x, textPos.y, std::string(1, 97 + i).c_str(), NULL);
		}
		if (tapeRecorderMixer && tapeRecorderMixer->params[TapeRecorderMixer::MUTE_PARAM].getValue()) {
			nvgFillColor(disp.vg, COLOR_GREY);
		} else {
			nvgFillColor(disp.vg, COLOR_WHITE);
		}
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

TrackNameMenuItem::TrackNameMenuItem(TapeRecorderMixer* tapeRecorderMixer) {
	this->tapeRecorderMixer = tapeRecorderMixer;
	if (tapeRecorderMixer) {
		text = tapeRecorderMixer->trackName;
	}
}

void TrackNameMenuItem::onChange(const event::Change& e) {
	TextFieldMenuItem::onChange(e);
	if (tapeRecorderMixer) {
		tapeRecorderMixer->trackName = text;
	}
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
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);

	addInput(createInputCentered<InPort>(Vec(31 + 1, 14 + 1), module,  TapeRecorderMixer::AUDIO_INPUT));
	addInput(createInputCentered<InPort>(Vec(15 + 14 - 1, 71 - 5), module,  TapeRecorderMixer::AUDIO_FX_RETURN));
	addInput(createInputCentered<InPortSmall>(Vec(12, 335), module,  TapeRecorderMixer::AUDIO_CHAIN_TO_TAPE_INPUT));
	addInput(createInputCentered<InPortSmall>(Vec(15 + 33, 357), module,  TapeRecorderMixer::AUDIO_CHAIN_FROM_TAPE_INPUT));

	addParam(createParamCentered<KnobScrewSnap>(Vec(    50, 71), module, TapeRecorderMixer::SAMPLE_DELAY_PARAM));

	addOutput(createOutputCentered<OutPort>(Vec(14 + 1, 32 + 1), module,  TapeRecorderMixer::AUDIO_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15 + 31 - 1, 53 - 5), module,  TapeRecorderMixer::AUDIO_FX_SEND));
	addOutput(createOutputCentered<OutPortSmall>(Vec(15 + 33, 335), module,  TapeRecorderMixer::AUDIO_CHAIN_TO_TAPE_OUTPUT));
	addOutput(createOutputCentered<OutPortSmall>(Vec(12, 357), module,  TapeRecorderMixer::AUDIO_CHAIN_FROM_TAPE_OUTPUT));
	
	volumeDisplay = new VolumeDisplay(Rect(3, 82, 54, 32), module);
	addChild(volumeDisplay);

	addParam(createParamCentered<RoundSwitchMediumRed>(Vec(5 + 12, 136 + 4), module, TapeRecorderMixer::RECORD_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(5 + 12, 157 + 4), module, TapeRecorderMixer::CV_RECORD_INPUT));
	addParam(createParamCentered<RoundSwitchMedium>(Vec(10 + 33, 143 + 4), module, TapeRecorderMixer::BYPASS_INSERT_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(10 + 33, 164 + 4), module, TapeRecorderMixer::CV_BYPASS_INSERT_INPUT));

	addParam(createParamCentered<KnobTiny>(Vec(5 + 12, 189 + 6), module, TapeRecorderMixer::TAPE_DUCKING_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(5 + 12, 210 + 6), module, TapeRecorderMixer::CV_TAPE_DUCKING_INPUT));
	addParam(createParamCentered<KnobTiny>(Vec(10 + 33, 196 + 6), module, TapeRecorderMixer::TAPE_ERASE_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(10 + 33, 217 + 6), module, TapeRecorderMixer::CV_TAPE_ERASE_INPUT));
	
	addParam(createParamCentered<RoundSwitchMedium>(Vec(5 + 12, 242 + 8), module, TapeRecorderMixer::SOLO_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(5 + 12, 263 + 8), module, TapeRecorderMixer::CV_SOLO_INPUT));
	addParam(createParamCentered<RoundSwitchMedium>(Vec(10 + 33, 249 + 8), module, TapeRecorderMixer::MUTE_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(10 + 33, 270 + 8), module, TapeRecorderMixer::CV_MUTE_INPUT));


	addParam(createParamCentered<KnobSmall>(Vec(5 + 16, 289 + 10), module, TapeRecorderMixer::INPUT_VOLUME_PARAM));
	addParam(createParamCentered<RoundSwitch>(Vec(5 + 16, 289 + 10), module, TapeRecorderMixer::INPUT_MUTE_PARAM));
	addInput(createInputCentered<InPortSmall>(Vec(10 + 33, 302 + 10), module, TapeRecorderMixer::CV_INPUT_VOLUME_INPUT));

	RoundSwitchMediumLink* roundSwitchMediumLink = dynamic_cast<RoundSwitchMediumLink*>(createParamCentered<RoundSwitchMediumLink>(Vec(30, 346), module, TapeRecorderMixer::LINK_PARAM));
	roundSwitchMediumLink->tapeRecorderMixer = module;
	roundSwitchMediumLink->hide();
	addParam(roundSwitchMediumLink);


}

void TapeRecorderMixerWidget::contextMenu(Menu* menu) {
	TapeRecorderMixer* tapeRecorderMixer = dynamic_cast<TapeRecorderMixer*>(this->module);
	menu->addChild(new TrackNameMenuItem(tapeRecorderMixer));
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

Model* modelTapeRecorderMixer = createModel<TapeRecorderMixer, TapeRecorderMixerWidget>("TapeRecorderMixer");
