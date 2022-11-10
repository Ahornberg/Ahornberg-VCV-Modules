#include "MIDIOverAudio.hpp"

// static MIDIOverAudioDriver* midiOverAudioDriver;

MIDIOverAudio::MIDIOverAudio() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	
	for (auto i = 0; i < MIDI_OVER_AUDIO_MAX_DEVICES; ++i) {
		configParam(TRIM_PARAM + i, -5, 5, 0, "Trim MIDI over Audio Port " + std::to_string(i + 1));
		resetMessageData(i);
		getParamQuantity(TRIM_PARAM + i)->randomizeEnabled = false;
		lightsSlewLimiter[i * 3 + 0].setRiseFall(0, 1);
		lightsSlewLimiter[i * 3 + 1].setRiseFall(0, 3);
		lightsSlewLimiter[i * 3 + 2].setRiseFall(0, 2);
		std::string portLabel = "MIDI over Audio Port " + std::to_string(i + 1);
		configInput(MIDI_OVER_AUDIO_INPUT + i, portLabel);
		//configLight(RGB_LIGHT + i * 3, portLabel);
	}
	new MIDIOverAudioDriver(true);
}

void MIDIOverAudio::process(const ProcessArgs& args) {
	if (MIDIOverAudioDriver::driver->enabled) {
		for (auto device = 0; device < MIDI_OVER_AUDIO_MAX_DEVICES; ++device) {
			if (!inputs[MIDI_OVER_AUDIO_INPUT + device].isConnected()) {
				resetMessageData(device);
				lights[RGB_LIGHT + device * 3 + 0].setBrightness(0);
				lights[RGB_LIGHT + device * 3 + 1].setBrightness(0);
				lights[RGB_LIGHT + device * 3 + 2].setBrightness(0);
			} else {
				float dataInputFloat = inputs[MIDI_OVER_AUDIO_INPUT + device].getVoltage() * (params[TRIM_PARAM + device].getValue() * 0.112f + 5.112f * 5);
				float red = 0;
				float green = 0;
				float blue = 0;
				if (dataInputFloat < 0) {
					lightsSlewLimiter[device * 3 + 0].reset();
					lightsSlewLimiter[device * 3 + 1].reset();
					lightsSlewLimiter[device * 3 + 2].reset();
					if (isNear(dataInputFloat, -255.56f, 0.44f)) {
						green = 1;
					} else if (isNear(dataInputFloat, -255.56f, 1.5f)) {
						red = 1;
						green = 1;
					} else  if (isNear(dataInputFloat, -255.56f, 3)) {
						red = 1;
						green = 0.5f;
					} else if (dataInputFloat > -255.56f) {
						blue = 1;
					} else {
						red = 1;
					}
				}
				red = lightsSlewLimiter[device * 3 + 0].process(args.sampleTime, red);
				green = lightsSlewLimiter[device * 3 + 1].process(args.sampleTime, green);
				blue = lightsSlewLimiter[device * 3 + 2].process(args.sampleTime, blue);
				lights[RGB_LIGHT + device * 3 + 0].setBrightness(red);
				lights[RGB_LIGHT + device * 3 + 1].setBrightness(green);
				lights[RGB_LIGHT + device * 3 + 2].setBrightness(blue);
				int dataInput = (int) dataInputFloat;
				if (dataInput > 0 && midiOverAudioMessages[device].messageDataPos == 0) {
					midiOverAudioMessages[device].messageData[0] = dataInput;
					++midiOverAudioMessages[device].messageDataPos;
				} else if (dataInput >= 0 && midiOverAudioMessages[device].messageDataPos > 0 && midiOverAudioMessages[device].messageDataPos < MIDI_MESSAGE_DATA_MAX_LENGTH) {
					midiOverAudioMessages[device].messageData[midiOverAudioMessages[device].messageDataPos] = dataInput;
					++midiOverAudioMessages[device].messageDataPos;
				} else if (dataInput < 0) {
					if (!MIDIOverAudioDriver::driver->devices[device].subscribed.empty()) {
						midi::Message message;
						// TODO messages != 3 bytes
						for (auto i = 0; i < MIDI_MESSAGE_DATA_MAX_LENGTH; ++i) {
							message.bytes[i] = midiOverAudioMessages[device].messageData[i];
						}
						MIDIOverAudioDriver::driver->devices[device].onMessage(message);
					}
					resetMessageData(device);
				}
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

void MIDIOverAudio::panic() {
	// TODO in Rack V2: change to all-notes-off messages
	if (MIDIOverAudioDriver::driver->enabled) {
		for (auto device = 0; device < MIDI_OVER_AUDIO_MAX_DEVICES; ++device) {
			for (auto channel = 0; channel < 16; ++channel) {
				for (auto note = 0; note < 128; ++note) {
					midi::Message message;
					message.bytes[0] = 128 + channel;
					message.bytes[1] = note;
					message.bytes[2] = 0;
					MIDIOverAudioDriver::driver->devices[device].onMessage(message);
				}
			}
		}
	}
}
