constexpr static int MIDI_OVER_AUDIO_DRIVER_ID = -99;
constexpr static int MIDI_OVER_AUDIO_MAX_DEVICES = 8;
constexpr static int MIDI_MESSAGE_DATA_MAX_LENGTH = 3;

struct MIDIOverAudioInputDevice : midi::InputDevice {
	int deviceId;
	
	MIDIOverAudioInputDevice(int id);
};

struct MIDIOverAudioDriver : midi::Driver {
	static MIDIOverAudioDriver* driver;
	
	std::vector<MIDIOverAudioInputDevice> devices;
	
	MIDIOverAudioDriver();
	std::string getName() override;
	std::vector<int> getInputDeviceIds() override;
	std::string getInputDeviceName(int deviceId) override;
	midi::InputDevice* subscribeInput(int deviceId, midi::Input* input) override;
	void unsubscribeInput(int deviceId, midi::Input* input) override;
	int addInputDevice();
};
