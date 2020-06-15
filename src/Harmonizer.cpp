#include "Ahornberg.hpp"

constexpr int NUM_CHANNELS = 16;
constexpr int NUM_MIXTURES = 4;

struct Harmonizer : Module {
	enum ParamIds {
		ENUMS(CHANNEL_ACTIVE_PARAM, NUM_CHANNELS),
		VELOCITY_GLOBAL_MODULATION_PARAM,
		ENUMS(VELOCITY_MODULATION_PARAM, NUM_CHANNELS),
		ENUMS(VELOCITY_PARAM, NUM_CHANNELS),
		PITCH_GLOBAL_MODULATION_PARAM,
		ENUMS(PITCH_MODULATION_PARAM, NUM_CHANNELS),
		ENUMS(PITCH_HARMONICS_PARAM, NUM_CHANNELS),
		ENUMS(PITCH_SUBHARMONICS_PARAM, NUM_CHANNELS),
		ENUMS(PITCH_FINE_PARAM, NUM_CHANNELS),
		ENUMS(MIXTUR_PARAM, NUM_MIXTURES),
		NUM_PARAMS
	};
	enum InputIds {
		GATE_INPUT,
		RETRIGGER_INPUT,
		VELOCITY_INPUT,
		VELOCITY_GLOBAL_MODULATION_INPUT,
		ENUMS(VELOCITY_MODULATION_INPUT, NUM_CHANNELS),
		PITCH_INPUT,
		PITCH_GLOBAL_MODULATION_INPUT,
		ENUMS(PITCH_MODULATION_INPUT, NUM_CHANNELS),
		MIXTUR_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		GATE_OUTPUT,
		RETRIGGER_OUTPUT,
		VELOCITY_GLOBAL_OUTPUT,
		ENUMS(VELOCITY_OUTPUT, NUM_CHANNELS),
		PITCH_GLOBAL_OUTPUT,
		ENUMS(PITCH_OUTPUT, NUM_CHANNELS),
		MIXTUR_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	Harmonizer() { 
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configParam(VELOCITY_GLOBAL_MODULATION_PARAM, -1, 1, 0, "Velocity Modulation");
		configParam(PITCH_GLOBAL_MODULATION_PARAM,    -1, 1, 0, "Pitch Modulation");
		for (auto i = 0; i < NUM_CHANNELS; ++i) {
			configParam(CHANNEL_ACTIVE_PARAM + i,       0,     1,    0, string::f("Channel %d active", i + 1));
			configParam(VELOCITY_MODULATION_PARAM + i, -1,     1,    0, string::f("Velocity Modulation Channel %d", i + 1));
			configParam(VELOCITY_PARAM + i,             0,     1,    1, string::f("Velocity Channel %d", i + 1));
			configParam(PITCH_MODULATION_PARAM + i,    -1,     1,    0, string::f("Pitch Modulation Channel %d", i + 1));
			configParam(PITCH_HARMONICS_PARAM + i,      1,    96,    1, string::f("Pitch Harmonics Channel %d", i + 1));
			configParam(PITCH_SUBHARMONICS_PARAM + i,   1,    96,    1, string::f("Pitch Subarmonics Channel %d", i + 1));
			configParam(PITCH_FINE_PARAM + i,           0.97,  1.03, 1, string::f("Pitch Fine Channel %d", i + 1), "%", 0, 100, -100);
			if (i < 4) {
				configParam(MIXTUR_PARAM + i,           0,     1,    0, string::f("Mixtur %d active", i + 1));
			}
		}
		params[CHANNEL_ACTIVE_PARAM].setValue(1);
	}
	
	
	float pitch2freq(float pitch) {
		return exp2(pitch);
	}

	float freq2pitch(float freq) {
		return log2(freq);
	}

	void process(const ProcessArgs &args) override {
		float gateIn = inputs[GATE_INPUT].getVoltage();
		float retriggerIn = inputs[RETRIGGER_INPUT].getVoltage();
		float frequencyIn = pitch2freq(inputs[PITCH_INPUT].getVoltage());
		if(inputs[PITCH_GLOBAL_MODULATION_INPUT].isConnected()) {
			frequencyIn += inputs[PITCH_GLOBAL_MODULATION_INPUT].getVoltage() * pow(params[PITCH_GLOBAL_MODULATION_PARAM].getValue(), 7);
		}
		float velocityIn = inputs[VELOCITY_INPUT].getVoltage();
		if(inputs[VELOCITY_GLOBAL_MODULATION_INPUT].isConnected()) {
			velocityIn += inputs[VELOCITY_GLOBAL_MODULATION_INPUT].getVoltage() * pow(params[VELOCITY_GLOBAL_MODULATION_PARAM].getValue(), 7);
		}
		int numActiveChannels = 0;
		for (auto i = 0; i < NUM_CHANNELS; ++i) {
			if (params[CHANNEL_ACTIVE_PARAM + i].getValue()) {
				outputs[GATE_OUTPUT].setVoltage(gateIn, numActiveChannels);
				outputs[RETRIGGER_OUTPUT].setVoltage(retriggerIn, numActiveChannels);
				
				float frequencyMiltiplier = params[PITCH_HARMONICS_PARAM + i].getValue() / params[PITCH_SUBHARMONICS_PARAM + i].getValue() * params[PITCH_FINE_PARAM + i].getValue();
				outputs[PITCH_GLOBAL_OUTPUT].setVoltage(log2(frequencyIn * frequencyMiltiplier), numActiveChannels);
				
				float velocityMultiplier = params[VELOCITY_PARAM + i].getValue();
				outputs[VELOCITY_GLOBAL_OUTPUT].setVoltage(velocityIn * velocityMultiplier, numActiveChannels);
				
				++numActiveChannels;
			}
		}
		outputs[GATE_OUTPUT].setChannels(numActiveChannels);
		outputs[RETRIGGER_OUTPUT].setChannels(numActiveChannels);
		outputs[PITCH_GLOBAL_OUTPUT].setChannels(numActiveChannels);
		outputs[VELOCITY_GLOBAL_OUTPUT].setChannels(numActiveChannels);
		
		
		// float multiplier = INTERVALS[(int)params[INTERVAL_PARAM].getValue()].value / params[STEPS_PARAM].getValue() * params[FINE_PARAM].getValue();

		// for (int i = 0; i < 9; i++) {
			// if (outputs[PITCH_OUTPUT + i].isConnected()) {
				// int channels = inputs[PITCH_INPUT + i].getChannels();
				// outputs[PITCH_OUTPUT + i].setChannels(channels);
				
				// for (int channel = 0; channel < channels; channel += 4) {
					// outputs[PITCH_OUTPUT + i].setVoltageSimd(inputs[PITCH_INPUT + i].getPolyVoltageSimd<float_4>(channel) * multiplier, channel);
				// }
			// }
		// }
	}
};
