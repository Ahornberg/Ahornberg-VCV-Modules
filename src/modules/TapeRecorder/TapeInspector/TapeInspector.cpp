#include "TapeInspector.hpp"

TapeInspector::TapeInspector() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configParam(TIME_PARAM, 0.4, 1, 1, "Time");
	configParam(AMPLITUDE_PARAM, 0.5, 1.5, 1, "Amplitude");
	tapeRecorder = NULL;
	sampleTime = 1.f / 44100.f;
}

void TapeInspector::process(const ProcessArgs& args) {
	sampleTime = args.sampleTime;
}

void TapeInspector::onExpanderChange(const ExpanderChangeEvent& e) {
	if (!e.side) {
		Module* leftModule = getLeftExpander().module;
		if (leftModule && leftModule->model == modelTapeRecorder) {
			tapeRecorder = dynamic_cast<TapeRecorder*>(leftModule);
		} else {
			tapeRecorder = NULL;
		}
	}
}
