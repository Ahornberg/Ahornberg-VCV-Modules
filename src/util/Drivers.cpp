#include "../Ahornberg.hpp"

extern MIDIOverAudioDriver* MIDIOverAudioDriver::driver;

MIDIOverAudioInputDevice::MIDIOverAudioInputDevice(int id) {
	deviceId = id;
}

MIDIOverAudioDriver::MIDIOverAudioDriver() {
	if (MIDIOverAudioDriver::driver == nullptr) {
		MIDIOverAudioDriver::driver = this;
		midi::addDriver(MIDI_OVER_AUDIO_DRIVER_ID, this);
		for (auto i = 0; i < MIDI_OVER_AUDIO_MAX_DEVICES; ++i) {
			MIDIOverAudioInputDevice* device = new MIDIOverAudioInputDevice(i);
			devices.push_back(*device);
		}
	}
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
	if (deviceId < 0) {
		return "";
	}
	return "Port " + std::to_string(deviceId + 1);
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
