#include "../../Ahornberg.hpp"

struct TapeLength {
	float value;
	std::string name;
};

struct TapeRecorder : BaseModule {
	const static std::string INIT_TAPE_NAME;
	const static TapeLength TAPE_LENGTHS[];
	constexpr static int NUM_TAPE_LENGTHS = 11;
	constexpr static int NUM_MAX_TRACKS = 4;

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
		// LOOP_START_BUTTON_PARAM,
		// LOOP_END_BUTTON_PARAM,
		LOOP_MODE_PARAM,
		WHEEL_LEFT_PARAM,
		WHEEL_RIGHT_PARAM,
		TAPE_LENGTH_PARAM,
		TRACK_COUNT_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		AUDIO_INPUT,
		LOOP_START_INPUT,
		LOOP_END_INPUT,
		SPEED_INPUT,
		TRANSPORT_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		AUDIO_OUTPUT,
		// LOOP_OUTPUT,
		SPEED_OUTPUT,
		TEMPO_OUTPUT,
		TRANSPORT_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	enum TapeStatus {
		TAPE_BEGIN,
		TAPE_MIDDLE,
		TAPE_END
	};
	
	enum TapeJump {
		JUMP_TO_BEGIN,
		JUMP_TO_END,
		JUMP_FORWARDS,
		JUMP_BACKWARDS
	};

	std::string tapeName;
	int stripeIndex;
	bool changeTapeInterrupt;
	bool tapeStoppedAndResetted;
	int sizeAudioBuffer;
	float* audioBuffer;
	double audioBufferPosition;
	double loopStartPosition;
	double loopEndPosition;
	double tapePosition;
	double tapeOnLeftWheel;
	double tapeOnRightWheel;
	float positionLeftWheel;
	float positionRightWheel;
	int lastAudioBufferLocation;
	TapeStatus tapeStatus;
	float lastDistortionLevelInput[NUM_MAX_TRACKS];
	float lastDistortionLevelValue[NUM_MAX_TRACKS];

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
	dsp::ExponentialSlewLimiter wheelMovementSlewLimiter;
	dsp::SchmittTrigger loopInputTrigger;
	dsp::SchmittTrigger pauseInputTrigger;
	dsp::SchmittTrigger playForwardsInputTrigger;
	dsp::SchmittTrigger playBackwardsInputTrigger;
	dsp::SchmittTrigger cueForwardsInputTrigger;
	dsp::SchmittTrigger cueBackwardsInputTrigger;
	dsp::SchmittTrigger cueForwardsMomentaryInputTrigger;
	dsp::SchmittTrigger cueBackwardsMomentaryInputTrigger;
	dsp::ClockDivider displayDivider;
	
	float tapeSpeed;
	float touchedWheelForce;
	float wheelMovement;
	float tapeLengthInMinutes;
	
	bool dataFromJsonCalled;
	
	TapeRecorder();
	~TapeRecorder();
	void initTape();
	void eraseTape();
	void calcTapeAndPositionsOnWheels(bool always);
	// float centerWheel(float positionOnWheel);
	void processTempoOutput(const ProcessArgs& args);
	void processSpeedInput(const ProcessArgs& args);
	void processSpeedOutput();
	void processLoopInput();
	// void processLoopOutput(const ProcessArgs& args);
	// void toggleParamValue(int param);
	bool isTransportCueSwitchMomentary();
	// float rescaleInput(int port, int channel = 0);
	// float rescaleInverseInput(int port, int channel = 0);
	void processTransportInput();
	void processTransportOutput();
	void processAudioInput(const ProcessArgs& args);
	void processAudioOutput(const ProcessArgs& args);
	void process(const ProcessArgs& args) override;
	void calcAudio(int trackCount);
	// float valueAtOffset (const float* const inputs, const float offset) noexcept;
	void setTrackCount(int trackCount);
	void setTapeLength(int tapeLength);
	// void fromJson(json_t* rootJ) override;
	void jumpToTapePosition(TapeJump tapeJump);
	json_t* dataToJson() override;
	void dataFromJson(json_t* rootJ) override;
};
