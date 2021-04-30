#include "TapeRecorderMixer.hpp"

TapeRecorderMixer::TapeRecorderMixer() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam<OnOff>(RECORD_PARAM, 0, 1, 0, "Record");
	configParam<OnOff>(BYPASS_CHAIN_PARAM, 0, 1, 0, "Bypass Insert");
	configParam(TAPE_DUCKING_PARAM, 0, PLUS_6_DB, 1, "Tape Ducking", " dB", -10, 40);
	configParam(TAPE_ERASE_PARAM, 0, PLUS_6_DB, 1, "Tape Erase Amount", " dB", -10, 40);
	configParam<OnOff>(SOLO_PARAM, 0, 1, 0, "Solo");
	configParam<OnOff>(MUTE_PARAM, 0, 1, 0, "Mute");
	configParam(INPUT_VOLUME_PARAM, 0, PLUS_6_DB, 1, "Input Volume", " dB", -10, 40);
	configParam<OnOff>(INPUT_MUTE_PARAM, 0, 1, 0, "Input Mute");
	configParam<OnOff>(INPUT_MUTE_ENABLED_PARAM, 0, 1, 0, "Input Mute Enabled");
	configParam<OnOff>(LINK_PARAM, 0, 1, 0, "Link To Left Module");
	muteSlewLimiter.setRiseFall(AUDIO_MUTE_SLEW, AUDIO_MUTE_SLEW);
	muteSlewLimiter.reset();
	inputMuteSlewLimiter.setRiseFall(AUDIO_MUTE_SLEW, AUDIO_MUTE_SLEW);
	inputMuteSlewLimiter.reset();
	fxReturnSlewLimiter.setRiseFall(AUDIO_MUTE_SLEW, AUDIO_MUTE_SLEW);
	fxReturnSlewLimiter.reset();
	fxBypassSlewLimiter.setRiseFall(AUDIO_MUTE_SLEW, AUDIO_MUTE_SLEW);
	fxBypassSlewLimiter.reset();
	cvInputVolumeSlewLimiter.setRiseFall(AUDIO_MUTE_SLEW, AUDIO_MUTE_SLEW);
	cvInputVolumeSlewLimiter.reset();
	recordInputTrigger.reset();
	fxBypassInputTrigger.reset();
	soloInputTrigger.reset();
	muteInputTrigger.reset();
}

void TapeRecorderMixer::process(const ProcessArgs& args) {
	// if bus not connected then in -> fx -> meter -> out
	// duck is a 3-band multicompressor in the tape, sidechained by the input
	// duck: half of duck works inverse (boost)
	// context menu switch: duck works on raw input (pre-fader) to make ducking only possible for bumping effects
	if (inputs[CV_RECORD_INPUT].isConnected()) {
		if (recordInputTrigger.process(rescaleInput(inputs[CV_RECORD_INPUT]))) {
			toggleParamValue(params[RECORD_PARAM]);
		}
	}
	if (inputs[CV_BYPASS_CHAIN_INPUT].isConnected()) {
		if (fxBypassInputTrigger.process(rescaleInput(inputs[CV_BYPASS_CHAIN_INPUT]))) {
			toggleParamValue(params[BYPASS_CHAIN_PARAM]);
		}
	}
	if (inputs[CV_SOLO_INPUT].isConnected()) {
		if (soloInputTrigger.process(rescaleInput(inputs[CV_SOLO_INPUT]))) {
			toggleParamValue(params[SOLO_PARAM]);
		}
	}
	if (inputs[CV_MUTE_INPUT].isConnected()) {
		if (muteInputTrigger.process(rescaleInput(inputs[CV_MUTE_INPUT]))) {
			toggleParamValue(params[MUTE_PARAM]);
		}
	}

	float audio = 0;
	if (inputs[AUDIO_INPUT].isConnected()) {
		float cvInputVolume = 1;
		if (inputs[CV_INPUT_VOLUME_INPUT].isConnected()) {
			if (params[INPUT_MUTE_ENABLED_PARAM].getValue()) {
				cvInputVolume = clamp(inputs[CV_INPUT_VOLUME_INPUT].getVoltage(), -5.f, 5.f) / 5.f;
				if (cvInputVolume      < 0) {
					params[INPUT_MUTE_PARAM].setValue(1);
					cvInputVolume = abs(cvInputVolume);
				} else {
					params[INPUT_MUTE_PARAM].setValue(0);
				}
			} else {
				cvInputVolume = clamp(inputs[CV_INPUT_VOLUME_INPUT].getVoltage(), 0.f, 10.f) / 10.f;
			}
			cvInputVolume = cvInputVolumeSlewLimiter.process(args.sampleTime, cvInputVolume);
		}
		audio = inputs[AUDIO_INPUT].getVoltage() * pow(params[INPUT_VOLUME_PARAM].getValue() * cvInputVolume, 2.f) * inputMuteSlewLimiter.process(args.sampleTime, !(params[INPUT_MUTE_ENABLED_PARAM].getValue() && params[INPUT_MUTE_PARAM].getValue()));
	}
	if (inputs[AUDIO_CHAIN_RIGHT_INPUT].isConnected()) {
		audio += inputs[AUDIO_CHAIN_RIGHT_INPUT].getVoltage() * pow(params[TAPE_ERASE_PARAM].getValue(), 2.f);
	}
	if (outputs[AUDIO_FX_SEND].isConnected()) {
		// outputs[AUDIO_FX_SEND].setVoltage(audio * fxSendSlewLimiter.process(args.sampleTime, !params[BYPASS_CHAIN_PARAM].getValue()));
		outputs[AUDIO_FX_SEND].setVoltage(audio);
	}
	if (inputs[AUDIO_FX_RETURN].isConnected()) {
		audio = audio * fxBypassSlewLimiter.process(args.sampleTime, params[BYPASS_CHAIN_PARAM].getValue()) + inputs[AUDIO_FX_RETURN].getVoltage() * fxReturnSlewLimiter.process(args.sampleTime, !params[BYPASS_CHAIN_PARAM].getValue());
	}
	if (outputs[AUDIO_CHAIN_RIGHT_OUTPUT].isConnected()) {
		if (params[RECORD_PARAM].getValue()) {
			outputs[AUDIO_CHAIN_RIGHT_OUTPUT].setVoltage(audio);
		} else {
			outputs[AUDIO_CHAIN_RIGHT_OUTPUT].setVoltage(inputs[AUDIO_CHAIN_LEFT_INPUT].getVoltage());
		}
	}
	if (outputs[AUDIO_OUTPUT].isConnected()) {
		outputs[AUDIO_OUTPUT].setVoltage(audio * muteSlewLimiter.process(args.sampleTime, !params[MUTE_PARAM].getValue()));
	}
	
	
	// if (outputs[AUDIO_CHAIN_LEFT_OUTPUT].isConnected()) {
		// int channels = inputs[AUDIO_CHAIN_LEFT_INPUT].getChannels() + 1;
		// for (int channel = 0; channel < channels; ++channel) {
			
		// }
	// }
}
