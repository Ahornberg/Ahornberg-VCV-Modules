#include "../Ahornberg.hpp"

MIDIOverAudioInputDevice::MIDIOverAudioInputDevice(int id) {
	deviceId = id;
}

MIDIOverAudioDriver::MIDIOverAudioDriver() {
	MIDIOverAudioInputDevice* device = new MIDIOverAudioInputDevice(0);
	devices.push_back(*device);
}

std::string MIDIOverAudioDriver::getName() {
	return "MIDI over Audio";
}

std::vector<int> MIDIOverAudioDriver::getInputDeviceIds() {
	std::vector<int> deviceIds;
	for (auto &i : devices) {
		deviceIds.push_back(i.deviceId);
	}
	return deviceIds;
}

std::string MIDIOverAudioDriver::getInputDeviceName(int deviceId) {
	return std::to_string(deviceId);
}

midi::InputDevice* MIDIOverAudioDriver::subscribeInput(int deviceId, midi::Input* input) {
	if (deviceId >= (int) devices.size()) {
		return NULL;
	}
	devices[deviceId].subscribe(input);
	return &devices[deviceId];
}

void MIDIOverAudioDriver::unsubscribeInput(int deviceId, midi::Input* input) {
	if (deviceId >= (int) devices.size()) {
		return;
	}
	devices[deviceId].unsubscribe(input);
}

int MIDIOverAudioDriver::addInputDevice() {
	int id = devices.size();
	MIDIOverAudioInputDevice* device = new MIDIOverAudioInputDevice(id);
	devices.push_back(*device);
	return id;
}
