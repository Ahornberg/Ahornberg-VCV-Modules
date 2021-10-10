#include "../../Ahornberg.hpp"

struct FunWithFrames : BaseModule {
	constexpr static int NUM_VALUE_IO = 4;
	const static std::string TIME_TABLE_NAMES[];
	float lastPitchChord[NUM_VALUE_IO];
	bool pitchGate[NUM_VALUE_IO];
	
	enum ParamIds {
		MAX_CHORD_SIZE_PARAM = NUM_MAX_SCREWS,
		ENUMS(VALUE_CONVERT_MODE, NUM_VALUE_IO),
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(VALUE_INPUT, NUM_VALUE_IO),
		NOTE_INPUT,
		GATE_INPUT,
		CLOCK_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(VALUE_OUTPUT, NUM_VALUE_IO),
		NOTE_OUTPUT,
		GATE_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};
	
	int arpeggioPosition;
	dsp::SchmittTrigger clockInputTrigger;

	FunWithFrames();
	void process(const ProcessArgs& args) override;
};
