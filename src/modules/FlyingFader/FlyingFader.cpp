#include "FlyingFader.hpp"

FlyingFader::FlyingFader() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam(FADER_PARAM, 0, PLUS_6_DB, 1, "Volume", " dB", -10, 40);
	configParam(CV_INPUT_WAS_CONNECTED, 0, 1, 0);
	configParam(FADER_VALUE_BEFORE_CONNECTED, 0, PLUS_6_DB, 1);
	
	faderDragged = false;
}

void FlyingFader::process(const ProcessArgs& args) {
	if (!faderDragged && inputs[CV_INPUT].isConnected()) {
		if (inputs[CV_INPUT].getVoltage() && !params[CV_INPUT_WAS_CONNECTED].getValue()) {
			params[FADER_VALUE_BEFORE_CONNECTED].setValue(params[FADER_VALUE_BEFORE_CONNECTED].getValue() / (inputs[CV_INPUT].getVoltage() / 10.f));
			params[CV_INPUT_WAS_CONNECTED].setValue(1);
		}
		params[FADER_PARAM].setValue(clamp(params[FADER_VALUE_BEFORE_CONNECTED].getValue() * inputs[CV_INPUT].getVoltage() / 10.f, 0.f, PLUS_6_DB));
	} else {
		params[CV_INPUT_WAS_CONNECTED].setValue(0);
		params[FADER_VALUE_BEFORE_CONNECTED].setValue(params[FADER_PARAM].getValue());
	}
	
	if (outputs[CV_OUTPUT].isConnected()) {
		outputs[CV_OUTPUT].setVoltage(params[FADER_PARAM].getValue() * 10.f);
	}

	// test
	// bool is_connected = inputs[CV_INPUT].isConnected();
	// float input_voltage = inputs[CV_INPUT].getVoltage();
	// if (is_connected != last_is_connected || input_voltage != last_input_voltage) {
		// DEBUG("Conn %d Volt %f", is_connected, input_voltage);
		// last_is_connected = is_connected;
		// last_input_voltage = input_voltage;
	// }
}
