#include "../Ahornberg.hpp"

void toggleParamValue(Param& param) {
	param.getValue() ? param.setValue(0) : param.setValue(1);
}

float rescaleInput(Input& port, int channel) {
	return rescale(port.getVoltage(channel), 0.1, 2, 0, 1);
}

const std::vector<std::string> BasicSwitch::ON_OFF_NAMES = {
	"Off",
	"On"
};

BasicSwitch::BasicSwitch() {
	sw = new ThemedSvgWidget;
	fb->addChild(sw);
	momentary = false;
	shadow->opacity = 0;
	hasContextMenu = false;
}

void BasicSwitch::addFrame(const std::string& filename, const std::string& filenameDark) {
	std::shared_ptr<window::Svg> svg = APP->window->loadSvg(asset::plugin(pluginInstance, filename));
	std::shared_ptr<window::Svg> svgDark = APP->window->loadSvg(asset::plugin(pluginInstance, filenameDark));
	frames.push_back(svg);
	frames.push_back(svgDark);
	// If this is our first frame, automatically set SVG and size
	if (!sw->svg) {
		dynamic_cast<ThemedSvgWidget*>(sw)->setSvg(svg, svgDark);
		box.size = sw->box.size;
		fb->box.size = sw->box.size;
		// Move shadow downward by 10%
		shadow->box.size = sw->box.size;
		shadow->box.pos = math::Vec(0, sw->box.size.y * 0.10);
	}
}

void BasicSwitch::onDragStart(const DragStartEvent& e) {
	Switch::onDragStart(e);
	if (e.button != GLFW_MOUSE_BUTTON_LEFT)
		return;

	// Set down frame if latch
	if (latch) {
		if (frames.size() >= 4) {
			dynamic_cast<ThemedSvgWidget*>(sw)->setSvg(frames[2], frames[3]);
			fb->setDirty();
		}
	}
}


void BasicSwitch::onDragEnd(const DragEndEvent& e) {
	Switch::onDragEnd(e);
	if (e.button != GLFW_MOUSE_BUTTON_LEFT)
		return;
	
	// Set up frame if latch
	if (latch) {
		if (frames.size() >= 2) {
			dynamic_cast<ThemedSvgWidget*>(sw)->setSvg(frames[0], frames[1]);
			fb->setDirty();
		}
	}
}


void BasicSwitch::onChange(const ChangeEvent& e) {
	if (!latch) {
		engine::ParamQuantity* pq = getParamQuantity();
		if (!frames.empty() && pq) {
			int index = (int) std::round(pq->getValue() - pq->getMinValue());
			index = math::clamp(index, 0, (int) (frames.size() / 2) - 1);
			dynamic_cast<ThemedSvgWidget*>(sw)->setSvg(frames[index * 2], frames[(index * 2) + 1]);
			fb->setDirty();
		}
	}
	ParamWidget::onChange(e);
}

void BasicSwitch::onButton(const ButtonEvent& e) {
	if (hasContextMenu || (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT && (e.mods & RACK_MOD_MASK) == 0)) {
		SvgSwitch::onButton(e);
	}
}

void BasicSwitch::step() {
	if (previousPreferDarkPanels != settings::preferDarkPanels) {
		fb->setDirty();
		previousPreferDarkPanels = settings::preferDarkPanels;
	}
	SvgSwitch::step();
}

BasicRoundSwitch::BasicRoundSwitch() {
	hasContextMenu = true;
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
		BasicSwitch::onButton(e);
	}
}

RoundSwitch::RoundSwitch() {
	addFrame("res/switches/Round_off.svg", "res/switches/Round_off-dark.svg");
	addFrame("res/switches/Round_on.svg", "res/switches/Round_on-dark.svg");
}

RoundSwitchMedium::RoundSwitchMedium() {
	addFrame("res/switches/RoundMedium_off.svg", "res/switches/RoundMedium_off-dark.svg");
	addFrame("res/switches/RoundMedium_on.svg", "res/switches/RoundMedium_on-dark.svg");
}

RoundSwitchRed::RoundSwitchRed() {
	addFrame("res/switches/Round_off.svg", "res/switches/Round_off-dark.svg");
	addFrame("res/switches/Round_red.svg", "res/switches/Round_red-dark.svg");
}

RoundSwitchMediumRed::RoundSwitchMediumRed() {
	addFrame("res/switches/RoundMedium_off.svg", "res/switches/RoundMedium_off-dark.svg");
	addFrame("res/switches/RoundMedium_red.svg", "res/switches/RoundMedium_red-dark.svg");
}

RoundSwitchLinearJump::RoundSwitchLinearJump() {
	addFrame("res/switches/Round_linear.svg", "res/switches/Round_linear-dark.svg");
	addFrame("res/switches/Round_linear_jump.svg", "res/switches/Round_linear_jump-dark.svg");
}

RoundSwitchLarge::RoundSwitchLarge() {
	hasContextMenu = true;
	addFrame("res/switches/RoundLarge_off.svg", "res/switches/RoundLarge_off-dark.svg");
	addFrame("res/switches/RoundLarge_on.svg", "res/switches/RoundLarge_on-dark.svg");
}

PauseSwitch::PauseSwitch() {
	addFrame("res/switches/Pause_off.svg", "res/switches/Pause_off-dark.svg");
	addFrame("res/switches/Pause_on.svg", "res/switches/Pause_on-dark.svg");
}

MidiButtonSmall::MidiButtonSmall() {
	addFrame("res/switches/MidiButtonSmall.svg", "res/switches/MidiButtonSmall-dark.svg");
}

struct MidiDriverValueItem : ui::MenuItem {
	midi::Port* port;
	int driverId;
	void onAction(const ActionEvent& e) override {
		port->setDriverId(driverId);
	}
};

struct MidiDeviceValueItem : ui::MenuItem {
	midi::Port* port;
	int deviceId;
	void onAction(const ActionEvent& e) override {
		port->setDeviceId(deviceId);
	}
};

void MidiButtonSmall::setMidiPort(midi::Port* port) {
	this->port = port;
}

void MidiButtonSmall::onDragStart(const DragStartEvent& e) {
	ui::Menu* menu = createMenu();
	menu->addChild(createMenuLabel("MIDI driver"));
	if (port) {
		for (int driverId : midi::getDriverIds()) {
			MidiDriverValueItem* item = new MidiDriverValueItem;
			item->port = port;
			item->driverId = driverId;
			item->text = midi::getDriver(driverId)->getName();
			item->rightText = CHECKMARK(item->driverId == port->getDriverId());
			menu->addChild(item);
		}
	}
	menu->addChild(new ui::MenuSeparator);
	menu->addChild(createMenuLabel("MIDI device"));
	if (port) {
		MidiDeviceValueItem* item = new MidiDeviceValueItem;
		item->port = port;
		item->deviceId = -1;
		item->text = "(No device)";
		item->rightText = CHECKMARK(item->deviceId == port->getDeviceId());
		menu->addChild(item);
		for (int deviceId : port->getDeviceIds()) {
			MidiDeviceValueItem* item = new MidiDeviceValueItem;
			item->port = port;
			item->deviceId = deviceId;
			item->text = port->getDeviceName(deviceId);
			item->rightText = CHECKMARK(item->deviceId == port->getDeviceId());
			menu->addChild(item);
		}
	}
}
