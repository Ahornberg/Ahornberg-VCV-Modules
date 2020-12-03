#include "FunWithFrames.hpp"

std::string ConvertModes::getDisplayValueString() {
	return getValue() ? "linear with jump point" : "linear";
}

FunWithFrames::FunWithFrames() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam(MAX_CHORD_SIZE_PARAM, 2, NUM_VALUE_IO + 1, 3, "Maximum Chord-Size");
	for (auto i = 0; i < NUM_VALUE_IO; ++i) {
		configParam<ConvertModes>(VALUE_CONVERT_MODE + i, 0, 1, 0, "Convert-Mode");
		lastPitchChord[i] = 0;
		pitchGate[i] = false;
	}
}

void FunWithFrames::process(const ProcessArgs& args) {
	for (auto i = 0; i < NUM_VALUE_IO; i++) {
		if (outputs[VALUE_OUTPUT + i].isConnected()) {
			float valueOut = inputs[VALUE_INPUT + i].getVoltage() * 2.125;
			if (params[VALUE_CONVERT_MODE + i].getValue()) {
				if (valueOut > 5 * 2.125) {
					valueOut -= 256. / 12;
				}
			} else {
				valueOut -= 5;
			}
			outputs[VALUE_OUTPUT + i].setVoltage(valueOut);
		}
	}
	if (outputs[NOTE_OUTPUT].isConnected()) {
		outputs[NOTE_OUTPUT].setChannels(params[MAX_CHORD_SIZE_PARAM].getValue());
		for (auto i = 0; i < params[MAX_CHORD_SIZE_PARAM].getValue(); i++) {
			float pitchOut = inputs[NOTE_INPUT].getVoltage();
			if (i > 0) {
				// add relative chord pitch from frames parameter value
				float pitchChord = inputs[VALUE_INPUT + i - 1].getVoltage() * 2.125;
				if (pitchChord > 5 * 2.125) {
					pitchChord -= 256. / 12;
				}
				if (pitchChord) {
					pitchOut += pitchChord;
					lastPitchChord[i - 1] = pitchOut;
					pitchGate[i - 1] = true;
				} else {
					pitchOut = lastPitchChord[i - 1];
					pitchGate[i - 1] = false;
				}
			}
			outputs[NOTE_OUTPUT].setVoltage(pitchOut, i);
		}
	}
	if (outputs[GATE_OUTPUT].isConnected()) {
		outputs[GATE_OUTPUT].setChannels(params[MAX_CHORD_SIZE_PARAM].getValue());
		for (auto i = 0; i < params[MAX_CHORD_SIZE_PARAM].getValue(); i++) {
			if (i > 0 && !pitchGate[i - 1]) {
				outputs[GATE_OUTPUT].setVoltage(0, i);
			} else {
				outputs[GATE_OUTPUT].setVoltage(inputs[GATE_INPUT].getVoltage(), i);
			}
		}
	}
}
