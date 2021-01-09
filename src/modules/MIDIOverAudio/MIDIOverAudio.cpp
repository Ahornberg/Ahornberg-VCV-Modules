#include "MIDIOverAudio.hpp"

// static MIDIOverAudioDriver* midiOverAudioDriver;

MIDIOverAudio::MIDIOverAudio() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	
	for (auto i = 0; i < MIDI_OVER_AUDIO_MAX_DEVICES; ++i) {
		resetMessageData(i);
	}
}

void MIDIOverAudio::process(const ProcessArgs& args) {
	for (auto i = 0; i < MIDI_OVER_AUDIO_MAX_DEVICES; ++i) {
		if (!inputs[MIDI_OVER_AUDIO_INPUT + i].isConnected()) {
			resetMessageData(i);
		} else {
			int dataInput = (int) (inputs[MIDI_OVER_AUDIO_INPUT + i].getVoltage() * 51.01f);
			if (dataInput > 0 && midiOverAudioMessages[i].messageDataPos == 0) {
				midiOverAudioMessages[i].messageData[0] = dataInput;
				++midiOverAudioMessages[i].messageDataPos;
			} else if (dataInput >= 0 && midiOverAudioMessages[i].messageDataPos > 0 && midiOverAudioMessages[i].messageDataPos < MIDI_MESSAGE_DATA_MAX_LENGTH) {
				midiOverAudioMessages[i].messageData[midiOverAudioMessages[i].messageDataPos] = dataInput;
				++midiOverAudioMessages[i].messageDataPos;
			} else if (dataInput < 0) {
				if (!MIDIOverAudioDriver::driver->devices[i].subscribed.empty()) {
					midi::Message msg;
					// TODO messages != 3 bytes
					for (auto j = 0; j < MIDI_MESSAGE_DATA_MAX_LENGTH; ++j) {
						msg.bytes[j] = midiOverAudioMessages[i].messageData[j];
					}
					MIDIOverAudioDriver::driver->devices[i].onMessage(msg);
				}
				resetMessageData(i);
			}
		}
	}
}

void  MIDIOverAudio::resetMessageData(int deviceId) {
	for (auto i = 0; i < MIDI_MESSAGE_DATA_MAX_LENGTH; ++i) {
		midiOverAudioMessages[deviceId].messageData[i] = 0;
	}
	midiOverAudioMessages[deviceId].messageDataPos = 0;
}
