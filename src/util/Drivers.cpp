#include "../Ahornberg.hpp"

MIDIOverAudioToCVInputDevice::MIDIOverAudioToCVInputDevice(int id) {
	deviceId = id;
}

MIDIOverAudioToCVDriver::MIDIOverAudioToCVDriver() {
	MIDIOverAudioToCVInputDevice* device = new MIDIOverAudioToCVInputDevice(0);
	devices.push_back(*device);
}

std::string MIDIOverAudioToCVDriver::getName() {
	return "MIDI over Audio";
}

std::vector<int> MIDIOverAudioToCVDriver::getInputDeviceIds() {
	std::vector<int> deviceIds;
	for (auto &i : devices) {
		deviceIds.push_back(i.deviceId);
	}
	return deviceIds;
}

std::string MIDIOverAudioToCVDriver::getInputDeviceName(int deviceId) {
	return std::to_string(deviceId);
}

midi::InputDevice* MIDIOverAudioToCVDriver::subscribeInput(int deviceId, midi::Input* input) {
	if (deviceId >= (int) devices.size()) {
		return NULL;
	}
	devices[deviceId].subscribe(input);
	return &devices[deviceId];
}

void MIDIOverAudioToCVDriver::unsubscribeInput(int deviceId, midi::Input* input) {
	if (deviceId >= (int) devices.size()) {
		return;
	}
	devices[deviceId].unsubscribe(input);
}

int MIDIOverAudioToCVDriver::addInputDevice() {
	int id = devices.size();
	MIDIOverAudioToCVInputDevice* device = new MIDIOverAudioToCVInputDevice(id);
	devices.push_back(*device);
	return id;
}
