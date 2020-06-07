#include "Ahornberg.hpp"

constexpr int INIT_STEP = 33;
constexpr int INIT_INTERVAL = 9;

struct Interval {
	float value;
	std::string shortName;
	std::string longName;
};

static const Interval INTERVALS[] = {
	{ log2f(9.f / 8.f) * 12, "9/8", "Major Second" },
	{ log2f(8.f / 7.f) * 12, "8/7", "Supermajor Second" },
	{ log2f(7.f / 6.f) * 12, "7/6", "Subminor Third" },
	{ log2f(6.f / 5.f) * 12, "6/5", "Minor Third" },
	{ log2f(5.f / 4.f) * 12, "5/4", "Major Third" },
	{ log2f(4.f / 3.f) * 12, "4/3", "Perfect Fourth" },
	{ log2f(3.f / 2.f) * 12, "3/2", "Perfect Fifth" },
	{ log2f(2.f) * 12,         "2", "Octave" },
	{ log2f(3.f) * 12,         "3", "Tritave (Octave + Perfect Fifth)" },
	{ log2f(4.f) * 12,         "4", "2 Octaves" },
	{ log2f(5.f) * 12,         "5", "2 Octaves + Major Third" },
	{ log2f(6.f) * 12,         "6", "2 Octaves + Perfect Fifth" },
	{ log2f(7.f) * 12,         "7", "2 Octaves + Subminor Seventh" },
	{ log2f(8.f) * 12,         "8", "3 Octaves" }
};

constexpr int NUM_INTERVALS = 13;

struct IntervalNames : ParamQuantity {
	std::string getDisplayValueString() override {
		return INTERVALS[(int) getValue()].longName;
	}
};

struct EqualDivision : Module {
	enum ParamIds {
		FINE_PARAM,
		STEPS_PARAM,
		INTERVAL_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		ENUMS(PITCH_INPUT, 9),
		NUM_INPUTS
	};
	enum OutputIds {
		ENUMS(PITCH_OUTPUT, 9),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	EqualDivision() { 
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(FINE_PARAM, .97f, 1.03f, 1.f, "Fine", "%", 0.f, 100.f, -100.f);
		configParam(STEPS_PARAM, 1, 99, INIT_STEP, "Equal Steps");
		configParam<IntervalNames>(INTERVAL_PARAM, 0, NUM_INTERVALS, INIT_INTERVAL, "Interval");

	}
	
	void process(const ProcessArgs &args) override {
		float multiplier = INTERVALS[(int)params[INTERVAL_PARAM].getValue()].value / params[STEPS_PARAM].getValue() * params[FINE_PARAM].getValue();

		for (int i = 0; i < 9; i++) {
			if (outputs[PITCH_OUTPUT + i].isConnected()) {
				int channels = inputs[PITCH_INPUT + i].getChannels();
				outputs[PITCH_OUTPUT + i].setChannels(channels);
				
				for (int channel = 0; channel < channels; channel += 4) {
					outputs[PITCH_OUTPUT + i].setVoltageSimd(inputs[PITCH_INPUT + i].getPolyVoltageSimd<float_4>(channel) * multiplier, channel);
				}
			}
		}
	}
};
