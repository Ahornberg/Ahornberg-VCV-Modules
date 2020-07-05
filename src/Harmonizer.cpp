#include "Ahornberg.hpp"

constexpr int NUM_HARMONICS = 96;

static float FREQ_RATIOS[NUM_HARMONICS][NUM_HARMONICS];

constexpr int NUM_CHANNELS = 16;
constexpr int NUM_MIXTURES = 4;

struct Harmonizer : ModuleWithScrews {
	enum ParamIds {
		VELOCITY_GLOBAL_MODULATION_PARAM = NUM_MAX_SCREWS,
		ENUMS(VELOCITY_MODULATION_PARAM, NUM_CHANNELS),
		ENUMS(VELOCITY_PARAM, NUM_CHANNELS),
		PITCH_GLOBAL_MODULATION_PARAM,
		ENUMS(PITCH_MODULATION_PARAM, NUM_CHANNELS),
		ENUMS(PITCH_HARMONICS_PARAM, NUM_CHANNELS),
		ENUMS(PITCH_SUBHARMONICS_PARAM, NUM_CHANNELS),
		ENUMS(PITCH_FINE_PARAM, NUM_CHANNELS),
		ENUMS(CHANNEL_ACTIVE_PARAM, NUM_CHANNELS),
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
		ENUMS(MIXTUR_INPUT, NUM_MIXTURES),
		NUM_INPUTS
	};
	enum OutputIds {
		GATE_OUTPUT,
		RETRIGGER_OUTPUT,
		VELOCITY_GLOBAL_OUTPUT,
		ENUMS(VELOCITY_OUTPUT, NUM_CHANNELS),
		PITCH_GLOBAL_OUTPUT,
		ENUMS(PITCH_OUTPUT, NUM_CHANNELS),
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	dsp::SchmittTrigger mixtureTriggers[NUM_MIXTURES];


	Harmonizer() { 
		config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
		configScrewParams();
		configParam(VELOCITY_GLOBAL_MODULATION_PARAM,  0, 1, 0, "Velocity Modulation");
		configParam(PITCH_GLOBAL_MODULATION_PARAM,    -1, 1, 0, "Pitch Modulation");
		for (auto i = 0; i < NUM_CHANNELS; ++i) {
			configParam(CHANNEL_ACTIVE_PARAM + i,       0,     1,    0, string::f("Channel %d active", i + 1));
			configParam(VELOCITY_MODULATION_PARAM + i,  0,     1,    0, string::f("Velocity Modulation Channel %d", i + 1));
			configParam(VELOCITY_PARAM + i,             0,     1,    1, string::f("Velocity Channel %d", i + 1));
			configParam(PITCH_MODULATION_PARAM + i,    -1,     1,    0, string::f("Pitch Modulation Channel %d", i + 1));
			configParam(PITCH_HARMONICS_PARAM + i,      1, NUM_HARMONICS, 1, string::f("Pitch Harmonics Channel %d", i + 1));
			configParam(PITCH_SUBHARMONICS_PARAM + i,   1, NUM_HARMONICS, 1, string::f("Pitch Subarmonics Channel %d", i + 1));
			configParam(PITCH_FINE_PARAM + i,           0.97,  1.03, 1, string::f("Pitch Fine Channel %d", i + 1), "%", 0, 100, -100);
			if (i < NUM_MIXTURES) {
				configParam(MIXTUR_PARAM + i,           0,     1,    0, string::f("Mixtur %d active", i + 1));
				mixtureTriggers[i].reset();
			}
		}
		params[CHANNEL_ACTIVE_PARAM].setValue(1);
		for (auto i = 0; i < NUM_HARMONICS; ++i) {
			for (auto j = 0; j < NUM_HARMONICS; ++j) {
				FREQ_RATIOS[i][j] = log2f((i + 1.) / (j + 1.));
			}
		}
	}
	
	void setMixture(int mixture) {
		for (auto i = 0; i < NUM_MIXTURES; ++i) {
			if (i == mixture) {
				params[MIXTUR_PARAM + i].setValue(1);
			} else {
				params[MIXTUR_PARAM + i].setValue(0);
			}
		}
	}
	
	int processMixtureTriggers() {
		for (auto i = 0; i < NUM_MIXTURES; ++i) {
			if (inputs[MIXTUR_INPUT + i].isConnected()) {
				if (mixtureTriggers[i].process(rescale(inputs[MIXTUR_INPUT + i].getVoltage(), 0.1, 2, 0, 1))) {
					setMixture(i);
				}
			}
		}
		int activeMixture = 0;
		for (auto i = 0; i < NUM_MIXTURES; ++i) {
			if (params[MIXTUR_PARAM + i].getValue()) {
				activeMixture = i + 1;
				break;
			}
		}
		return activeMixture;
	}

	void process(const ProcessArgs &args) override {
		int activeMixture = processMixtureTriggers();

		float gateIn = inputs[GATE_INPUT].getVoltage();
		float retriggerIn = inputs[RETRIGGER_INPUT].getVoltage();
		float pitchGlobalIn = inputs[PITCH_INPUT].getVoltage();
		if (inputs[PITCH_GLOBAL_MODULATION_INPUT].isConnected()) {
			pitchGlobalIn += inputs[PITCH_GLOBAL_MODULATION_INPUT].getVoltage() * powf(params[PITCH_GLOBAL_MODULATION_PARAM].getValue(), 4);
		}
		float velocityIn = inputs[VELOCITY_INPUT].getVoltage();
		if (inputs[VELOCITY_GLOBAL_MODULATION_INPUT].isConnected()) {
			velocityIn += inputs[VELOCITY_GLOBAL_MODULATION_INPUT].getVoltage() * params[VELOCITY_GLOBAL_MODULATION_PARAM].getValue();
		}
		int numActiveChannels = 0;
		int firstChannel = 0;
		int lastChannel = NUM_CHANNELS;
		if (activeMixture) {
			firstChannel = (activeMixture - 1) * NUM_CHANNELS / NUM_MIXTURES;
			lastChannel = firstChannel + NUM_CHANNELS / NUM_MIXTURES;
		}
		for (auto i = firstChannel; i < lastChannel; ++i) {
			if (params[CHANNEL_ACTIVE_PARAM + i].getValue()) {
				outputs[GATE_OUTPUT].setVoltage(gateIn, numActiveChannels);
				outputs[RETRIGGER_OUTPUT].setVoltage(retriggerIn, numActiveChannels);
				
				float pitchModIn = 0;
				if (inputs[PITCH_MODULATION_INPUT + i].isConnected()) {
					pitchModIn = inputs[PITCH_MODULATION_INPUT + i].getVoltage() * powf(params[PITCH_MODULATION_PARAM + i].getValue(), 4);
				}
				float freqMultiplier = FREQ_RATIOS[(int) params[PITCH_HARMONICS_PARAM + i].getValue() - 1][(int) params[PITCH_SUBHARMONICS_PARAM + i].getValue() - 1];
				float pitchOut = pitchModIn + pitchGlobalIn + freqMultiplier + params[PITCH_FINE_PARAM + i].getValue() - 1;
				outputs[PITCH_GLOBAL_OUTPUT].setVoltage(pitchOut, numActiveChannels);
				if (outputs[PITCH_OUTPUT + i].isConnected()) {
					outputs[PITCH_OUTPUT + i].setVoltage(pitchOut);
				}
				
				float velocityModIn = 0;
				if (inputs[VELOCITY_MODULATION_INPUT + i].isConnected()) {
					velocityModIn = inputs[VELOCITY_MODULATION_INPUT + i].getVoltage() * params[VELOCITY_MODULATION_PARAM + i].getValue();
				}
				float velocityMultiplier = params[VELOCITY_PARAM + i].getValue();
				float velocityOut = velocityModIn + velocityIn * velocityMultiplier;
				outputs[VELOCITY_GLOBAL_OUTPUT].setVoltage(velocityOut, numActiveChannels);
				if (outputs[VELOCITY_OUTPUT + i].isConnected()) {
					outputs[VELOCITY_OUTPUT + i].setVoltage(velocityOut);
				}
				++numActiveChannels;
			}
		}
		outputs[GATE_OUTPUT].setChannels(numActiveChannels);
		outputs[RETRIGGER_OUTPUT].setChannels(numActiveChannels);
		outputs[PITCH_GLOBAL_OUTPUT].setChannels(numActiveChannels);
		outputs[VELOCITY_GLOBAL_OUTPUT].setChannels(numActiveChannels);
	}
};
