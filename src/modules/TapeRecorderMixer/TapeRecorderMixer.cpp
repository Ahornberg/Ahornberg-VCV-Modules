#include "TapeRecorderMixer.hpp"

TapeRecorderMixer::TapeRecorderMixer() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam(RECORD_PARAM, 0, 1, 1, "Record On/Off");
	configParam(BYPASS_CHAIN_PARAM, 0, 1, 1, "Bypass Insert");
	configParam(TAPE_DUCKING_PARAM, 0, 10, 0, "Ducking");
	configParam(TAPE_ERASE_PARAM, -5, 5, 0, "Overdub / Replace");
	configParam(SOLO_PARAM, -1, 1, 0, "Solo");
	configParam(MUTE_PARAM, -1, 1, 0, "Mute");
	configParam(INPUT_VOLUME_PARAM, -5, 5, 0, "Input Volume");
	configParam(LINK_PARAM, 0, 1, 0, "Link To Left Module");
}

void TapeRecorderMixer::process(const ProcessArgs& args) {
	if (outputs[AUDIO_CHAIN_LEFT_OUTPUT].isConnected()) {
		int channels = inputs[AUDIO_CHAIN_LEFT_INPUT].getChannels() + 1;
		for (int channel = 0; channel < channels; ++channel) {
			
		}
	}
}
