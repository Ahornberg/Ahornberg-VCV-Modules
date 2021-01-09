#include "Ahornberg.hpp"

Plugin* pluginInstance;

void init(Plugin* plugin) {
	new MIDIOverAudioDriver;
	pluginInstance = plugin;

	pluginInstance->addModel(modelTapeRecorder);
	pluginInstance->addModel(modelEqualDivision);
	pluginInstance->addModel(modelCVFreqShift);
	pluginInstance->addModel(modelFunWithFrames);
	pluginInstance->addModel(modelMetronome);
	pluginInstance->addModel(modelHarmonizer);
	pluginInstance->addModel(modelMIDIOverAudio);
	// pluginInstance->addModel(modelManual);
}
