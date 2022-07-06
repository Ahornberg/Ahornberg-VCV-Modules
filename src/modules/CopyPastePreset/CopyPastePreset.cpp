#include "CopyPastePreset.hpp"

CopyPastePreset::CopyPastePreset() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();

	configButton(DO_COPY_PASTE_PARAM, "copy preset from the left and paste to the right");
	configInput(LINK_CV_INPUT, "CV link");
	configOutput(LINK_CV_OUTPUT, "CV link");
	configBypass(LINK_CV_INPUT, LINK_CV_OUTPUT);
	pulse.reset();
}

void CopyPastePreset::process(const ProcessArgs& args) {
	float paramValue = params[DO_COPY_PASTE_PARAM].getValue();
	float inputValue = 0;
	
	if (inputs[LINK_CV_INPUT].isConnected()) {
		inputValue = inputs[LINK_CV_INPUT].getVoltage();
	}
	
	bool pulseState = pulse.process(args.sampleTime);
	bool paramTrigger = !oldParamValue && paramValue;
	bool inputTrigger = (oldInputValue < 1.f) && (inputValue >= 1.f);

	oldParamValue = paramValue;
	oldInputValue = inputValue;

	if (!pulseState && (paramTrigger || inputTrigger)) {
		pulse.trigger();
		params[DO_COPY_PASTE_PARAM].setValue(1);
	}
	
	if (outputs[LINK_CV_OUTPUT].isConnected()) {
		if (pulseState) {
			outputs[LINK_CV_OUTPUT].setVoltage(10);
		} else {
			outputs[LINK_CV_OUTPUT].setVoltage(0);
		}
	}
}
