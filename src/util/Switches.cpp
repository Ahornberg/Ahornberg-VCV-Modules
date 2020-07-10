#include "../Ahornberg.hpp"

BasicSwitch::BasicSwitch() {
	momentary = false;
	shadow->opacity = 0;
}

void BasicSwitch::addFrame(const std::string& filename) {
	SvgSwitch::addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}	

RoundSwitch::RoundSwitch() {
	addFrame("res/switches/Round_off.svg");
	addFrame("res/switches/Round_on.svg");
}

RoundSwitchRed::RoundSwitchRed() {
	addFrame("res/switches/Round_off.svg");
	addFrame("res/switches/Round_red.svg");
}

RoundLargeSwitch::RoundLargeSwitch() {
	addFrame("res/switches/RoundLarge_off.svg");
	addFrame("res/switches/RoundLarge_on.svg");
}

PauseSwitch::PauseSwitch() {
	addFrame("res/switches/Pause_off.svg");
	addFrame("res/switches/Pause_on.svg");
}
