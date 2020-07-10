#include "../Ahornberg.hpp"

BasicPort::BasicPort() {
	shadow->hide();
}

void BasicPort::setSvg(const std::string& filename) {
	SvgPort::setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}	

InPort::InPort() {
	InPort::setSvg("res/ports/In.svg");
}	

OutPort::OutPort() {
	OutPort::setSvg("res/ports/Out.svg");
}	
