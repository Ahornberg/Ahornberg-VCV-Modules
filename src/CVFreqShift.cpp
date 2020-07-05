#include "Ahornberg.hpp"

struct CVFreqShift : ModuleWithScrews {
	enum ParamIds {
		FREQUENCY_PARAM = NUM_MAX_SCREWS,
		FREQUENCY_MODULATION_AMOUNT_PARAM,
		ROOT_OCTAVE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(PITCH_INPUT, 9),
		FREQUENCY_MODULATION_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(PITCH_OUTPUT, 9),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	CVFreqShift() { 
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configScrewParams();
		configParam(FREQUENCY_PARAM, -150, 150, 0, "Frequency", "Hz");
		configParam(FREQUENCY_MODULATION_AMOUNT_PARAM, 0, 1, 0, "Frequency Modulation");
		configParam(ROOT_OCTAVE_PARAM, 0, 8, 4, "Root Octave");
	}
	
	void process(const ProcessArgs &args) override {
		float freqShift = (params[FREQUENCY_PARAM].getValue() / dsp::FREQ_C4);
		if (inputs[FREQUENCY_MODULATION_INPUT].isConnected()) {
			freqShift += powf(params[FREQUENCY_MODULATION_AMOUNT_PARAM].getValue(), 4) * inputs[FREQUENCY_MODULATION_INPUT].getVoltage();
		}
		float baseShift = params[ROOT_OCTAVE_PARAM].getValue() - 4;

		for (int i = 0; i < 9; i++) {
			if (outputs[PITCH_OUTPUT + i].isConnected()) {
				int channels = inputs[PITCH_INPUT + i].getChannels();
				outputs[PITCH_OUTPUT + i].setChannels(channels);
				
				for (int channel = 0; channel < channels; channel += 4) {
					outputs[PITCH_OUTPUT + i].setVoltageSimd(
						log2(
							pow(
								2.f, inputs[PITCH_INPUT + i].getPolyVoltageSimd<float_4>(channel) + baseShift
							) + freqShift
						) - baseShift, channel
					);
				}
			}
		}
	}
};
