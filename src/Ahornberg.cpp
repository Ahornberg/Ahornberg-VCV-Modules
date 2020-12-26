#include "Ahornberg.hpp"

Plugin* pluginInstance;

void init(Plugin* plugin) {
	pluginInstance = plugin;

	pluginInstance->addModel(modelTapeRecorder);
	pluginInstance->addModel(modelEqualDivision);
	pluginInstance->addModel(modelCVFreqShift);
	pluginInstance->addModel(modelFunWithFrames);
	pluginInstance->addModel(modelMetronome);
	pluginInstance->addModel(modelHarmonizer);
	// pluginInstance->addModel(modelManual);
}
