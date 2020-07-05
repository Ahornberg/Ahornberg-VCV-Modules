#include "Ahornberg.hpp"

const std::string INIT_TAPE_NAME = "My Magic Tape";

struct TapeLength {
	float value;
	std::string name;
};

static const TapeLength TAPE_LENGTHS[] = {
	{ 44100 * 60 *  1, "MC 1" },
	{ 44100 * 60 *  3, "MC 3" },
	{ 44100 * 60 *  6, "MC 6" },
	{ 44100 * 60 * 10, "MC 10" },
	{ 44100 * 60 * 15, "MC 15" },
	{ 44100 * 60 * 20, "MC 20" },
	{ 44100 * 60 * 30, "MC 30" },
	{ 44100 * 60 * 45, "MC 45" },
	{ 44100 * 60 * 60, "MC 60" }
};

constexpr int NUM_TAPE_LENGTHS = 9;

struct TapeRecorder : ModuleWithScrews {
	enum ParamIds {
		PAUSE_PARAM = NUM_MAX_SCREWS,
		PLAY_FORWARDS_PARAM,
		PLAY_BACKWARDS_PARAM,
		CUE_FORWARDS_PARAM,
		CUE_BACKWARDS_PARAM,
		CUE_SPEED_PARAM,
		CUE_SLEW_PARAM,
		TEMPO_PARAM,
		BEATS_PER_BAR_PARAM,
		LOOP_START_PARAM,
		LOOP_END_PARAM,
		LOOP_START_BUTTON_PARAM,
		LOOP_END_BUTTON_PARAM,
		WHEEL_LEFT_PARAM,
		WHEEL_RIGHT_PARAM,
		TAPE_LENGTH_PARAM,
		TRACK_COUNT_PARAM,
		OLD_SCHOOL_MODE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_INPUT,
		LOOP_INPUT,
		SPEED_INPUT,
		TRANSPORT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AUDIO_OUTPUT,
		LOOP_OUTPUT,
		SPEED_OUTPUT,
		TEMPO_OUTPUT,
		TRANSPORT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	int sizeAudioBuffer;
	float* audioBuffer;
	double audioBufferPosition;
	double loopStartPosition;
	double loopEndPosition;
	double tapePosition;
	double tapeOnLeftWheel;
	double tapeOnRightWheel;
	int lastAudioBufferLocation;

	dsp::BooleanTrigger playForwardsTrigger;
	dsp::BooleanTrigger playBackwardsTrigger;
	dsp::BooleanTrigger cueForwardsTrigger;
	dsp::BooleanTrigger cueBackwardsTrigger;
	bool playStatus;
	bool cueStatus;
	bool playForwardStatus;
	bool cueForwardStatus;
	int loopStart;
	int loopEnd;
	int beat;
	int bar;
	int oldLoopStartParam;
	int oldLoopEndParam;
	
	dsp::PulseGenerator barsPulse;
	dsp::PulseGenerator beatsPulse;
	dsp::ExponentialSlewLimiter speedSlewLimiter;
	dsp::SchmittTrigger loopInputTrigger;
	dsp::SchmittTrigger pauseInputTrigger;
	dsp::SchmittTrigger playForwardsInputTrigger;
	dsp::SchmittTrigger playBackwardsInputTrigger;
	dsp::SchmittTrigger cueForwardsInputTrigger;
	dsp::SchmittTrigger cueBackwardsInputTrigger;
	dsp::SchmittTrigger cueForwardsMomentaryInputTrigger;
	dsp::SchmittTrigger cueBackwardsMomentaryInputTrigger;
	
	float speed;
	float touchedWheelForce;
	
	TapeRecorder() { 
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
		configParam(LOOP_START_BUTTON_PARAM,   0,     1,   0, "Loop Start on Tape Position");
		configParam(LOOP_END_BUTTON_PARAM,     0,     1,   0, "Loop End on Tape Position");
		configParam(WHEEL_LEFT_PARAM,        -12.4,  12,   0, "Left Wheel");
		configParam(WHEEL_RIGHT_PARAM,       -12,    12.1, 0, "Right Wheel");
		configParam(TAPE_LENGTH_PARAM,         0, NUM_TAPE_LENGTHS - 1, 0, "Tape Length");
		configParam(TRACK_COUNT_PARAM,         1,    16,   1, "Tape Layout");
		configParam(OLD_SCHOOL_MODE_PARAM,     0,     1,   0, "Old School Mode");
		sizeAudioBuffer = TAPE_LENGTHS[(int) params[TAPE_LENGTH_PARAM].getValue()].value;
		audioBuffer = new float[sizeAudioBuffer];
		for (auto i = 0; i < sizeAudioBuffer; ++i) {
			audioBuffer[i] = 0.f;
		}
		playStatus = false;
		cueStatus = false;
		playForwardStatus = false;
		cueForwardStatus = false;
		audioBufferPosition = 0.;
		loopStartPosition = 0;
		loopEndPosition = 0;
		tapePosition = 0;
		tapeOnLeftWheel = 0;
		tapeOnRightWheel = 1;
		lastAudioBufferLocation = -1;
		beat = 0;
		bar = 0;
		barsPulse.reset();
		beatsPulse.reset();
		speed = 0;
		touchedWheelForce = 1;
		speedSlewLimiter.setRiseFall(params[CUE_SLEW_PARAM].getValue(), 20);
		pauseInputTrigger.reset();
		playForwardsInputTrigger.reset();
		playBackwardsInputTrigger.reset();
		cueForwardsInputTrigger.reset();
		cueBackwardsInputTrigger.reset();
		cueForwardsMomentaryInputTrigger.reset();
		cueBackwardsMomentaryInputTrigger.reset();
	}
	
	~TapeRecorder() {
		delete audioBuffer;
	}
	
	void processTempoOutput(const ProcessArgs &args) {
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
	
	void processSpeedInput(const ProcessArgs &args) {
		if (inputs[SPEED_INPUT].isConnected()) {
			speed *= exp2(inputs[SPEED_INPUT].getVoltage());
		}
		speed *= touchedWheelForce;
		speedSlewLimiter.setRiseFall(params[CUE_SLEW_PARAM].getValue(), 20);
		speed = speedSlewLimiter.process(args.sampleTime, speed);
	}
	
	void processSpeedOutput() {
		if (speed > 0) {
			outputs[SPEED_OUTPUT].setVoltage(log2(speed));
		} else if (speed < 0) {
			outputs[SPEED_OUTPUT].setVoltage(log2(speed * -1.f));
		} else {
			outputs[SPEED_OUTPUT].setVoltage(0);
		}
	}
	
	void processLoopInput() {
		if (params[LOOP_START_PARAM].getValue() != oldLoopStartParam) {
			params[LOOP_START_BUTTON_PARAM].setValue(0.f);
			oldLoopStartParam = params[LOOP_START_PARAM].getValue();
		}
		if (inputs[LOOP_INPUT].isConnected()) {
			loopStart = (int) ((inputs[LOOP_INPUT].getVoltage(0) * 12.f) + 0.5f + params[LOOP_START_PARAM].getValue());
			if (loopStart < 0) {
				loopStart = 0;
			}
		} else {
			loopStart = params[LOOP_START_PARAM].getValue();
		}
		
		if (params[LOOP_END_PARAM].getValue() != oldLoopEndParam) {
			params[LOOP_END_BUTTON_PARAM].setValue(0.f);
			oldLoopEndParam = params[LOOP_END_PARAM].getValue();
		}
		if (inputs[LOOP_INPUT].isConnected() && inputs[LOOP_INPUT].getChannels() > 1) {
			loopEnd = (int) ((inputs[LOOP_INPUT].getVoltage(1) * 12.f) + 0.5f + params[LOOP_END_PARAM].getValue());
			if (loopEnd < 0) {
				loopEnd = 0;
			}
		} else {
			loopEnd = params[LOOP_END_PARAM].getValue();
		}
	}
	
	void processLoopOutput(const ProcessArgs &args) {
		
	}
	
	void toggleParamValue(int param) {
		params[param].getValue() ? params[param].setValue(0) : params[param].setValue(1);
	}
	
	bool isTransportCueSwitchMomentary() {
		return params[PLAY_FORWARDS_PARAM].getValue() || params[PLAY_BACKWARDS_PARAM].getValue();
	}
	
	float rescaleInput(int port, int channel = 0) {
		return rescale(inputs[port].getVoltage(channel), 0.1, 2, 0, 1);
	}
	
	float rescaleInverseInput(int port, int channel = 0) {
		return rescale(inputs[port].getVoltage(channel), 2, 0.1, 0, 1);
	}
	
	void processTransportInput() {
		// TODO loop input
		if (inputs[TRANSPORT_INPUT].isConnected()) {
			if (pauseInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 2))) {
				// pause is independend from all other transport keys
				toggleParamValue(PAUSE_PARAM);
			}
			if (playBackwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 1))) {
				toggleParamValue(PLAY_BACKWARDS_PARAM);
				if (params[PLAY_BACKWARDS_PARAM].getValue()) {
					// only one play direction active at a time
					params[PLAY_FORWARDS_PARAM].setValue(0);
				}
			}
			if (playForwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 3))) {
				toggleParamValue(PLAY_FORWARDS_PARAM);
				if (params[PLAY_FORWARDS_PARAM].getValue()) {
					// only one play direction active at a time
					params[PLAY_BACKWARDS_PARAM].setValue(0);
				}
			}
			if (cueBackwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 0))) {
				toggleParamValue(CUE_BACKWARDS_PARAM);
				if (params[CUE_BACKWARDS_PARAM].getValue()) {
					// only one cue direction active at a time
					params[CUE_FORWARDS_PARAM].setValue(0);
				}
			}
			if (cueBackwardsMomentaryInputTrigger.process(rescaleInverseInput(TRANSPORT_INPUT, 0))) {
				if (isTransportCueSwitchMomentary()) {
					params[CUE_BACKWARDS_PARAM].setValue(0);
				}
			}

			if (cueForwardsInputTrigger.process(rescaleInput(TRANSPORT_INPUT, 4))) {
				toggleParamValue(CUE_FORWARDS_PARAM);
				if (params[CUE_FORWARDS_PARAM].getValue()) {
					// only one cue direction active at a time
					params[CUE_BACKWARDS_PARAM].setValue(0);
				}
			}
			if (cueForwardsMomentaryInputTrigger.process(rescaleInverseInput(TRANSPORT_INPUT, 4))) {
				if (isTransportCueSwitchMomentary()) {
					params[CUE_FORWARDS_PARAM].setValue(0);
				}
			}

		}
	}
	
	void processTransportOutput(const ProcessArgs &args) {
		
	}
	
	void processAudioInput(const ProcessArgs &args) {
		
	}
	
	void processAudioOutput(const ProcessArgs &args) {
		
	}
	
	// process ****************************************************************
	
	void process(const ProcessArgs &args) override {
		// tape begin/end reached
		
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

		playStatus = !params[PAUSE_PARAM].getValue() && (params[PLAY_FORWARDS_PARAM].getValue() || params[PLAY_BACKWARDS_PARAM].getValue());
		playForwardStatus = !params[PLAY_BACKWARDS_PARAM].getValue();
		cueStatus = params[CUE_FORWARDS_PARAM].getValue() || params[CUE_BACKWARDS_PARAM].getValue();
		cueForwardStatus = !params[CUE_BACKWARDS_PARAM].getValue();

		processLoopInput();
		processTempoOutput(args);

		speed = 1.f;
		
		processSpeedInput(args);
		
		if (!playStatus && !cueStatus) {
			// TODO slew
			speed = 0.f;
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
		
		audioBufferPosition += speed;
		
		processSpeedOutput();

	    if (audioBufferPosition >= 0) {
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
			if (params[LOOP_START_BUTTON_PARAM].getValue() == 1.0f) {
				loopStartPosition = audioBufferPosition;
			} else {
				loopStartPosition = (args.sampleRate * loopStart * 60 * params[BEATS_PER_BAR_PARAM].getValue()) / params[TEMPO_PARAM].getValue();
			}
			if (params[LOOP_END_BUTTON_PARAM].getValue() == 1.0f) {
				loopEndPosition = audioBufferPosition;
			} else {
				loopEndPosition = (args.sampleRate * loopEnd * 60 * params[BEATS_PER_BAR_PARAM].getValue()) / params[TEMPO_PARAM].getValue();
			}
		}
		if (speed == 0) {
			if (outputs[AUDIO_OUTPUT].isConnected()) {
				outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage());
			}
			return;
		}
		
		if (audioBufferPosition < 0) {
			if (outputs[AUDIO_OUTPUT].isConnected()) {
				outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage());
			}
			audioBufferPosition = 0.;
			lastAudioBufferLocation = -1;
			if (cueStatus && !cueForwardStatus) {
				cueStatus = false;
			}
			if (playStatus && !playForwardStatus) {
				playStatus = false;
			}
			tapeOnLeftWheel = 0;
			tapeOnRightWheel = 1;
			params[WHEEL_LEFT_PARAM].setValue(0);
			params[WHEEL_RIGHT_PARAM].setValue(0);
			speedSlewLimiter.reset();
			return;
		}
		if (audioBufferPosition >= sizeAudioBuffer) {
			if (outputs[AUDIO_OUTPUT].isConnected()) {
				outputs[AUDIO_OUTPUT].setVoltage(inputs[AUDIO_INPUT].getVoltage());
			}
			audioBufferPosition = sizeAudioBuffer - 1;
			lastAudioBufferLocation = sizeAudioBuffer;
			if (cueStatus && cueForwardStatus) {
				cueStatus = false;
			}
			if (playStatus && playForwardStatus) {
				playStatus = false;
			}
			tapeOnLeftWheel = 1;
			tapeOnRightWheel = 0;
			params[WHEEL_LEFT_PARAM].setValue(0);
			params[WHEEL_RIGHT_PARAM].setValue(0);
			speedSlewLimiter.reset();
			return;
		}
		int audioBufferLocation = (int) audioBufferPosition;
		
		tapeOnLeftWheel = (1.0 - ((pow((1.0 - (audioBufferPosition / sizeAudioBuffer)) * 21.0 + 14.0, 2.0) - 196.0) / 1029.0));
		tapeOnRightWheel = (1.0 - ((pow((audioBufferPosition / sizeAudioBuffer) * 21.0 + 14.0, 2) - 196.0) / 1029.0));
		float positionLeftWheel = tapeOnLeftWheel * 120;
		while (positionLeftWheel > 6) {
			positionLeftWheel -= 12;
		}
		while (positionLeftWheel < -6) {
			positionLeftWheel += 12;
		}
		float positionRightWheel = tapeOnRightWheel * -120;
		while (positionRightWheel > 6) {
			positionRightWheel -= 12;
		}
		while (positionRightWheel < -6) {
			positionRightWheel += 12;
		}
		params[WHEEL_LEFT_PARAM].setValue(positionLeftWheel);
		params[WHEEL_RIGHT_PARAM].setValue(positionRightWheel);
		
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
	
	float valueAtOffset (const float* const inputs, const float offset) noexcept {
		// see https://en.wikipedia.org/wiki/Centripetal_Catmull–Rom_spline
		auto y0 = inputs[3];
		auto y1 = inputs[2];
		auto y2 = inputs[1];
		auto y3 = inputs[0];

		auto halfY0 = 0.5f * y0;
		auto halfY3 = 0.5f * y3;

		return y1 + offset * ((0.5f * y2 - halfY0)
			+ (offset * (((y0 + 2.0f * y2) - (halfY3 + 2.5f * y1))
			+ (offset * ((halfY3 + 1.5f * y1) - (halfY0 + 1.5f * y2))))));
	}
	
	void setTrackCount(int trackCount) {
		if (params[TRACK_COUNT_PARAM].getValue() != trackCount) {
			params[TRACK_COUNT_PARAM].setValue(trackCount);
			// TODO stop tape if needed
			// panic();
		}
	}
	
	void setTapeLength(int tapeLength) {
		if ((sizeof (*audioBuffer) / sizeof (float)) != TAPE_LENGTHS[tapeLength].value) {
			params[TAPE_LENGTH_PARAM].setValue(tapeLength);
			// audioBuffer = trackCount;
			// TODO stop tape if needed
			// panic();
		}
	}
	
	void setOldSchoolMode(bool oldSchoolMode) {
		oldSchoolMode ? params[OLD_SCHOOL_MODE_PARAM].setValue(1) : params[OLD_SCHOOL_MODE_PARAM].setValue(0);
	}
};
