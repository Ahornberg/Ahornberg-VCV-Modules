#include "FunWithFrames.hpp"

FunWithFrames::FunWithFrames() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam(CHORD_SIZE_PARAM, 2, 5, 3, "Chord Size");
	for (auto i = 0; i < 4; ++i) {
		configParam(VALUE_CONVERT_MODE + i, 0, 1, 0, "Convert Mode");
	}
}

void FunWithFrames::process(const ProcessArgs& args) {
	for (auto i = 0; i < 4; i++) {
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
		outputs[NOTE_OUTPUT].setChannels(params[CHORD_SIZE_PARAM].getValue());
		for (auto i = 0; i < params[CHORD_SIZE_PARAM].getValue(); i++) {
			float pitchOut = inputs[NOTE_INPUT].getVoltage();
			if (i > 0) {
				// add relative chord pitch from frames parameter value
				float pitchChord = inputs[VALUE_INPUT + i - 1].getVoltage() * 2.125;
				if (pitchChord > 5 * 2.125) {
					pitchChord -= 256. / 12;
				}
				pitchOut += pitchChord;
			}
			outputs[NOTE_OUTPUT].setVoltage(pitchOut, i);
		}
	}
	if (outputs[GATE_OUTPUT].isConnected()) {
		outputs[GATE_OUTPUT].setChannels(params[CHORD_SIZE_PARAM].getValue());
		for (auto i = 0; i < params[CHORD_SIZE_PARAM].getValue(); i++) {
			outputs[GATE_OUTPUT].setVoltage(inputs[GATE_INPUT].getVoltage(), i);
		}
	}
}
