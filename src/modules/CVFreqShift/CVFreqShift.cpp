#include "CVFreqShift.hpp"

const FrequencyRange CVFreqShift::FREQUENCY_RANGES[] = {
	{ (int) powf(2, 0), powf(2, 0) / dsp::FREQ_C4},
	{ (int) powf(2, 1), powf(2, 1) / dsp::FREQ_C4},
	{ (int) powf(2, 2), powf(2, 2) / dsp::FREQ_C4},
	{ (int) powf(2, 3), powf(2, 3) / dsp::FREQ_C4},
	{ (int) powf(2, 4), powf(2, 4) / dsp::FREQ_C4},
	{ (int) powf(2, 5), powf(2, 5) / dsp::FREQ_C4},
	{ (int) powf(2, 6), powf(2, 6) / dsp::FREQ_C4},
	{ (int) powf(2, 7), powf(2, 7) / dsp::FREQ_C4},
	{ (int) powf(2, 8), powf(2, 8) / dsp::FREQ_C4}
};

const std::vector<std::string> CVFreqShift::FREQUENCY_RANGE_LABELS = {
	"+/-" + std::to_string(CVFreqShift::FREQUENCY_RANGES[0].rangeInHz * CVFreqShift::MIN_FREQUENCY_RANGE) + " Hz",
	"+/-" + std::to_string(CVFreqShift::FREQUENCY_RANGES[1].rangeInHz * CVFreqShift::MIN_FREQUENCY_RANGE) + " Hz",
	"+/-" + std::to_string(CVFreqShift::FREQUENCY_RANGES[2].rangeInHz * CVFreqShift::MIN_FREQUENCY_RANGE) + " Hz",
	"+/-" + std::to_string(CVFreqShift::FREQUENCY_RANGES[3].rangeInHz * CVFreqShift::MIN_FREQUENCY_RANGE) + " Hz",
	"+/-" + std::to_string(CVFreqShift::FREQUENCY_RANGES[4].rangeInHz * CVFreqShift::MIN_FREQUENCY_RANGE) + " Hz",
	"+/-" + std::to_string(CVFreqShift::FREQUENCY_RANGES[5].rangeInHz * CVFreqShift::MIN_FREQUENCY_RANGE) + " Hz",
	"+/-" + std::to_string(CVFreqShift::FREQUENCY_RANGES[6].rangeInHz * CVFreqShift::MIN_FREQUENCY_RANGE) + " Hz",
	"+/-" + std::to_string(CVFreqShift::FREQUENCY_RANGES[7].rangeInHz * CVFreqShift::MIN_FREQUENCY_RANGE) + " Hz",
	"+/-" + std::to_string(CVFreqShift::FREQUENCY_RANGES[8].rangeInHz * CVFreqShift::MIN_FREQUENCY_RANGE) + " Hz"
};

CVFreqShift::CVFreqShift() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configParam(FREQUENCY_PARAM, MIN_FREQUENCY_RANGE * -1, MIN_FREQUENCY_RANGE, 0, "Frequency", " Hz");
	configParam(FREQUENCY_MODULATION_AMOUNT_PARAM, 0, 1, 0, "Frequency Modulation", "%", 0, 100);
	configSwitch(FREQUENCY_RANGE_PARAM, 0, 8, 4, "Frequency Range", FREQUENCY_RANGE_LABELS);
	for (auto i = 0; i < 9; ++i) {
		std::string portLabel = "Pitch " + std::to_string(i + 1) + " (1V/Octave)";
		configInput(PITCH_INPUT + i, portLabel);
		configOutput(PITCH_OUTPUT + i, portLabel);
		configBypass(PITCH_INPUT + i, PITCH_OUTPUT + i);
	}
	configInput(FREQUENCY_MODULATION_INPUT, "Frequency Modulation");
}

void CVFreqShift::process(const ProcessArgs& args) {
	float freqShift = params[FREQUENCY_PARAM].getValue() * FREQUENCY_RANGES[(int) params[FREQUENCY_RANGE_PARAM].getValue()].rangeBasedOnC4;
	if (inputs[FREQUENCY_MODULATION_INPUT].isConnected()) {
		freqShift += powf(params[FREQUENCY_MODULATION_AMOUNT_PARAM].getValue(), 4) * inputs[FREQUENCY_MODULATION_INPUT].getVoltage();
	}

	for (int i = 0; i < 9; i++) {
		if (outputs[PITCH_OUTPUT + i].isConnected()) {
			int channels = inputs[PITCH_INPUT + i].getChannels();
			outputs[PITCH_OUTPUT + i].setChannels(channels);
			
			for (int channel = 0; channel < channels; channel += 4) {
				outputs[PITCH_OUTPUT + i].setVoltageSimd(
					log2(
						fmax(
							.0009766f, pow(
								2.f, inputs[PITCH_INPUT + i].getPolyVoltageSimd<float_4>(channel)
							) + freqShift
						)
					), channel
				);
			}
		}
	}
}
