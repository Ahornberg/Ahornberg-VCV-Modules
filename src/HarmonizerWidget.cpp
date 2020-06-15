#include "Harmonizer.cpp"

struct HarmonicsDisplay : Display {
	Harmonizer* harmonizer;
	int channel;
	int harmonics;
	int subharmonics;

	HarmonicsDisplay(Rect box, Harmonizer* harmonizer, int channel) : Display(box) {
		this->harmonizer = harmonizer;
		this->channel = channel;
		harmonics = 1;
		subharmonics = 1;
	}

	void drawText(const DrawArgs &disp) override {
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
};

struct HarmonizerWidget : ModuleWidgetWithScrews {
	HarmonizerWidget(Harmonizer* module) {
		setModule(module);
		setPanel("res/Harmonizer.svg");
		setSize(Vec(885, 380));
		setScrews(true, true, true, true);

		addInput(createInputCentered<InPort>(Vec(   380,  30), module, Harmonizer::GATE_INPUT));
		addOutput(createOutputCentered<OutPort>(Vec(415,  30), module, Harmonizer::GATE_OUTPUT));

		addInput(createInputCentered<InPort>(Vec(   500,  30), module, Harmonizer::RETRIGGER_INPUT));
		addOutput(createOutputCentered<OutPort>(Vec(535,  30), module, Harmonizer::RETRIGGER_OUTPUT));

		addInput(createInputCentered<InPort>(Vec(   320, 350), module, Harmonizer::VELOCITY_INPUT));
		addInput(createInputCentered<InPort>(Vec(   355, 350), module, Harmonizer::VELOCITY_GLOBAL_MODULATION_INPUT));
		addParam(createParamCentered<KnobTiny>(Vec( 385, 350), module, Harmonizer::VELOCITY_GLOBAL_MODULATION_PARAM));
		addOutput(createOutputCentered<OutPort>(Vec(415, 350), module, Harmonizer::VELOCITY_GLOBAL_OUTPUT));
		
		addInput(createInputCentered<InPort>(Vec(   500, 350), module, Harmonizer::PITCH_INPUT));
		addInput(createInputCentered<InPort>(Vec   (535, 350), module, Harmonizer::PITCH_GLOBAL_MODULATION_INPUT));
		addParam(createParamCentered<KnobTiny>(Vec( 565, 350), module, Harmonizer::PITCH_GLOBAL_MODULATION_PARAM));
		addOutput(createOutputCentered<OutPort>(Vec(595, 350), module, Harmonizer::PITCH_GLOBAL_OUTPUT));

		for (auto i = 0; i < NUM_CHANNELS; ++i) {
			float x = i < 8 ? 20 : 470;
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
		addParam(createParamCentered<RoundSwitch>(Vec(190,  30), module, Harmonizer::MIXTUR_PARAM));
		addParam(createParamCentered<RoundSwitch>(Vec(640,  30), module, Harmonizer::MIXTUR_PARAM + 1));
		addParam(createParamCentered<RoundSwitch>(Vec(190, 350), module, Harmonizer::MIXTUR_PARAM + 2));
		addParam(createParamCentered<RoundSwitch>(Vec(640, 350), module, Harmonizer::MIXTUR_PARAM + 3));
		
		addInput(createInputCentered<InPort>(Vec(     830,  30), module, Harmonizer::MIXTUR_INPUT));
		addOutput(createOutputCentered<OutPort>(Vec(  865,  30), module, Harmonizer::MIXTUR_OUTPUT));
	}
};

Model* modelHarmonizer = createModel<Harmonizer, HarmonizerWidget>("Harmonizer");
