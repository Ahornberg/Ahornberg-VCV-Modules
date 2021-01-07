constexpr static int AUDIO_OVER_MIDI_DRIVER_ID = -99;
constexpr static int MIDI_MESSAGE_DATA_MAX_LENGTH = 3;

struct MIDIOverAudioToCVInputDevice : midi::InputDevice {
	int deviceId;
	
	MIDIOverAudioToCVInputDevice(int id);
};

struct MIDIOverAudioToCVDriver : midi::Driver {
	std::vector<MIDIOverAudioToCVInputDevice> devices;
	
	MIDIOverAudioToCVDriver();
	std::string getName() override;
	std::vector<int> getInputDeviceIds() override;
	std::string getInputDeviceName(int deviceId) override;
	midi::InputDevice* subscribeInput(int deviceId, midi::Input* input) override;
	void unsubscribeInput(int deviceId, midi::Input* input) override;
	int addInputDevice();
};
