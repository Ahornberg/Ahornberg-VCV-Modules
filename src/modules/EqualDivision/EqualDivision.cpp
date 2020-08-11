#include "EqualDivision.hpp"

std::string IntervalNames::getDisplayValueString() {
	return EqualDivision::INTERVALS[(int) getValue()].longName;
}

const Interval EqualDivision::INTERVALS[] = {
	{ log2f(9. / 8.) * 12, "9/8", "Major Second" },
	{ log2f(8. / 7.) * 12, "8/7", "Supermajor Second" },
	{ log2f(7. / 6.) * 12, "7/6", "Subminor Third" },
	{ log2f(6. / 5.) * 12, "6/5", "Minor Third" },
	{ log2f(5. / 4.) * 12, "5/4", "Major Third" },
	{ log2f(4. / 3.) * 12, "4/3", "Perfect Fourth" },
	{ log2f(3. / 2.) * 12, "3/2", "Perfect Fifth" },
	{ log2f(2.) * 12,      "2",   "Octave" },
	{ log2f(3.) * 12,      "3",   "Tritave (Octave + Perfect Fifth)" },
	{ log2f(4.) * 12,      "4",   "2 Octaves" },
	{ log2f(5.) * 12,      "5",   "2 Octaves + Major Third" },
	{ log2f(6.) * 12,      "6",   "2 Octaves + Perfect Fifth" },
	{ log2f(7.) * 12,      "7",   "2 Octaves + Subminor Seventh" },
	{ log2f(8.) * 12,      "8",   "3 Octaves" }
};

EqualDivision::EqualDivision() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam(FINE_PARAM, .97, 1.03, 1, "Fine", "%", 0, 100, -100);
	configParam(STEPS_PARAM, 1, 99, INIT_STEP, "Equal Steps");
	configParam<IntervalNames>(INTERVAL_PARAM, 0, NUM_INTERVALS, INIT_INTERVAL, "Interval");

}

void EqualDivision::process(const ProcessArgs& args) {
	float multiplier = INTERVALS[(int)params[INTERVAL_PARAM].getValue()].value / params[STEPS_PARAM].getValue() * params[FINE_PARAM].getValue();

	for (int i = 0; i < 9; i++) {
		if (outputs[PITCH_OUTPUT + i].isConnected()) {
			int channels = inputs[PITCH_INPUT + i].getChannels();
			outputs[PITCH_OUTPUT + i].setChannels(channels);
			
			for (int channel = 0; channel < channels; channel += 4) {
				outputs[PITCH_OUTPUT + i].setVoltageSimd(inputs[PITCH_INPUT + i].getPolyVoltageSimd<float_4>(channel) * multiplier, channel);
			}
		}
	}
}
