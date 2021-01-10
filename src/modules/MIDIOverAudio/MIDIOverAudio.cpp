#include "MIDIOverAudio.hpp"

// static MIDIOverAudioDriver* midiOverAudioDriver;

MIDIOverAudio::MIDIOverAudio() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	
	for (auto i = 0; i < MIDI_OVER_AUDIO_MAX_DEVICES; ++i) {
		configParam(TRIM_PARAM + i, -5, 5, 0, "Trim Port " + std::to_string(i + 1));
		resetMessageData(i);
	}
}

void MIDIOverAudio::process(const ProcessArgs& args) {
	for (auto i = 0; i < MIDI_OVER_AUDIO_MAX_DEVICES; ++i) {
		if (!inputs[MIDI_OVER_AUDIO_INPUT + i].isConnected()) {
			resetMessageData(i);
		} else {
			float dataInputFloat = inputs[MIDI_OVER_AUDIO_INPUT + i].getVoltage() * (params[TRIM_PARAM + i].getValue() * 0.112f + 5.112f * 5);
			if (dataInputFloat < 0) {
				if (isNear(dataInputFloat, -255.56f, 0.44f)) {
					lights[RGB_LIGHT + i * 3 + 0].setBrightness(0);
					lights[RGB_LIGHT + i * 3 + 1].setBrightness(1);
				} else if (isNear(dataInputFloat, -255.56f, 1.5f)) {
					lights[RGB_LIGHT + i * 3 + 0].setBrightness(1);
					lights[RGB_LIGHT + i * 3 + 1].setBrightness(1);
				} else  if (isNear(dataInputFloat, -255.56f, 3)) {
					lights[RGB_LIGHT + i * 3 + 0].setBrightness(1);
					lights[RGB_LIGHT + i * 3 + 1].setBrightness(0.5f);
				} else {
					lights[RGB_LIGHT + i * 3 + 0].setBrightness(1);
					lights[RGB_LIGHT + i * 3 + 1].setBrightness(0);				
				}
			}
			int dataInput = (int) dataInputFloat;
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
