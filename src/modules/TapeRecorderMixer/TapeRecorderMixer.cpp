#include "TapeRecorderMixer.hpp"

TapeRecorderMixer::TapeRecorderMixer() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam(TAPE_VOLUME_PARAM, -5, 5, 0, "Overdub / Replace");
}

void TapeRecorderMixer::process(const ProcessArgs& args) {
	if (outputs[AUDIO_CHAIN_LEFT_OUTPUT].isConnected()) {
		int channels = inputs[AUDIO_CHAIN_LEFT_INPUT].getChannels() + 1;
		for (int channel = 0; channel < channels; ++channel) {
			
		}
	}
}
