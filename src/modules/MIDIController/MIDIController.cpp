#include "MIDIController.hpp"

MIDIControllerOutput::MIDIControllerOutput() {
	frame = SEND_MIDI_MESSAGE_NOW;
	reset();
}

void MIDIControllerOutput::reset() {
	Output::reset();
}

void MIDIControllerOutput::setValue(uint8_t cc, uint8_t value) {
	midi::Message message;
	message.setStatus(0xb);
	message.setNote(cc);
	message.setValue(value);
	message.setFrame(frame);
	sendMessage(message);
}

void MIDIControllerOutput::setFrame(int64_t frame) {
	this->frame = frame;
}

MIDIController::MIDIController() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	
	configParam(MOVEMENT_ADJUST_PARAM,  0.1f, 1.9f, 1, "Adjust Movement");
	configParam(TEST_PARAM,  0, 10, 0, "Test Parameter");
	configInput(TEST_INPUT, "Test");
	configOutput(TEST_OUTPUT, "Test");
	onReset();
}

void MIDIController::process(const ProcessArgs& args) {
	midi::Message msg;
	while (midiInput.tryPop(&msg, args.frame)) {
		processMidiMessage(msg);
	}
	for (auto i = 0; i < MAX_MIDI_CC; ++i) {
		if (ccData[i] != CC_NO_VALUE) {
			int delta = ccData[i] - ccDataOld[i];
			if (delta == 0) {
				if (ccData[i] == 127) {
					delta++;
				} else if (ccData[i] == 0) {
					delta--;
				}
			}
			if (delta < 0) {
				cvData[i] -= params[MOVEMENT_ADJUST_PARAM].getValue() * 250.f / (args.frame - lastFrame[i]);
			} else if (delta > 0) {
				cvData[i] += params[MOVEMENT_ADJUST_PARAM].getValue() * 250.f / (args.frame - lastFrame[i]);
			}
			cvData[i] = clamp(cvData[i], 0.f, 10.f);
			controllerCorrected[i] = false;
			ccDataOld[i] = ccData[i];
			ccData[i] = CC_NO_VALUE;
			lastFrame[i] = args.frame;
			params[TEST_PARAM].setValue(cvData[i]);
			outputs[TEST_OUTPUT].setVoltage(cvData[i]);
		} else {
			if (i == 10 && params[TEST_PARAM].getValue() != cvData[i]) {
				cvData[i] = params[TEST_PARAM].getValue();
				outputs[TEST_OUTPUT].setVoltage(cvData[i]);
				controllerCorrected[i] = false;
			}
			if (!controllerCorrected[i] && (args.frame - lastFrame[i]) > 5000) {
				int actualCC = round(cvData[i] * 12.7f);
				if (actualCC != ccDataOld[i]) {
					midiOutput.setFrame(args.frame);
					midiOutput.setValue(i, actualCC);
					ccDataOld[i] = actualCC;
					controllerCorrected[i] = true;
				}				
			}
		}
	}
}

void MIDIController::processMidiMessage(const midi::Message& msg) {
	if (msg.getStatus() == 0xb) {
		ccData[msg.getNote()] = msg.getValue();
	}
	// DEBUG("size %i", msg.getSize());
}

void MIDIController::onReset() {
	for (auto i = 0; i < MAX_MIDI_CC; ++i) {
		ccData[i] = CC_NO_VALUE;
		ccDataOld[i] = CC_NO_VALUE;
		cvData[i] = 0;
		controllerCorrected[i] = true;
		cvSlews[i].setRiseFall(SLEW_VALUE, SLEW_VALUE);
		cvSlews[i].reset();
	}
	midiInput.reset();
	midiOutput.reset();
	midiOutput.midi::Output::reset();
}

json_t* MIDIController::dataToJson() {
	json_t* rootJ = json_object();
	json_object_set_new(rootJ, "midi-input", midiInput.toJson());
	json_object_set_new(rootJ, "midi-output", midiOutput.toJson());
	return rootJ;
}

void MIDIController::dataFromJson(json_t* rootJ) {
	json_t* midiJ = json_object_get(rootJ, "midi-input");
	if (midiJ) {
		midiInput.fromJson(midiJ);
	}
	midiJ = json_object_get(rootJ, "midi-output");
	if (midiJ) {
		midiOutput.fromJson(midiJ);
	}
}

