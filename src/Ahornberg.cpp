#include "Ahornberg.hpp"

Plugin* pluginInstance;

void init(Plugin* plugin) {
	new MIDIOverAudioDriver(false);
	pluginInstance = plugin;

	pluginInstance->addModel(modelTracker);
	pluginInstance->addModel(modelTapeRecorder);
	pluginInstance->addModel(modelTapeRecorderMixer);
	pluginInstance->addModel(modelTapeInspector);
	pluginInstance->addModel(modelFlyingFader);
	pluginInstance->addModel(modelEqualDivision);
	pluginInstance->addModel(modelCVFreqShift);
	pluginInstance->addModel(modelFunWithFrames);
	pluginInstance->addModel(modelMoreFunWithFrames);
	pluginInstance->addModel(modelMetronome);
	pluginInstance->addModel(modelHarmonizer);
	pluginInstance->addModel(modelMIDIController);
	pluginInstance->addModel(modelMIDIPolyExpression);
	pluginInstance->addModel(modelXGlide);
	pluginInstance->addModel(modelMIDIOverAudio);
	pluginInstance->addModel(modelCopyPastePreset);
	// pluginInstance->addModel(modelManual);
}
