#include "Ahornberg.hpp"

Plugin* pluginInstance;

void init(Plugin* plugin) {
	new MIDIOverAudioDriver(false);
	pluginInstance = plugin;

	pluginInstance->addModel(modelTracker);
	pluginInstance->addModel(modelMIDIPolyEnvelope);
	pluginInstance->addModel(modelTapeRecorder);
	pluginInstance->addModel(modelTapeRecorderMixer);
	pluginInstance->addModel(modelFlyingFader);
	pluginInstance->addModel(modelEqualDivision);
	pluginInstance->addModel(modelCVFreqShift);
	pluginInstance->addModel(modelFunWithFrames);
	pluginInstance->addModel(modelMetronome);
	pluginInstance->addModel(modelHarmonizer);
	pluginInstance->addModel(modelMIDIOverAudio);
	// pluginInstance->addModel(modelManual);
}
