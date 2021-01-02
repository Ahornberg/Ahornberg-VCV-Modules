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
	configParam(PAUSE_PARAM,               0,     1,   0, "Pause");
	configParam(PLAY_FORWARDS_PARAM,       0,     1,   0, "Play");
	configParam(PLAY_BACKWARDS_PARAM,      0,     1,   0, "Play Reverse");
	configParam(CUE_FORWARDS_PARAM,        0,     1,   0, "Fast Forward");
	configParam(CUE_BACKWARDS_PARAM,       0,     1,   0, "Rewind");
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
	changeTapeInterrupt = false;
	tapeStoppedAndResetted = true;
	initTape();
}

TapeRecorder::~TapeRecorder() {
	delete audioBuffer;
}

void TapeRecorder::initTape() {
	changeTapeInterrupt = true;
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
	calcTapeAndPositionsOnWheels();
	params[WHEEL_LEFT_PARAM].setValue(positionLeftWheel);
	params[WHEEL_RIGHT_PARAM].setValue(positionRightWheel);
	lastAudioBufferLocation = -1;
	beat = 0;
	bar = 0;
	barsPulse.reset();
	beatsPulse.reset();
	speed = 0;
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
	dataFromJsonCalled = false;
	
	changeTapeInterrupt = false;
}

void TapeRecorder::eraseTape() {
	for (auto i = 0; i < sizeAudioBuffer; ++i) {
		audioBuffer[i] = 0.f;
	}
}

void TapeRecorder::calcTapeAndPositionsOnWheels() {
	tapeOnLeftWheel = (1.0 - ((pow((1.0 - (audioBufferPosition / sizeAudioBuffer)) * 21.0 + 14.0, 2.0) - 196.0) / 1029.0));
	tapeOnRightWheel = (1.0 - ((pow((audioBufferPosition / sizeAudioBuffer) * 21.0 + 14.0, 2.0) - 196.0) / 1029.0));
	positionLeftWheel = centerWheel(tapeOnLeftWheel * 120 * tapeLengthInMinutes);
	positionRightWheel = centerWheel(tapeOnRightWheel * -120 * tapeLengthInMinutes);
	// TODO test somd::fmod
	// positionLeftWheel = fmod(tapeOnLeftWheel * 120 * tapeLengthInMinutes, 12);
	// positionRightWheel = fmod(tapeOnRightWheel * -120 * tapeLengthInMinutes, 12);
}

float TapeRecorder::centerWheel(float positionOnWheel) {
	while (positionOnWheel > 6000) {
		positionOnWheel -= 12000;
	}
	while (positionOnWheel > 600) {
		positionOnWheel -= 1200;
	}
	while (positionOnWheel > 60) {
		positionOnWheel -= 120;
	}
	while (positionOnWheel > 6) {
		positionOnWheel -= 12;
	}
	while (positionOnWheel < -6000) {
		positionOnWheel += 12000;
	}
	while (positionOnWheel < -600) {
		positionOnWheel += 1200;
	}
	while (positionOnWheel < -60) {
		positionOnWheel += 120;
	}
	while (positionOnWheel < -6) {
		positionOnWheel += 12;
	}
	return positionOnWheel;
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
		speed *= exp2(inputs[SPEED_INPUT].getVoltage());
	}
	speed *= touchedWheelForce;
	speedSlewLimiter.setRiseFall(params[CUE_SLEW_PARAM].getValue(), 20);
	speed = speedSlewLimiter.process(args.sampleTime, speed);
}

void TapeRecorder::processSpeedOutput() {
	if (speed > 0) {
		outputs[SPEED_OUTPUT].setVoltage(log2(speed));
	} else if (speed < 0) {
		outputs[SPEED_OUTPUT].setVoltage(log2(speed * -1.f));
	} else {
		outputs[SPEED_OUTPUT].setVoltage(0);
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

void TapeRecorder::toggleParamValue(int param) {
	params[param].getValue() ? params[param].setValue(0) : params[param].setValue(1);
}

bool TapeRecorder::isTransportCueSwitchMomentary() {
	return params[PLAY_FORWARDS_PARAM].getValue() || params[PLAY_BACKWARDS_PARAM].getValue();
}

float TapeRecorder::rescaleInput(int port, int channel) {
	return rescale(inputs[port].getVoltage(channel), 0.1, 2, 0, 1);
}

float TapeRecorder::rescaleInverseInput(int port, int channel) {
	return rescale(inputs[port].getVoltage(channel), 2, 0.1, 0, 1);
}

void TapeRecorder::processTransportInput() {
	// TODO loop input
	if (inputs[TRANSPORT_INPUT].isConnected()) {
		if (pauseInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 0))) {
			// pause is independend from all other transport keys
			toggleParamValue(PAUSE_PARAM);
		}
		// if (playBackwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 1))) {
			// toggleParamValue(PLAY_BACKWARDS_PARAM);
			// if (params[PLAY_BACKWARDS_PARAM].getValue()) {
				// // only one play direction active at a time
				// params[PLAY_FORWARDS_PARAM].setValue(0);
			// }
		// }
		// if (playForwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 3))) {
			// toggleParamValue(PLAY_FORWARDS_PARAM);
			// if (params[PLAY_FORWARDS_PARAM].getValue()) {
				// // only one play direction active at a time
				// params[PLAY_BACKWARDS_PARAM].setValue(0);
			// }
		// }
		// if (cueBackwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 0))) {
			// toggleParamValue(CUE_BACKWARDS_PARAM);
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
			// toggleParamValue(CUE_FORWARDS_PARAM);
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

	speed = 1.f;
	
	processSpeedInput(args);
	
	if ((!playStatus && !cueStatus) || (playStatus && params[PAUSE_PARAM].getValue() && !cueStatus)) {
		// TODO slew
		// speed = 0.f;
		speed = wheelMovementSlewLimiter.process(args.sampleTime, wheelMovement);
		// speed = speedSlewLimiter.process(args.sampleTime, speed);
	} else if (cueStatus) {
		speed *= params[CUE_SPEED_PARAM].getValue();
		// speed = speedSlewLimiter.process(args.sampleTime, speed);
		if (!cueForwardStatus) {
			// TODO slew
			speed *= -1.f;
		}
	} else if (!playForwardStatus) {
		// TODO slew
		speed *= -1.f;
	}
	
	// if (speed == 0) {
		// // if (positionLeftWheel != params[WHEEL_LEFT_PARAM].getValue()) {
			// speed = wheelMovement;
		// // }
	// }
	
	audioBufferPosition += speed;
	
	processSpeedOutput();

	if (audioBufferPosition > 0 && audioBufferPosition < sizeAudioBuffer) {
		tapeStatus = TAPE_MIDDLE;
		tapePosition = (args.sampleTime * audioBufferPosition * params[TEMPO_PARAM].getValue()) / (60 * params[BEATS_PER_BAR_PARAM].getValue());
		int newBar = (int) tapePosition;
		int newBeat = (int) ((tapePosition - bar) * params[BEATS_PER_BAR_PARAM].getValue());
		if (newBar != bar) {
			barsPulse.trigger();
		}
		bar = newBar;
		if (newBeat != beat) {
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
	if (loopStart < loopEnd) {
		if (audioBufferPosition <= loopStartPosition && playStatus && !playForwardStatus) {
			toggleParamValue(PLAY_FORWARDS_PARAM);
			toggleParamValue(PLAY_BACKWARDS_PARAM);
		}
		if (audioBufferPosition > loopEndPosition && playStatus && playForwardStatus) {
			toggleParamValue(PLAY_FORWARDS_PARAM);
			toggleParamValue(PLAY_BACKWARDS_PARAM);
		}
	} else if (loopStart > loopEnd) {
		if (audioBufferPosition <= loopEndPosition && playStatus && !playForwardStatus) {
			toggleParamValue(PLAY_FORWARDS_PARAM);
			toggleParamValue(PLAY_BACKWARDS_PARAM);
		}
		if (audioBufferPosition > loopStartPosition && playStatus && playForwardStatus) {
			toggleParamValue(PLAY_FORWARDS_PARAM);
			toggleParamValue(PLAY_BACKWARDS_PARAM);
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
			outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage());
		}
		audioBufferPosition = 0.;
		lastAudioBufferLocation = -1;
		calcTapeAndPositionsOnWheels();
		params[WHEEL_LEFT_PARAM].setValue(positionLeftWheel);
		params[WHEEL_RIGHT_PARAM].setValue(positionRightWheel);
		speedSlewLimiter.reset();
		return;
	}
	if (audioBufferPosition >= sizeAudioBuffer) {
		tapeStatus = TAPE_END;
		if (outputs[AUDIO_OUTPUT].isConnected()) {
			outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage());
		}
		audioBufferPosition = sizeAudioBuffer - 1;
		lastAudioBufferLocation = sizeAudioBuffer;
		calcTapeAndPositionsOnWheels();
		params[WHEEL_LEFT_PARAM].setValue(positionLeftWheel);
		params[WHEEL_RIGHT_PARAM].setValue(positionRightWheel);
		speedSlewLimiter.reset();
		return;
	}
	int audioBufferLocation = (int) audioBufferPosition;
	
	calcTapeAndPositionsOnWheels();
	params[WHEEL_LEFT_PARAM].setValue(positionLeftWheel);
	params[WHEEL_RIGHT_PARAM].setValue(positionRightWheel);
	
	if (speed == 0) {
		if (outputs[AUDIO_OUTPUT].isConnected()) {
			outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage());
		}
		return;
	}

	if (speed > 0) {
		while (lastAudioBufferLocation < audioBufferLocation) {
			++lastAudioBufferLocation;
			if (playStatus) {
				audioBuffer[lastAudioBufferLocation] += inputs[AUDIO_INPUT].getVoltage();
			}
		}
	} else {
		while (lastAudioBufferLocation > audioBufferLocation) {
			--lastAudioBufferLocation;
			if (playStatus) {
				audioBuffer[lastAudioBufferLocation] += inputs[AUDIO_INPUT].getVoltage();
			}
		}
	}
	if (outputs[AUDIO_OUTPUT].isConnected()) {
		if (playStatus) {
			outputs[AUDIO_OUTPUT].setVoltage(audioBuffer[audioBufferLocation]);
		} else {
			outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage());
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

void TapeRecorder::dataFromJson(json_t* root) {
	Module::dataFromJson(root);
	dataFromJsonCalled = true;
	// params[PLAY_BACKWARDS_PARAM].setValue(0.);
	// params[CUE_BACKWARDS_PARAM].setValue(0.);
	// params[PLAY_FORWARDS_PARAM].setValue(0.);
	// params[CUE_FORWARDS_PARAM].setValue(0.);
}
