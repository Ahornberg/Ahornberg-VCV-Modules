#include "Ahornberg.hpp"

struct MicroTuner1 : Module {
	enum ParamIds {
		FINE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		PITCH_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		PITCH_OUTPUT,
		PITCH_33ED4_OUTPUT,
		NUM_OUTPUTS
	};
	float lastTuned;

	MicroTuner1() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void MicroTuner1::step() {
	float tuned = inputs[PITCH_INPUT].value * params[FINE_PARAM].value;
	if (tuned != lastTuned) {
		// Compute 12edo tretched or compressed
		outputs[PITCH_OUTPUT].value = tuned;
		// Compute the Equivocal Tuning http://xenharmonic.wikispaces.com/33ed4
		outputs[PITCH_33ED4_OUTPUT].value = tuned * 8.0 / 11.0;
		lastTuned = tuned;
	}
}


MicroTuner1Widget::MicroTuner1Widget() {
	MicroTuner1 *module = new MicroTuner1();
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/MicroTuner.svg")));
		addChild(panel);
	}

	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
	addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

	addParam(createParam<Davies1900hBlackKnob>(Vec(28, 87), module, MicroTuner1::FINE_PARAM, 0.97, 1.03, 1.0));

	addInput(createInput<PJ301MPort>(Vec(33, 170), module, MicroTuner1::PITCH_INPUT));
	addOutput(createOutput<PJ301MPort>(Vec(33, 230), module, MicroTuner1::PITCH_33ED4_OUTPUT));
	addOutput(createOutput<PJ301MPort>(Vec(33, 280), module, MicroTuner1::PITCH_OUTPUT));
}
