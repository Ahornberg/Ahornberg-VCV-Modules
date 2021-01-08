#include "MIDIOverAudio.hpp"

static MIDIOverAudioDriver* midiOverAudioDriver;

MIDIOverAudio::MIDIOverAudio() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	if (midiOverAudioDriver == nullptr) {
		midiOverAudioDriver = new MIDIOverAudioDriver;
		midi::addDriver(AUDIO_OVER_MIDI_DRIVER_ID, midiOverAudioDriver);
		deviceId = 0;
	} else {
		deviceId = midiOverAudioDriver->addInputDevice();
	}
	for (auto i = 0; i < MIDI_MESSAGE_DATA_MAX_LENGTH; ++i) {
		messageData[i] = 0;
	}
	messageDataPos = 0;
}

MIDIOverAudio::~MIDIOverAudio() { 
// TODO delete device - in driver, mark deleted devices for new assignment - show ID on widget 
}

void MIDIOverAudio::process(const ProcessArgs& args) {
	int dataInput = (int) (inputs[PITCH_INPUT].getVoltage() * 51.01f);
	if (dataInput > 0 && messageDataPos == 0) {
		messageData[0] = dataInput;
		++messageDataPos;
	} else if (dataInput >= 0 && messageDataPos > 0 && messageDataPos < MIDI_MESSAGE_DATA_MAX_LENGTH) {
		messageData[messageDataPos] = dataInput;
		++messageDataPos;
	} else if (dataInput < 0) {
		if (!midiOverAudioDriver->devices[deviceId].subscribed.empty()) {
			midi::Message msg;
			for (auto i = 0; i < MIDI_MESSAGE_DATA_MAX_LENGTH; ++i) {
				msg.bytes[i] = messageData[i];
			}
			midiOverAudioDriver->devices[deviceId].onMessage(msg);
		}
		for (auto i = 0; i < MIDI_MESSAGE_DATA_MAX_LENGTH; ++i) {
			messageData[i] = 0;
		}
		messageDataPos = 0;
	}
}
