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

InPortSmall::InPortSmall() {
	InPortSmall::setSvg("res/ports/InSmall.svg");
}	

OutPort::OutPort() {
	OutPort::setSvg("res/ports/Out.svg");
}	

OutPortSmall::OutPortSmall() {
	OutPortSmall::setSvg("res/ports/OutSmall.svg");
}	
