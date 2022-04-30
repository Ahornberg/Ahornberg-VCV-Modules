#include "TapeRecorder.hpp"

const std::string TapeRecorder::INIT_TAPE_NAME = "My Magic Tape";
const TapeLength TapeRecorder::TAPE_LENGTHS[] = {
	{ 44100 * 60 *   1, "MC 1" },
	{ 44100 * 60 *   3, "MC 3" },
	{ 44100 * 60 *   6, "MC 6" },
	{ 44100 * 60 *  10, "MC 10" },
	{ 44100 * 60 *  15, "MC 15" },
	{ 44100 * 60 *  20, "MC 20" },
	{ 44100 * 60 *  30, "MC 30" },
	{ 44100 * 60 *  45, "MC 45" },
	{ 44100 * 60 *  60, "MC 60" },
	{ 44100 * 60 *  90, "MC 90" },
	{ 44100 * 60 * 120, "MC 120" }
};

TapeRecorder::TapeRecorder() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configButton(PAUSE_PARAM, "Pause");
	configButton(PLAY_FORWARDS_PARAM, "Play");
	configButton(PLAY_BACKWARDS_PARAM, "Play Reverse");
	configButton(CUE_FORWARDS_PARAM, "Fast Forward");
	configButton(CUE_BACKWARDS_PARAM, "Rewind");
	configParam(CUE_SPEED_PARAM,           0.25, 20,   3, "Fast Speed");
	configParam(CUE_SLEW_PARAM,            1,    20,  15, "Motor Power");
	configParam(TEMPO_PARAM,               1,   360, 120, "BPM");
	configParam(BEATS_PER_BAR_PARAM,       1,    48,   4, "Beats / Bar");
	configParam(LOOP_START_PARAM,          0,    99,   0, "Loop Start on Bar");
	configParam(LOOP_END_PARAM,            0,    99,   0, "Loop End on Bar");
	// configParam(LOOP_START_BUTTON_PARAM,   0,     1,   0, "Loop Start on Tape Position");
	// configParam(LOOP_END_BUTTON_PARAM,     0,     1,   0, "Loop End on Tape Position");
	configParam(LOOP_MODE_PARAM,           0,     1,   1, "Loop Mode");
	configParam(WHEEL_LEFT_PARAM,        -12.4,  11.6, 0, "Left Wheel");
	configParam(WHEEL_RIGHT_PARAM,       -11.9,  12.1, 0, "Right Wheel");
	configParam(TAPE_LENGTH_PARAM,         0, NUM_TAPE_LENGTHS - 1, 0, "Tape Length");
	configParam(TRACK_COUNT_PARAM,         1,    16,   1, "Tape Layout");
	getParamQuantity(PAUSE_PARAM)->randomizeEnabled = false;
	getParamQuantity(PLAY_FORWARDS_PARAM)->randomizeEnabled = false;
	getParamQuantity(PLAY_BACKWARDS_PARAM)->randomizeEnabled = false;
	getParamQuantity(CUE_FORWARDS_PARAM)->randomizeEnabled = false;
	getParamQuantity(CUE_BACKWARDS_PARAM)->randomizeEnabled = false;
	getParamQuantity(LOOP_MODE_PARAM)->randomizeEnabled = false;
	getParamQuantity(WHEEL_LEFT_PARAM)->randomizeEnabled = false;
	getParamQuantity(WHEEL_RIGHT_PARAM)->randomizeEnabled = false;
	getParamQuantity(TAPE_LENGTH_PARAM)->randomizeEnabled = false;
	getParamQuantity(TRACK_COUNT_PARAM)->randomizeEnabled = false;
	configInput(AUDIO_INPUT, "Audio");
	configInput(LOOP_START_INPUT, "Loop Start");
	configInput(LOOP_END_INPUT, "Loop End");
	configInput(SPEED_INPUT, "Speed (1V/Octave)");
	configInput(TRANSPORT_INPUT, "Transport Trigger");
	configOutput(AUDIO_OUTPUT, "Audio");
	configOutput(SPEED_OUTPUT, "Speed (1V/Octave)");
	configOutput(TEMPO_OUTPUT, "Tempo Trigger");
	configOutput(TRANSPORT_OUTPUT, "Transport");
	configBypass(AUDIO_INPUT, AUDIO_OUTPUT);
	configBypass(SPEED_INPUT, SPEED_OUTPUT);
	configBypass(TRANSPORT_INPUT, TRANSPORT_OUTPUT);
	tapeName = INIT_TAPE_NAME;
	stripeIndex = (int) (random::uniform() * 12);
	displayDivider.setDivision(512);
	changeTapeInterrupt = false;
	tapeStoppedAndResetted = true;
	audioBuffer = nullptr;
	initTape();
}

TapeRecorder::~TapeRecorder() {
	delete audioBuffer;
}

void TapeRecorder::initTape() {
	changeTapeInterrupt = true;
	// DEBUG("length %d, type %d", (int) params[TAPE_LENGTH_PARAM].getValue(), (int) params[TRACK_COUNT_PARAM].getValue());
	// while (!tapeStoppedAndResetted) {
		
	// }
	// init
	sizeAudioBuffer = TAPE_LENGTHS[(int) params[TAPE_LENGTH_PARAM].getValue()].value;
	if (audioBuffer) {
		delete audioBuffer;
	}
	audioBuffer = new float[sizeAudioBuffer * (int) params[TRACK_COUNT_PARAM].getValue()];
	eraseTape();
	playStatus = false;
	cueStatus = false;
	playForwardStatus = false;
	cueForwardStatus = false;
	audioBufferPosition = 0.;
	loopStartPosition = 0;
	loopEndPosition = 0;
	tapePosition = 0;
	tapeLengthInMinutes = sizeAudioBuffer / (44100 * 60);
	displayDivider.reset();
	calcTapeAndPositionsOnWheels(true);
	params[WHEEL_LEFT_PARAM].setValue(positionLeftWheel);
	params[WHEEL_RIGHT_PARAM].setValue(positionRightWheel);
	lastAudioBufferLocation = -1;
	beat = 0;
	bar = 0;
	barsPulse.reset();
	beatsPulse.reset();
	tapeSpeed = 0;
	touchedWheelForce = 1;
	speedSlewLimiter.setRiseFall(params[CUE_SLEW_PARAM].getValue(), 20);
	wheelMovementSlewLimiter.setRiseFall(40, 40);
	wheelMovement = 0.f;
	pauseInputTrigger.reset();
	playForwardsInputTrigger.reset();
	playBackwardsInputTrigger.reset();
	cueForwardsInputTrigger.reset();
	cueBackwardsInputTrigger.reset();
	cueForwardsMomentaryInputTrigger.reset();
	cueBackwardsMomentaryInputTrigger.reset();
	tapeStatus = TAPE_BEGIN;
	for (auto i = 0; i < NUM_MAX_TRACKS; ++i) {
		lastDistortionLevelInput[i] = 10;
		lastDistortionLevelValue[i] = 0;
	}

	dataFromJsonCalled = false;
	
	changeTapeInterrupt = false;
}

void TapeRecorder::eraseTape() {
	for (auto i = 0; i < sizeAudioBuffer; ++i) {
		audioBuffer[i] = 0.f;
	}
}

void TapeRecorder::calcTapeAndPositionsOnWheels(bool always) {
	if (displayDivider.process() || always) {
		tapeOnLeftWheel = (1.0 - ((pow((1.0 - (audioBufferPosition / sizeAudioBuffer)) * 21.0 + 14.0, 2.0) - 196.0) / 1029.0));
		tapeOnRightWheel = (1.0 - ((pow((audioBufferPosition / sizeAudioBuffer) * 21.0 + 14.0, 2.0) - 196.0) / 1029.0));
		positionLeftWheel = fmod(tapeOnLeftWheel * 120 * 4 * tapeLengthInMinutes, 24);
		positionRightWheel = fmod(tapeOnRightWheel * -120 * 4 * tapeLengthInMinutes, 24);
	}
}

void TapeRecorder::processTempoOutput(const ProcessArgs& args) {
	if (outputs[TEMPO_OUTPUT].isConnected() && playStatus) {
		float beat = 0;
		if (beatsPulse.process(args.sampleTime)) {
			beat = 5;
		}
		if (barsPulse.process(args.sampleTime)) {
			beat += 5;
		}
		outputs[TEMPO_OUTPUT].setVoltage(beat);
	}
}

void TapeRecorder::processSpeedInput(const ProcessArgs& args) {
	if (inputs[SPEED_INPUT].isConnected()) {
		tapeSpeed *= exp2(inputs[SPEED_INPUT].getVoltage());
	}
	tapeSpeed *= touchedWheelForce;
	speedSlewLimiter.setRiseFall(params[CUE_SLEW_PARAM].getValue(), 20);
	// tapeSpeed = speedSlewLimiter.process(args.sampleTime, tapeSpeed);
}

void TapeRecorder::processSpeedOutput() {
	if (outputs[SPEED_OUTPUT].isConnected()) {
		if (tapeSpeed > 0) {
			outputs[SPEED_OUTPUT].setVoltage(log2(tapeSpeed));
		} else if (tapeSpeed < 0) {
			outputs[SPEED_OUTPUT].setVoltage(log2(tapeSpeed * -1.f));
		} else {
			outputs[SPEED_OUTPUT].setVoltage(0);
		}
	}
}

void TapeRecorder::processLoopInput() {
	if (params[LOOP_START_PARAM].getValue() != oldLoopStartParam) {
		// params[LOOP_START_BUTTON_PARAM].setValue(0.f);
		oldLoopStartParam = params[LOOP_START_PARAM].getValue();
	}
	if (inputs[LOOP_START_INPUT].isConnected()) {
		loopStart = (int) ((inputs[LOOP_START_INPUT].getVoltage() * 12.f) + 0.5f + params[LOOP_START_PARAM].getValue());
		if (loopStart < 0) {
			loopStart = 0;
		}
	} else {
		loopStart = params[LOOP_START_PARAM].getValue();
	}
	
	if (params[LOOP_END_PARAM].getValue() != oldLoopEndParam) {
		// params[LOOP_END_BUTTON_PARAM].setValue(0.f);
		oldLoopEndParam = params[LOOP_END_PARAM].getValue();
	}
	if (inputs[LOOP_END_INPUT].isConnected()) { // && inputs[LOOP_INPUT].getChannels() > 1) {
		loopEnd = (int) ((inputs[LOOP_END_INPUT].getVoltage() * 12.f) + 0.5f + params[LOOP_END_PARAM].getValue());
		if (loopEnd < 0) {
			loopEnd = 0;
		}
	} else {
		loopEnd = params[LOOP_END_PARAM].getValue();
	}
}

// void TapeRecorder::processLoopOutput(const ProcessArgs& args) {
	
// }

// void TapeRecorder::toggleParamValue(int param) {
	// params[param].getValue() ? params[param].setValue(0) : params[param].setValue(1);
// }

bool TapeRecorder::isTransportCueSwitchMomentary() {
	return params[PLAY_FORWARDS_PARAM].getValue() || params[PLAY_BACKWARDS_PARAM].getValue();
}

// float TapeRecorder::rescaleInput(int port, int channel) {
	// return rescale(inputs[port].getVoltage(channel), 0.1, 2, 0, 1);
// }

// float TapeRecorder::rescaleInverseInput(int port, int channel) {
	// return rescale(inputs[port].getVoltage(channel), 2, 0.1, 0, 1);
// }

void TapeRecorder::processTransportInput() {
	// TODO loop input
	if (inputs[TRANSPORT_INPUT].isConnected()) {
		if (pauseInputTrigger.process(rescaleInput(inputs[TRANSPORT_INPUT], 0))) {
			// pause is independend from all other transport keys
			toggleParamValue(params[PAUSE_PARAM]);
		}
		// if (playBackwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 1))) {
			// toggleParamValue(params[PLAY_BACKWARDS_PARAM]);
			// if (params[PLAY_BACKWARDS_PARAM].getValue()) {
				// // only one play direction active at a time
				// params[PLAY_FORWARDS_PARAM].setValue(0);
			// }
		// }
		// if (playForwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 3))) {
			// toggleParamValue(params[PLAY_FORWARDS_PARAM]);
			// if (params[PLAY_FORWARDS_PARAM].getValue()) {
				// // only one play direction active at a time
				// params[PLAY_BACKWARDS_PARAM].setValue(0);
			// }
		// }
		// if (cueBackwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 0))) {
			// toggleParamValue(params[CUE_BACKWARDS_PARAM]);
			// if (params[CUE_BACKWARDS_PARAM].getValue()) {
				// // only one cue direction active at a time
				// params[CUE_FORWARDS_PARAM].setValue(0);
			// }
		// }
		// if (cueBackwardsMomentaryInputTrigger.process(rescaleInverseInput(TRANSPORT_INPUT, 0))) {
			// if (isTransportCueSwitchMomentary()) {
				// params[CUE_BACKWARDS_PARAM].setValue(0);
			// }
		// }
		// if (cueForwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 4))) {
			// toggleParamValue(params[CUE_FORWARDS_PARAM]);
			// if (params[CUE_FORWARDS_PARAM].getValue()) {
				// // only one cue direction active at a time
				// params[CUE_BACKWARDS_PARAM].setValue(0);
			// }
		// }
		// if (cueForwardsMomentaryInputTrigger.process(rescaleInverseInput(TRANSPORT_INPUT, 4))) {
			// if (isTransportCueSwitchMomentary()) {
				// params[CUE_FORWARDS_PARAM].setValue(0);
			// }
		// }

	}
}

void TapeRecorder::processTransportOutput() {
	if (outputs[TRANSPORT_OUTPUT].isConnected()) {
		if (playStatus) {
			if (playForwardStatus) {
				outputs[TRANSPORT_OUTPUT].setVoltage(5);
			} else {
				outputs[TRANSPORT_OUTPUT].setVoltage(-5);
			}
		} else {
			outputs[TRANSPORT_OUTPUT].setVoltage(0);
		}
	}
}

void TapeRecorder::processAudioInput(const ProcessArgs& args) {
	
}

void TapeRecorder::processAudioOutput(const ProcessArgs& args) {
	
}

// process ****************************************************************

void TapeRecorder::process(const ProcessArgs& args) {
	if (changeTapeInterrupt) {
		params[PLAY_BACKWARDS_PARAM].setValue(0.);
		params[CUE_BACKWARDS_PARAM].setValue(0.);
		params[PLAY_FORWARDS_PARAM].setValue(0.);
		params[CUE_FORWARDS_PARAM].setValue(0.);
		audioBufferPosition = 0;
		tapeStoppedAndResetted = true;
		return;
	} else {
		tapeStoppedAndResetted = false;
	}

	auto trackCount = (int) params[TRACK_COUNT_PARAM].getValue();
	// tape begin/end reached
	if (audioBufferPosition <= 0) {
		tapeStatus = TAPE_BEGIN;
	} else if (audioBufferPosition >= sizeAudioBuffer) {
		tapeStatus = TAPE_END;
	} else {
		tapeStatus = TAPE_MIDDLE;
	}
	
	// locator left/right reached
	
	// loop input -> goto locator
	
	// transport input -> transport buttons clicked
	
	// GUI -> buttons clicked / knobs moved
	
	// menu -> old school mode / new tape
	
	// speed
	
	// process tape
	
	// tempo out
	
	// speed out
	
	// loop out
	
	// transport out
	
	processTransportInput();

	playStatus = params[PLAY_FORWARDS_PARAM].getValue() || params[PLAY_BACKWARDS_PARAM].getValue();
	playForwardStatus = !params[PLAY_BACKWARDS_PARAM].getValue();
	cueStatus = params[CUE_FORWARDS_PARAM].getValue() || params[CUE_BACKWARDS_PARAM].getValue();
	cueForwardStatus = !params[CUE_BACKWARDS_PARAM].getValue();
	if (tapeStatus == TAPE_BEGIN) {
		if (cueStatus && !cueForwardStatus) {
			cueStatus = false;
		}
		if (playStatus && !playForwardStatus) {
			playStatus = false;
		}
	} else if (tapeStatus == TAPE_END) {
		if (cueStatus && cueForwardStatus) {
			cueStatus = false;
		}
		if (playStatus && playForwardStatus) {
			playStatus = false;
		}
	}

	processLoopInput();
	processTempoOutput(args);
	processTransportOutput();

	tapeSpeed = args.sampleTime * args.sampleRate;//1.f;
	
	processSpeedInput(args);
	
	if ((!playStatus && !cueStatus) || (playStatus && params[PAUSE_PARAM].getValue() && !cueStatus)) {
		// TODO slew
		// tapeSpeed = 0.f;
		tapeSpeed = wheelMovementSlewLimiter.process(args.sampleTime, wheelMovement);
		// tapeSpeed = speedSlewLimiter.process(args.sampleTime, tapeSpeed);
	} else if (cueStatus) {
		tapeSpeed *= params[CUE_SPEED_PARAM].getValue();
		// tapeSpeed = speedSlewLimiter.process(args.sampleTime, tapeSpeed);
		if (!cueForwardStatus) {
			// TODO slew
			tapeSpeed *= -1.f;
		}
	} else if (!playForwardStatus) {
		// TODO slew
		tapeSpeed *= -1.f;
	}
	
	// if (tapeSpeed == 0) {
		// // if (positionLeftWheel != params[WHEEL_LEFT_PARAM].getValue()) {
			// tapeSpeed = wheelMovement;
		// // }
	// }
	
	audioBufferPosition += tapeSpeed;
	
	processSpeedOutput();

	if (audioBufferPosition >= 0 && audioBufferPosition <= sizeAudioBuffer) {
		tapeStatus = TAPE_MIDDLE;
		tapePosition = (args.sampleTime * audioBufferPosition * params[TEMPO_PARAM].getValue()) / (60 * params[BEATS_PER_BAR_PARAM].getValue());
		int newBar = (int) tapePosition;
		int newBeat = (int) ((tapePosition - bar) * params[BEATS_PER_BAR_PARAM].getValue());
		if (newBar != bar || tapePosition == 0) {
			barsPulse.trigger();
		}
		bar = newBar;
		if (newBeat != beat || tapePosition == 0) {
			beatsPulse.trigger();
		}
		beat = newBeat;
		// if (params[LOOP_START_BUTTON_PARAM].getValue() == 1.0f) {
			// loopStartPosition = audioBufferPosition;
		// } else {
			loopStartPosition = (args.sampleRate * loopStart * 60 * params[BEATS_PER_BAR_PARAM].getValue()) / params[TEMPO_PARAM].getValue();
		// }
		// if (params[LOOP_END_BUTTON_PARAM].getValue() == 1.0f) {
			// loopEndPosition = audioBufferPosition;
		// } else {
			loopEndPosition = (args.sampleRate * loopEnd * 60 * params[BEATS_PER_BAR_PARAM].getValue()) / params[TEMPO_PARAM].getValue();
		// }
	}
	if (params[LOOP_MODE_PARAM].getValue() > 0) {
		if (loopStart < loopEnd) {
			if (audioBufferPosition <= loopStartPosition && playStatus && !playForwardStatus) {
				toggleParamValue(params[PLAY_FORWARDS_PARAM]);
				toggleParamValue(params[PLAY_BACKWARDS_PARAM]);
			}
			if (audioBufferPosition > loopEndPosition && playStatus && playForwardStatus) {
				toggleParamValue(params[PLAY_FORWARDS_PARAM]);
				toggleParamValue(params[PLAY_BACKWARDS_PARAM]);
			}
		} else if (loopStart > loopEnd) {
			if (audioBufferPosition <= loopEndPosition && playStatus && !playForwardStatus) {
				toggleParamValue(params[PLAY_FORWARDS_PARAM]);
				toggleParamValue(params[PLAY_BACKWARDS_PARAM]);
			}
			if (audioBufferPosition > loopStartPosition && playStatus && playForwardStatus) {
				toggleParamValue(params[PLAY_FORWARDS_PARAM]);
				toggleParamValue(params[PLAY_BACKWARDS_PARAM]);
			}
		}
	} else {
		if (loopStart < loopEnd) {
			if (audioBufferPosition <= loopStartPosition && playStatus && !playForwardStatus) {
				audioBufferPosition = loopEndPosition;
			} else if (audioBufferPosition > loopEndPosition && playStatus && playForwardStatus) {
				audioBufferPosition = loopStartPosition;
			}
		} else if (loopStart > loopEnd) {
			if (audioBufferPosition <= loopEndPosition && playStatus && !playForwardStatus) {
				audioBufferPosition = loopStartPosition;
			} else if (audioBufferPosition > loopStartPosition && playStatus && playForwardStatus) {
				audioBufferPosition = loopEndPosition;
			}
		}
	}
	if (audioBufferPosition <= 0) {
		params[PLAY_BACKWARDS_PARAM].setValue(0.);
		params[CUE_BACKWARDS_PARAM].setValue(0.);
	} else if (audioBufferPosition >= sizeAudioBuffer) {
		params[PLAY_FORWARDS_PARAM].setValue(0.);
		params[CUE_FORWARDS_PARAM].setValue(0.);
	}
	
	if (audioBufferPosition <= 0) {
		tapeStatus = TAPE_BEGIN;
		if (outputs[AUDIO_OUTPUT].isConnected()) {
			outputs[AUDIO_OUTPUT].setChannels(trackCount);
			for (auto i = 0; i < trackCount; ++i) {
				outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage(i), i);
			}
		}
		audioBufferPosition = 0.;
		lastAudioBufferLocation = -1;
		calcTapeAndPositionsOnWheels(false);
		params[WHEEL_LEFT_PARAM].setValue(positionLeftWheel);
		params[WHEEL_RIGHT_PARAM].setValue(positionRightWheel);
		speedSlewLimiter.reset();
		return;
	}
	if (audioBufferPosition >= sizeAudioBuffer) {
		tapeStatus = TAPE_END;
		if (outputs[AUDIO_OUTPUT].isConnected()) {
			outputs[AUDIO_OUTPUT].setChannels(trackCount);
			for (auto i = 0; i < trackCount; ++i) {
				outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage(i), i);
			}
		}
		audioBufferPosition = sizeAudioBuffer - 1;
		lastAudioBufferLocation = sizeAudioBuffer;
		calcTapeAndPositionsOnWheels(false);
		params[WHEEL_LEFT_PARAM].setValue(positionLeftWheel);
		params[WHEEL_RIGHT_PARAM].setValue(positionRightWheel);
		speedSlewLimiter.reset();
		return;
	}
	int audioBufferLocation = (int) audioBufferPosition;
	float fractAudioBufferLocation = audioBufferPosition - audioBufferLocation;
	
	calcTapeAndPositionsOnWheels(false);
	params[WHEEL_LEFT_PARAM].setValue(positionLeftWheel);
	params[WHEEL_RIGHT_PARAM].setValue(positionRightWheel);
	
	if (tapeSpeed == 0) {
		if (outputs[AUDIO_OUTPUT].isConnected()) {
			outputs[AUDIO_OUTPUT].setChannels(trackCount);
			for (auto i = 0; i < trackCount; ++i) {
				outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage(i), i);
			}
		}
		return;
	}

	if (tapeSpeed > 0) {
		while (lastAudioBufferLocation < audioBufferLocation) {
			++lastAudioBufferLocation;
			if (playStatus) {
				calcAudio(trackCount);
			}
		}
	} else {
		while (lastAudioBufferLocation > audioBufferLocation) {
			--lastAudioBufferLocation;
			if (playStatus) {
				calcAudio(trackCount);
			}
		}
	}
	if (outputs[AUDIO_OUTPUT].isConnected()) {
		outputs[AUDIO_OUTPUT].setChannels(trackCount);
		for (auto i = 0; i < trackCount; ++i) {
			if (playStatus) {
				// TODO add interpolation on recording
				outputs[AUDIO_OUTPUT].setVoltage(crossfade(audioBuffer[audioBufferLocation * trackCount + i], audioBuffer[(audioBufferLocation + 1) * trackCount + i], fractAudioBufferLocation), i);
				// outputs[AUDIO_OUTPUT].setVoltage(audioBuffer[audioBufferLocation * trackCount + i], i);
			} else {
				outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage(i), i);
			}
		}
	}
}

void TapeRecorder::calcAudio(int trackCount) {
	for (auto i = 0; i < trackCount; ++i) {
		float distortionLevel = inputs[AUDIO_INPUT].getVoltage(i + NUM_MAX_TRACKS * 2);
		bool distortionOnInput = true;
		if (distortionLevel < 0) {
			distortionLevel *= -1.f;
			distortionOnInput = false;
		}
		if (distortionLevel > 10) {
			distortionLevel = 10;
		}
		distortionLevel = 10 - distortionLevel;
		if (distortionLevel < 10 && audioBuffer[lastAudioBufferLocation * trackCount + i] > 0) {
			if (distortionLevel != lastDistortionLevelInput[i]) {
				lastDistortionLevelInput[i] = distortionLevel;
				distortionLevel = pow(2, distortionLevel * 0.65f + 0.6f);
				lastDistortionLevelValue[i] = distortionLevel;
			} else {
				distortionLevel = lastDistortionLevelValue[i];
			}
			distortionLevel += (random::uniform() + 0.8f) * 0.2f / (distortionLevel + 1.f);
		} else {
			lastDistortionLevelInput[i] = distortionLevel;
			distortionLevel = 0;
		}
		float replaceLevel = inputs[AUDIO_INPUT].getVoltage(i + NUM_MAX_TRACKS) * -1.f;
		if (replaceLevel < -10) {
			replaceLevel = -10;
		} else if (replaceLevel > 10) {
			replaceLevel = 10;
		}
		if (replaceLevel == 0) {
			if (distortionLevel != 0) {
				if (distortionOnInput) {
					audioBuffer[lastAudioBufferLocation * trackCount + i] = tanh((audioBuffer[lastAudioBufferLocation * trackCount + i] + inputs[AUDIO_INPUT].getVoltage(i)) / distortionLevel) * distortionLevel;
				} else {
					audioBuffer[lastAudioBufferLocation * trackCount + i] = tanh(audioBuffer[lastAudioBufferLocation * trackCount + i] / distortionLevel) * distortionLevel + inputs[AUDIO_INPUT].getVoltage(i);
				}
			} else {
				audioBuffer[lastAudioBufferLocation * trackCount + i] += inputs[AUDIO_INPUT].getVoltage(i);
			}
		} else if (replaceLevel == 10) {
			if (distortionOnInput) {
				audioBuffer[lastAudioBufferLocation * trackCount + i] = tanh(inputs[AUDIO_INPUT].getVoltage(i) / distortionLevel) * distortionLevel;
			} else {
				audioBuffer[lastAudioBufferLocation * trackCount + i] = inputs[AUDIO_INPUT].getVoltage(i);
			}
		} else {
			if (distortionLevel != 0) {
				if (distortionOnInput) {
					audioBuffer[lastAudioBufferLocation * trackCount + i] = tanh((audioBuffer[lastAudioBufferLocation * trackCount + i] + inputs[AUDIO_INPUT].getVoltage(i)) / distortionLevel) * distortionLevel * (10.f - replaceLevel) * 0.1f;
				} else {
					audioBuffer[lastAudioBufferLocation * trackCount + i] = tanh(audioBuffer[lastAudioBufferLocation * trackCount + i] / distortionLevel) * distortionLevel * (10.f - replaceLevel) * 0.1f + inputs[AUDIO_INPUT].getVoltage(i);
				}
			} else {
				audioBuffer[lastAudioBufferLocation * trackCount + i] = audioBuffer[lastAudioBufferLocation * trackCount + i] * (10.f - replaceLevel) * 0.1f + inputs[AUDIO_INPUT].getVoltage(i);
			}
		}
	}
}

// float TapeRecorder::valueAtOffset (const float* const inputs, const float offset) noexcept {
	// // see https://en.wikipedia.org/wiki/Centripetal_Catmull–Rom_spline
	// auto y0 = inputs[3];
	// auto y1 = inputs[2];
	// auto y2 = inputs[1];
	// auto y3 = inputs[0];

	// auto halfY0 = 0.5f * y0;
	// auto halfY3 = 0.5f * y3;

	// return y1 + offset * ((0.5f * y2 - halfY0)
		// + (offset * (((y0 + 2.0f * y2) - (halfY3 + 2.5f * y1))
		// + (offset * ((halfY3 + 1.5f * y1) - (halfY0 + 1.5f * y2))))));
// }

void TapeRecorder::setTrackCount(int trackCount) {
	if (params[TRACK_COUNT_PARAM].getValue() != trackCount) {
		params[TRACK_COUNT_PARAM].setValue(trackCount);
		initTape();
	}
}

void TapeRecorder::setTapeLength(int tapeLength) {
	if (sizeof (*audioBuffer) / sizeof (float) != params[TRACK_COUNT_PARAM].getValue() * TAPE_LENGTHS[tapeLength].value) {
		params[TAPE_LENGTH_PARAM].setValue(tapeLength);
		initTape();
	}
}

// void TapeRecorder::fromJson(json_t* rootJ) {
	// Module::fromJson(rootJ);
	// dataFromJsonCalled = true;
	// DEBUG("dataFromJson");
	// params[PLAY_BACKWARDS_PARAM].setValue(0.);
	// params[CUE_BACKWARDS_PARAM].setValue(0.);
	// params[PLAY_FORWARDS_PARAM].setValue(0.);
	// params[CUE_FORWARDS_PARAM].setValue(0.);
// }

void TapeRecorder::jumpToTapePosition(TapeJump tapeJump) {
	switch (tapeJump) {
		case JUMP_TO_BEGIN:
			audioBufferPosition = 0.;
			break;
		case JUMP_TO_END:
			audioBufferPosition = sizeAudioBuffer;
			break;
		case JUMP_FORWARDS:
			if (loopStartPosition < loopEndPosition) {
				if (audioBufferPosition < loopStartPosition) {
					audioBufferPosition = loopStartPosition;
				} else if (audioBufferPosition < loopEndPosition) {
					audioBufferPosition = loopEndPosition;
				} else {
					audioBufferPosition = sizeAudioBuffer;
				}
			} else {
				if (audioBufferPosition < loopEndPosition) {
					audioBufferPosition = loopEndPosition;
				} else if (audioBufferPosition < loopStartPosition) {
					audioBufferPosition = loopStartPosition;
				} else {
					audioBufferPosition = sizeAudioBuffer;
				}
			}
			break;
		case JUMP_BACKWARDS:
			if (loopStartPosition > loopEndPosition) {
				if (audioBufferPosition > loopStartPosition) {
					audioBufferPosition = loopStartPosition;
				} else if (audioBufferPosition > loopEndPosition) {
					audioBufferPosition = loopEndPosition;
				} else {
					audioBufferPosition = 0.;
				}
			} else {
				if (audioBufferPosition > loopEndPosition) {
					audioBufferPosition = loopEndPosition;
				} else if (audioBufferPosition > loopStartPosition) {
					audioBufferPosition = loopStartPosition;
				} else {
					audioBufferPosition = 0.;
				}
			}
			break;
	}
}

json_t* TapeRecorder::dataToJson() {
	json_t* rootJ = json_object();
	json_object_set_new(rootJ, "tape-name", json_string(tapeName.c_str()));
	json_object_set_new(rootJ, "tape-stripe", json_integer(stripeIndex));
	return rootJ;
}

void TapeRecorder::dataFromJson(json_t* rootJ) {
	json_t* tapeNameJ = json_object_get(rootJ, "tape-name");
	if (tapeNameJ) {
		tapeName = json_string_value(tapeNameJ);
	}
	json_t* tapeStripeJ = json_object_get(rootJ, "tape-stripe");
	if (tapeStripeJ) {
		stripeIndex = json_integer_value(tapeStripeJ);
	}
	initTape();
}

void TapeRecorder::onAdd(const AddEvent& e) {
	// std::string path = system::join(createPatchStorageDirectory(), "wavetable.wav");
	// Read file...
	DEBUG("onAdd");
}

void TapeRecorder::onSave(const SaveEvent& e) {
	// std::string path = system::join(createPatchStorageDirectory(), "wavetable.wav");
	// Write file...
	DEBUG("onSave");
}


