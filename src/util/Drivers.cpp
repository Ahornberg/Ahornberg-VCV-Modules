#include "../Ahornberg.hpp"

extern MIDIOverAudioDriver* MIDIOverAudioDriver::driver;

MIDIOverAudioInputDevice::MIDIOverAudioInputDevice(int id) {
	deviceId = id;
}

MIDIOverAudioDriver::MIDIOverAudioDriver() {
	enabled = loadMIDIOverAudioDriverEnabled();
	if (MIDIOverAudioDriver::driver == nullptr) {
		MIDIOverAudioDriver::driver = this;
	} else {
		MIDIOverAudioDriver::driver->enabled = enabled;
	}
	if (enabled) {
		midi::addDriver(MIDI_OVER_AUDIO_DRIVER_ID, MIDIOverAudioDriver::driver);
		for (auto i = 0; i < MIDI_OVER_AUDIO_MAX_DEVICES; ++i) {
			MIDIOverAudioInputDevice* device = new MIDIOverAudioInputDevice(i);
			MIDIOverAudioDriver::driver->devices.push_back(*device);
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

void saveMIDIOverAudioDriverEnabled(bool enabled) {
    json_t* settingsJ = json_object();
    json_object_set_new(settingsJ, "midiOverAudioEnabled", json_boolean(enabled));
    std::string settingsFilename = asset::user("Ahornberg.json");
    FILE* file = fopen(settingsFilename.c_str(), "w");
    if (file) {
        json_dumpf(settingsJ, file, JSON_INDENT(2) | JSON_REAL_PRECISION(9));
        fclose(file);
    }
    json_decref(settingsJ);
}

bool loadMIDIOverAudioDriverEnabled() {
    bool ret = false;
    std::string settingsFilename = asset::user("Ahornberg.json");
    FILE* file = fopen(settingsFilename.c_str(), "r");
    if (!file) {
        saveMIDIOverAudioDriverEnabled(false);
        return ret;
    }
    json_error_t error;
    json_t* settingsJ = json_loadf(file, 0, &error);
    if (!settingsJ) {
        // invalid setting json file
        fclose(file);
        saveMIDIOverAudioDriverEnabled(false);
        return ret;
    }
    json_t* qualityJ = json_object_get(settingsJ, "midiOverAudioEnabled");
    if (qualityJ)
        ret = json_boolean_value(qualityJ);

    fclose(file);
    json_decref(settingsJ);
    return ret;
}

