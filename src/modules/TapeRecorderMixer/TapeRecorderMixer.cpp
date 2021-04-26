#include "TapeRecorderMixer.hpp"

TapeRecorderMixer::TapeRecorderMixer() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam<OnOff>(RECORD_PARAM, 0, 1, 0, "Record");
	configParam<OnOff>(BYPASS_CHAIN_PARAM, 0, 1, 0, "Bypass Insert");
	configParam(TAPE_DUCKING_PARAM, 0, 10, 0, "Tape Ducking");
	configParam(TAPE_ERASE_PARAM, PLUS_6_DB, 0, 1, "Tape Volume", " dB", -10, 40);
	configParam<OnOff>(SOLO_PARAM, 0, 1, 0, "Solo");
	configParam<OnOff>(MUTE_PARAM, 0, 1, 0, "Mute");
	configParam(INPUT_VOLUME_PARAM, 0, PLUS_6_DB, 1, "Input Volume", " dB", -10, 40);
	configParam<OnOff>(INPUT_MUTE_PARAM, 0, 1, 0, "Input Mute");
	configParam<OnOff>(LINK_PARAM, 0, 1, 0, "Link To Left Module");
}

void TapeRecorderMixer::process(const ProcessArgs& args) {
	// if bus not connected then in -> fx -> meter -> out
	// duck is a 3-band multicompressor in the tape, sidechained by the input
	// duck: half of duck works inverse (boost)
	// context menu switch: duck works on raw input (pre-fader) to make ducking only possible for bumping effects
	float audio = 0;
	float duckingSidechain = 0;
	if (inputs[AUDIO_INPUT].isConnected()) {
		duckingSidechain = inputs[AUDIO_INPUT].getVoltage() * pow(params[INPUT_VOLUME_PARAM].getValue(), 2.f);
		if (!params[INPUT_MUTE_PARAM].getValue()) {
			audio = duckingSidechain;
		}
	}
	if (inputs[AUDIO_CHAIN_RIGHT_INPUT].isConnected()) {
		audio += inputs[AUDIO_CHAIN_RIGHT_INPUT].getVoltage() * pow(params[TAPE_ERASE_PARAM].getValue(), 2.f);
	}
	if (outputs[AUDIO_FX_SEND].isConnected() && !params[BYPASS_CHAIN_PARAM].getValue()) {
		outputs[AUDIO_FX_SEND].setVoltage(audio);
	}
	if (inputs[AUDIO_FX_RETURN].isConnected() && !params[BYPASS_CHAIN_PARAM].getValue()) {
		audio = inputs[AUDIO_FX_RETURN].getVoltage();
	}
	if (outputs[AUDIO_CHAIN_RIGHT_OUTPUT].isConnected()) {
		if (params[RECORD_PARAM].getValue()) {
			outputs[AUDIO_CHAIN_RIGHT_OUTPUT].setVoltage(audio);
		} else {
			outputs[AUDIO_CHAIN_RIGHT_OUTPUT].setVoltage(inputs[AUDIO_CHAIN_LEFT_INPUT].getVoltage());
		}
	}
	if (outputs[AUDIO_OUTPUT].isConnected() && !params[MUTE_PARAM].getValue()) {
		outputs[AUDIO_OUTPUT].setVoltage(audio);
	}
	
	
	// if (outputs[AUDIO_CHAIN_LEFT_OUTPUT].isConnected()) {
		// int channels = inputs[AUDIO_CHAIN_LEFT_INPUT].getChannels() + 1;
		// for (int channel = 0; channel < channels; ++channel) {
			
		// }
	// }
}
