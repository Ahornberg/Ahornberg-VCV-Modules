#include "Ahornberg.hpp"

Plugin* pluginInstance;

void init(Plugin *p) {
	pluginInstance = p;

	p->addModel(modelEqualDivision);
	p->addModel(modelCVFreqShift);
	p->addModel(modelTapeRecorder);
	p->addModel(modelMetronome);
	p->addModel(modelHarmonizer);
}
