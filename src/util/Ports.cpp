#include "../Ahornberg.hpp"

BasicPort::BasicPort() {
	shadow->hide();
}

void BasicPort::setSvg(const std::string& filename, const std::string& filenameDark) {
	ThemedSvgPort::setSvg(
		APP->window->loadSvg(asset::plugin(pluginInstance, filename)),
		APP->window->loadSvg(asset::plugin(pluginInstance, filenameDark))
	);
}	

InPort::InPort() {
	InPort::setSvg("res/ports/In.svg", "res/ports/In-dark.svg");
}	

InPortSmall::InPortSmall() {
	InPortSmall::setSvg("res/ports/InSmall.svg", "res/ports/InSmall-dark.svg");
}	

OutPort::OutPort() {
	OutPort::setSvg("res/ports/Out.svg", "res/ports/Out-dark.svg");
}	

OutPortSmall::OutPortSmall() {
	OutPortSmall::setSvg("res/ports/OutSmall.svg", "res/ports/OutSmall-dark.svg");
}	
