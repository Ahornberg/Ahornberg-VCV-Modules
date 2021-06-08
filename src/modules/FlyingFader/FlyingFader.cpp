#include "FlyingFader.hpp"

const std::string FlyingFader::INIT_FADER_NAME = "My Flying Fader";

FlyingFader::FlyingFader() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam(FADER_PARAM, 0, PLUS_6_DB, 1, "Volume", " dB", -10, 40);
	// configParam(FADER_PARAM, 0, PLUS_6_DB, 1, "Volume", " dB", -10.f, 60.f * PLUS_6_DB);
	// configParam(FADER_PARAM, 0, PLUS_6_DB, 1, "Volume", " dB", -10.f, 60.f);
	configParam(CV_INPUT_WAS_CONNECTED, 0, 1, 0);
	configParam(FADER_VALUE_BEFORE_CONNECTED, 0, PLUS_6_DB, 1);
	configParam(AUDIO_POLY_MODE, 0, 1, 0);
	configParam(CV_SCALE_MODE, 0, 1, 0);
	
	faderDragged = false;
	audioSlewLimiter.setRiseFall(AUDIO_MUTE_SLEW, AUDIO_MUTE_SLEW);
	audioSlewLimiter.reset();
}

void FlyingFader::process(const ProcessArgs& args) {
	if (!faderDragged && inputs[CV_INPUT].isConnected()) {
		float cvInputNormalized = inputs[CV_INPUT].getVoltage() * .1;
		if (cvInputNormalized && !params[CV_INPUT_WAS_CONNECTED].getValue()) {
			params[FADER_VALUE_BEFORE_CONNECTED].setValue(params[FADER_VALUE_BEFORE_CONNECTED].getValue() / cvInputNormalized);
			params[CV_INPUT_WAS_CONNECTED].setValue(1);
		}
		cvInputNormalized += 1e-37;
		params[FADER_PARAM].setValue(clamp((params[FADER_VALUE_BEFORE_CONNECTED].getValue()) * (cvInputNormalized), 0.f, PLUS_6_DB));
	} else {
		params[CV_INPUT_WAS_CONNECTED].setValue(0);
		params[FADER_VALUE_BEFORE_CONNECTED].setValue(params[FADER_PARAM].getValue());
	}
	
	if (outputs[CV_OUTPUT].isConnected()) {
		if (params[CV_SCALE_MODE].getValue()) {
			outputs[CV_OUTPUT].setVoltage(pow(params[FADER_PARAM].getValue() * 22.3606765f, 1.f / 1.5f));
		} else {
			outputs[CV_OUTPUT].setVoltage(params[FADER_PARAM].getValue() * 10.f);
		}
	}
	
	if (outputs[AUDIO_OUTPUT].isConnected() && inputs[AUDIO_INPUT].isConnected()) {
		float multiplier = pow(audioSlewLimiter.process(args.sampleTime, params[FADER_PARAM].getValue()), 2.f);
		int channels = inputs[AUDIO_INPUT].getChannels();
		if (channels < 2) {
			outputs[AUDIO_OUTPUT].setChannels(1);
			outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage() * multiplier);
		} else if (params[AUDIO_POLY_MODE].getValue()) {
			outputs[AUDIO_OUTPUT].setChannels(1);
			outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltageSum() * multiplier);
		} else {
			
			outputs[AUDIO_OUTPUT].setChannels(channels);
			for (int channel = 0; channel < channels; channel += 4) {
				outputs[AUDIO_OUTPUT].setVoltageSimd(inputs[AUDIO_INPUT].getPolyVoltageSimd<float_4>(channel) * multiplier, channel);
			}
		}
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
