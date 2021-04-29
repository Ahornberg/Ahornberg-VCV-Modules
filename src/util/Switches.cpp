#include "../Ahornberg.hpp"

void toggleParamValue(Param& param) {
	param.getValue() ? param.setValue(0) : param.setValue(1);
}

float rescaleInput(Input& port, int channel) {
	return rescale(port.getVoltage(channel), 0.1, 2, 0, 1);
}

std::string OnOff::getDisplayValueString() {
	return getValue() ? "On" : "Off";
}

BasicSwitch::BasicSwitch() {
	momentary = false;
	shadow->opacity = 0;
}

void BasicSwitch::addFrame(const std::string& filename) {
	SvgSwitch::addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}

void BasicRoundSwitch::onHover(const event::Hover& e) {
	math::Vec c = box.size.div(2);
	float dist = e.pos.minus(c).norm();
	if (dist <= c.x) {
		SvgSwitch::onHover(e);
	}
}

void BasicRoundSwitch::onButton(const event::Button& e) {
	math::Vec c = box.size.div(2);
	float dist = e.pos.minus(c).norm();
	if (dist <= c.x) {
		SvgSwitch::onButton(e);
	}
}

RoundSwitch::RoundSwitch() {
	addFrame("res/switches/Round_off.svg");
	addFrame("res/switches/Round_on.svg");
}

RoundSwitchMedium::RoundSwitchMedium() {
	addFrame("res/switches/RoundMedium_off.svg");
	addFrame("res/switches/RoundMedium_on.svg");
}

RoundSwitchRed::RoundSwitchRed() {
	addFrame("res/switches/Round_off.svg");
	addFrame("res/switches/Round_red.svg");
}

RoundSwitchLinearJump::RoundSwitchLinearJump() {
	addFrame("res/switches/Round_linear.svg");
	addFrame("res/switches/Round_linear_jump.svg");
}

RoundLargeSwitch::RoundLargeSwitch() {
	addFrame("res/switches/RoundLarge_off.svg");
	addFrame("res/switches/RoundLarge_on.svg");
}

PauseSwitch::PauseSwitch() {
	addFrame("res/switches/Pause_off.svg");
	addFrame("res/switches/Pause_on.svg");
}
