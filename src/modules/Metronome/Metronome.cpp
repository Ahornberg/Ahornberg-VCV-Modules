#include "Metronome.hpp"

const int Metronome::BPM_VALUES[] = {
	40,	42, 44, 46, 48, 50, 52, 54, 56, 58,	60, 63, 66, 69, 72, 76, 80, 84, 88, 92,	96,
	100, 104, 108, 112, 116, 120, 126, 132, 138, 144, 152, 160, 168, 176, 184, 192, 200, 208
};

std::string BpmValues::getDisplayValueString() {
	return string::f("%d", Metronome::BPM_VALUES[(int) getValue()]);
}

Metronome::Metronome() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configScrewParams();
	for (auto i = 0; i < NUM_BPM_VALUES; ++i) {
		configParam(BPM_VALUE_BUTTON_PARAM + i, 0, 1, i == INIT_BPM ? 1 : 0, string::f("%d BPM", BPM_VALUES[i]));
	}
	configParam<BpmValues>(BPM_VALUE_PARAM, 0, NUM_BPM_VALUES - 1, INIT_BPM, "BPM");
	configParam<BpmValues>(BPM_RESET_VALUE_PARAM, 0, NUM_BPM_VALUES - 1, INIT_BPM, "BPM");
	configParam(PLAY_PARAM, 0, 1, 0, "Play / Stop");
	pulse.reset();
	timer.reset();
	threeStepFasterTrigger.reset();
	twoStepFasterTrigger.reset();
	oneStepFasterTrigger.reset();
	oneStepSlowerTrigger.reset();
	bpmResetTrigger.reset();
	playTrigger.reset();
}

void Metronome::process(const ProcessArgs &args) {
	if (outputs[BPM_OUTPUT].isConnected() && params[PLAY_PARAM].getValue()) {
		float time = timer.process(args.sampleTime);
		if (time >= 60. / BPM_VALUES[(int) params[BPM_VALUE_PARAM].getValue()]) {
			pulse.trigger();
			timer.reset();
		} else if (time == args.sampleTime) {
			pulse.trigger();
		}
		if (pulse.process(args.sampleTime)) {
			outputs[BPM_OUTPUT].setVoltage(10);
		} else {
			outputs[BPM_OUTPUT].setVoltage(0);
		}
		
	}
	if (inputs[THREE_STEP_FASTER_INPUT].isConnected()) {
		if (threeStepFasterTrigger.process(rescale(inputs[THREE_STEP_FASTER_INPUT].getVoltage(), 0.1, 2, 0, 1))) {
			int bpmIndex = params[BPM_VALUE_PARAM].getValue() + 3;
			while (bpmIndex >= NUM_BPM_VALUES) {
				bpmIndex -= NUM_BPM_VALUES;
			}
			params[BPM_VALUE_PARAM].setValue(bpmIndex);
		}
	}
	if (inputs[TWO_STEP_FASTER_INPUT].isConnected()) {
		if (twoStepFasterTrigger.process(rescale(inputs[TWO_STEP_FASTER_INPUT].getVoltage(), 0.1, 2, 0, 1))) {
			int bpmIndex = params[BPM_VALUE_PARAM].getValue() + 2;
			while (bpmIndex >= NUM_BPM_VALUES) {
				bpmIndex -= NUM_BPM_VALUES;
			}
			params[BPM_VALUE_PARAM].setValue(bpmIndex);
		}
	}
	if (inputs[ONE_STEP_FASTER_INPUT].isConnected()) {
		if (oneStepFasterTrigger.process(rescale(inputs[ONE_STEP_FASTER_INPUT].getVoltage(), 0.1, 2, 0, 1))) {
			int bpmIndex = params[BPM_VALUE_PARAM].getValue();
			if (++bpmIndex >= NUM_BPM_VALUES) {
				bpmIndex = 0;
			}
			params[BPM_VALUE_PARAM].setValue(bpmIndex);
		}
	}
	if (inputs[ONE_STEP_SLOWER_INPUT].isConnected()) {
		if (oneStepSlowerTrigger.process(rescale(inputs[ONE_STEP_SLOWER_INPUT].getVoltage(), 0.1, 2, 0, 1))) {
			int bpmIndex = params[BPM_VALUE_PARAM].getValue();
			if (--bpmIndex <= 0) {
				bpmIndex = NUM_BPM_VALUES - 1;
			}
			params[BPM_VALUE_PARAM].setValue(bpmIndex);
		}
	}
	if (inputs[BPM_RESET_INPUT].isConnected()) {
		if (bpmResetTrigger.process(rescale(inputs[BPM_RESET_INPUT].getVoltage(), 0.1, 2, 0, 1))) {
			params[BPM_VALUE_PARAM].setValue(params[BPM_RESET_VALUE_PARAM].getValue());
		}
	}
	if (inputs[PLAY_INPUT].isConnected()) {
		if (playTrigger.process(rescale(inputs[PLAY_INPUT].getVoltage(), 0.1, 2, 0, 1))) {
			params[PLAY_PARAM].getValue() ? params[PLAY_PARAM].setValue(0) : params[PLAY_PARAM].setValue(1);
			timer.reset();
		}
	}
}
