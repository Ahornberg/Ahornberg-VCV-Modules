#include "Ahornberg.hpp"

Plugin *pluginInstance;

void init(rack::Plugin *p) {

	pluginInstance = p;

	p->addModel(modelEqualDivision);
}
