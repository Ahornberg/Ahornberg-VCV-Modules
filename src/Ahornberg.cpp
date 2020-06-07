#include "Ahornberg.hpp"

Plugin* pluginInstance;

void init(rack::Plugin *p) {

	pluginInstance = p;

	p->addModel(modelEqualDivision);
	// p->addModel(modelTapeRecorder);
	p->addModel(modelMetronome);
}
