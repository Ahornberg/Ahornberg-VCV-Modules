// #include <atomic>
// #include <thread>
#include <future>
#include "../../Ahornberg.hpp"
#include "../../include/AudioFile.h"

struct TapeLength {
	float value;
	std::string name;
};

struct TapeRecorder : Module {
	const static std::string INITIAL_TAPE_NAME;
	const static TapeLength TAPE_LENGTHS[];
	constexpr static int NUM_TAPE_LENGTHS = 11;
	constexpr static int NUM_MAX_TRACKS = 4;
	const static std::string AUDIO_FILE_DIR;

	enum ParamIds {
		PAUSE_PARAM,
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
		// TAPE_LENGTH_PARAM,
		// TRACK_COUNT_PARAM,
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
	
	enum InitTape {
		INIT_TAPE_NOOP,
		INIT_TAPE_COMPLETE,
		INIT_TAPE_TRACK_COUNT,
		INIT_TAPE_LENGTH,
		INIT_TAPE_ERASE
	};

	std::string tapeName;
	std::string oldTapeName;
	int stripeIndex;
	int oldStripeIndex;
	// std::atomic<bool> changeTapeInterrupt;
	// std::atomic<bool> initThreadEnded;
	bool changeTapeInterrupt;
	bool initThreadEnded;
	// std::thread initThread;
	std::future<void> initThreadFuture;
	bool tapeStoppedAndResetted;
	AudioFile<float> audioFile;
	// std::mutex mylock;
	std::string audioFilePath;
	std::string oldAudioFilePath;
	std::string warningString;
	int sizeAudioBuffer;
	int trackCountParam;
	int oldTrackCountParam;
	int tapeLengthParam;
	int oldTapeLengthParam;
	// float* audioBuffer;
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
	float prevInputSample[NUM_MAX_TRACKS];

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
	
	InitTape callInitTape;
	
	TapeRecorder();
	~TapeRecorder();
	void initTape(InitTape what);
	void initTapeThread(InitTape what);
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
	void calcAudio(int trackCount, float fractAudioBufferLocation);
	// float valueAtOffset (const float* const inputs, const float offset) noexcept;
	void setTrackCount(int trackCount);
	void setTapeLength(int tapeLength);
	// void fromJson(json_t* rootJ) override;
	void jumpToTapePosition(TapeJump tapeJump);
	json_t* dataToJson() override;
	void dataFromJson(json_t* rootJ) override;
	void onAdd(const AddEvent& e) override;
	void onSave(const SaveEvent& e) override;
	void onRemove(const RemoveEvent& e) override;
	std::string getAudioFileDir();
	bool isTapeEmpty();
};
