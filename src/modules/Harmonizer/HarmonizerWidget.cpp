#include "HarmonizerWidget.hpp"

HarmonicsDisplay::HarmonicsDisplay(Rect box, Harmonizer* harmonizer, int channel) : Display(box) {
	this->harmonizer = harmonizer;
	this->channel = channel;
	harmonics = 1;
	subharmonics = 1;
}

void HarmonicsDisplay::drawText(const DrawArgs& disp) {
	if (harmonizer) {
		harmonics = (int) harmonizer->params[Harmonizer::PITCH_HARMONICS_PARAM + channel].getValue();
		subharmonics = (int) harmonizer->params[Harmonizer::PITCH_SUBHARMONICS_PARAM + channel].getValue();
	}
	Vec textPos = Vec(4, 14);
	nvgFillColor(disp.vg, textColorLight);
	std::string text = std::to_string(harmonics);
	if (harmonics < 10) {
		text = " " + text;
	}
	if (subharmonics < 10) {
		text += "/ " + std::to_string(subharmonics);
	} else {
		text += "/" + std::to_string(subharmonics);
	}
	nvgText(disp.vg, textPos.x, textPos.y, text.c_str(), NULL);
}

void MixtureSwitch::onChange(const event::Change& e) {
	RoundSwitch::onChange(e);
	if (harmonizer) {
		if (harmonizer->params[Harmonizer::MIXTUR_PARAM + mixture].getValue()) {
			for (auto i = 0; i < Harmonizer::NUM_MIXTURES; ++i) {
				if (i != mixture) {
					if (harmonizer->params[Harmonizer::MIXTUR_PARAM + i].getValue()) {
						harmonizer->params[Harmonizer::MIXTUR_PARAM + i].setValue(0);
					}
				}
			}
		}
	}
}

HarmonizerWidget::HarmonizerWidget(Harmonizer* module) {
	setModule(module);
	setPanel("res/Harmonizer.svg");
	setSize(Vec(885, 380));
	setScrews(SCREW_TOP_LEFT, SCREW_TOP_RIGHT, SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);

	addInput(createInputCentered<InPort>(Vec(   385,  30), module, Harmonizer::GATE_INPUT));
	addOutput(createOutputCentered<OutPort>(Vec(420,  30), module, Harmonizer::GATE_OUTPUT));

	addInput(createInputCentered<InPort>(Vec(   495,  30), module, Harmonizer::RETRIGGER_INPUT));
	addOutput(createOutputCentered<OutPort>(Vec(530,  30), module, Harmonizer::RETRIGGER_OUTPUT));

	addInput(createInputCentered<InPort>(Vec(   325, 350), module, Harmonizer::VELOCITY_INPUT));
	addInput(createInputCentered<InPort>(Vec(   360, 350), module, Harmonizer::VELOCITY_GLOBAL_MODULATION_INPUT));
	addParam(createParamCentered<KnobTiny>(Vec( 390, 350), module, Harmonizer::VELOCITY_GLOBAL_MODULATION_PARAM));
	addOutput(createOutputCentered<OutPort>(Vec(420, 350), module, Harmonizer::VELOCITY_GLOBAL_OUTPUT));
	
	addInput(createInputCentered<InPort>(Vec(   495, 350), module, Harmonizer::PITCH_INPUT));
	addInput(createInputCentered<InPort>(Vec   (530, 350), module, Harmonizer::PITCH_GLOBAL_MODULATION_INPUT));
	addParam(createParamCentered<KnobTiny>(Vec( 560, 350), module, Harmonizer::PITCH_GLOBAL_MODULATION_PARAM));
	addOutput(createOutputCentered<OutPort>(Vec(590, 350), module, Harmonizer::PITCH_GLOBAL_OUTPUT));

	for (auto i = 0; i < Harmonizer::NUM_CHANNELS; ++i) {
		float x = i < 8 ? 25 : 465;
		float y = i < 8 ? 63 + i * 33 + (i < 4 ? 0 : 23) : 63 + (i - 8) * 33 + (i < 12 ? 0 : 23);
		addParam(createParamCentered<RoundSwitch>(Vec(  x +   0, y), module, Harmonizer::CHANNEL_ACTIVE_PARAM + i));

		addInput(createInputCentered<InPort>(Vec(       x +  30, y), module, Harmonizer::VELOCITY_MODULATION_INPUT + i));
		addParam(createParamCentered<KnobTiny>(Vec(     x +  60, y), module, Harmonizer::VELOCITY_MODULATION_PARAM + i));
		addParam(createParamCentered<KnobSmall>(Vec(    x +  95, y), module, Harmonizer::VELOCITY_PARAM + i));
		addOutput(createOutputCentered<OutPort>(Vec(    x + 130, y), module, Harmonizer::VELOCITY_OUTPUT + i));

		addInput(createInputCentered<InPort>(Vec(       x + 170, y), module, Harmonizer::PITCH_MODULATION_INPUT + i));
		addParam(createParamCentered<KnobTiny>(Vec(     x + 200, y), module, Harmonizer::PITCH_MODULATION_PARAM + i));
		addParam(createParamCentered<KnobSmallSnap>(Vec(x + 235, y), module, Harmonizer::PITCH_HARMONICS_PARAM + i));
		addChild(new HarmonicsDisplay(Rect(             x + 259, y - 9, 53, 18), module, i));
		addParam(createParamCentered<KnobSmallSnap>(Vec(x + 335, y), module, Harmonizer::PITCH_SUBHARMONICS_PARAM + i));
		addParam(createParamCentered<KnobTiny>(Vec(     x + 365, y), module, Harmonizer::PITCH_FINE_PARAM + i));
		addOutput(createOutputCentered<OutPort>(Vec(    x + 395, y), module, Harmonizer::PITCH_OUTPUT + i));
	}
	for (auto i = 0; i < Harmonizer::NUM_MIXTURES; ++i) {
		float x = i < 2 ? 195 : 635;
		float y = i % 2 ? 350 : 30;
		MixtureSwitch* mixtureSwitch = dynamic_cast<MixtureSwitch*>(createParamCentered<MixtureSwitch>(Vec(  x +   0, y), module, Harmonizer::MIXTUR_PARAM + i));
		mixtureSwitch->mixture = i;
		mixtureSwitch->harmonizer = module;
		addParam(mixtureSwitch);
		addInput(createInputCentered<InPort>(Vec(50 + i * 35,  350), module, Harmonizer::MIXTUR_INPUT + i));
	}
}

Model* modelHarmonizer = createModel<Harmonizer, HarmonizerWidget>("Harmonizer");
