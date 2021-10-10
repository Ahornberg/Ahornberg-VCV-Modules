#include "Harmonizer.hpp"

float Harmonizer::FREQ_RATIOS[NUM_HARMONICS] = {};
bool Harmonizer::FREQ_RATIOS_INITIALIZED = false;

Harmonizer::Harmonizer() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	configParam(VELOCITY_GLOBAL_MODULATION_PARAM,  0, 1, 0, "Velocity Modulation", "%", 0, 100);
	configParam(PITCH_GLOBAL_MODULATION_PARAM,    -1, 1, 0, "Pitch Modulation", "%", 0, 100);
	configInput(GATE_INPUT, "Gate");
	configOutput(GATE_OUTPUT, "Gate");
	configBypass(GATE_INPUT, GATE_OUTPUT);
	configInput(RETRIGGER_INPUT, "Retrigger");
	configOutput(RETRIGGER_OUTPUT, "Retrigger");
	configBypass(RETRIGGER_INPUT, RETRIGGER_OUTPUT);
	configInput(VELOCITY_INPUT, "Velocity");
	configInput(VELOCITY_GLOBAL_MODULATION_INPUT, "Velocity Modulation");
	configOutput(VELOCITY_GLOBAL_OUTPUT, "Velocity");
	configBypass(VELOCITY_INPUT, VELOCITY_GLOBAL_OUTPUT);
	configInput(PITCH_INPUT, "Pitch (1V/Octave)");
	configInput(PITCH_GLOBAL_MODULATION_INPUT, "Pitch Modulation");
	configOutput(PITCH_GLOBAL_OUTPUT, "Pitch (1V/Octave)");
	configBypass(PITCH_INPUT, PITCH_GLOBAL_OUTPUT);
	for (auto i = 0; i < NUM_CHANNELS; ++i) {
		configSwitch(CHANNEL_ACTIVE_PARAM + i, 0,     1,    i ? 0 : 1, string::f("Channel %d", i + 1), BasicSwitch::ON_OFF_NAMES);
		configParam(VELOCITY_MODULATION_PARAM + i,   0,     1,    0, string::f("Velocity Modulation Channel %d", i + 1), "%", 0, 100);
		configParam(VELOCITY_PARAM + i,              0,     1,    1, string::f("Velocity Channel %d", i + 1), "%", 0, 100);
		configParam(PITCH_MODULATION_PARAM + i,     -1,     1,    0, string::f("Pitch Modulation Channel %d", i + 1), "%", 0, 100);
		configParam(PITCH_HARMONICS_PARAM + i,       1, NUM_HARMONICS, 1, string::f("Pitch Harmonics Channel %d", i + 1));
		configParam(PITCH_SUBHARMONICS_PARAM + i,    1, NUM_HARMONICS, 1, string::f("Pitch Subarmonics Channel %d", i + 1));
		configParam(PITCH_FINE_PARAM + i,            0.97,  1.03, 1, string::f("Pitch Fine Channel %d", i + 1), "%", 0, 100, -100);
		std::string portLabel = "Velocity Modulation Channel " + std::to_string(i + 1);
		configInput(VELOCITY_MODULATION_INPUT + i, portLabel);
		portLabel = "Velocity Channel " + std::to_string(i + 1);
		configOutput(VELOCITY_OUTPUT + i, portLabel);
		configBypass(VELOCITY_INPUT, VELOCITY_OUTPUT + i);
		portLabel = "Pitch Modulation Channel " + std::to_string(i + 1);
		configInput(PITCH_MODULATION_INPUT + i, portLabel);
		portLabel = "Pitch Channel " + std::to_string(i + 1) + " (1V/Octave)";
		configOutput(PITCH_OUTPUT + i, portLabel);
		configBypass(PITCH_INPUT, PITCH_OUTPUT + i);
		if (i < NUM_MIXTURES) {
			portLabel = "Mixtur " + std::to_string(i + 1) + " Trigger";
			configSwitch(MIXTUR_PARAM + i,     0,     1,    0, string::f("Mixtur %d", i + 1), BasicSwitch::ON_OFF_NAMES);
			mixtureTriggers[i].reset();
			configInput(MIXTUR_INPUT + i, portLabel);
		}
	}
	if (!FREQ_RATIOS_INITIALIZED) {
		for (auto i = 0; i < NUM_HARMONICS; ++i) {
			FREQ_RATIOS[i] = log2f(i + 1.);
		}
		FREQ_RATIOS_INITIALIZED = true;
	}
}

void Harmonizer::setMixture(int mixture) {
	for (auto i = 0; i < NUM_MIXTURES; ++i) {
		if (i == mixture) {
			params[MIXTUR_PARAM + i].setValue(1);
		} else {
			params[MIXTUR_PARAM + i].setValue(0);
		}
	}
}

int Harmonizer::processMixtureTriggers() {
	for (auto i = 0; i < NUM_MIXTURES; ++i) {
		if (inputs[MIXTUR_INPUT + i].isConnected()) {
			if (mixtureTriggers[i].process(rescaleInput(inputs[MIXTUR_INPUT + i]))) {
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

void Harmonizer::process(const ProcessArgs& args) {
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
			float freqMultiplier = FREQ_RATIOS[(int) params[PITCH_HARMONICS_PARAM + i].getValue() - 1] - FREQ_RATIOS[(int) params[PITCH_SUBHARMONICS_PARAM + i].getValue() - 1];
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
