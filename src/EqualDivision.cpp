#include "Ahornberg.hpp"

struct SnapKnob : Davies1900hBlackKnob {
	SnapKnob() {
		// minAngle = -1.83*M_PI;
		// maxAngle = 1.83*M_PI;
		snap = true;
		box.size = Vec(28, 28);
	}
};

static const char *intervalValues[] = {
	"9/8",
	"8/7",
	"7/6",
	"6/5",
	"5/4",
	"4/3",
	"3/2",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8"
};

struct Display : TransparentWidget {
	int step;
	int interval;
	std::shared_ptr<Font> font;
	NVGcolor backgroundColor;
	NVGcolor borderColor;
	NVGcolor textColor;
	NVGcolor smallTextColor;

	Display() {
		font = Font::load(assetPlugin(plugin, "res/hdad-segment14-1.002/Segment14.ttf"));
		backgroundColor = nvgRGB(0x28, 0x28, 0x28);
		borderColor = nvgRGB(0x00, 0x00, 0x00);
		textColor = nvgRGB(0xe8, 0xe8, 0x2c);
		smallTextColor = nvgRGB(0xaf, 0xe2, 0x2c);
	}

	void draw(NVGcontext *vg) override {
		// Background
		nvgBeginPath(vg);
		nvgRoundedRect(vg, 0.0, 0.0, box.size.x, box.size.y, 2.0);
		nvgFillColor(vg, backgroundColor);
		nvgFill(vg);
		nvgStrokeWidth(vg, 1.0);
		nvgStrokeColor(vg, borderColor);
		nvgStroke(vg);

		nvgFontSize(vg, 10);
		nvgFontFaceId(vg, font->handle);
		nvgTextLetterSpacing(vg, 1);

		Vec textPos = Vec(4, 14);
		nvgFillColor(vg, textColor);
		if (step < 10) {
			nvgText(vg, textPos.x, textPos.y, (" " + std::to_string(step)).c_str(), NULL);
		} else {
			nvgText(vg, textPos.x, textPos.y, (std::to_string(step)).c_str(), NULL);
		}
		nvgFillColor(vg, smallTextColor);
		nvgFontSize(vg, 8);
		nvgText(vg, textPos.x + 21, textPos.y, "ed", NULL);
		nvgFillColor(vg, textColor);
		nvgFontSize(vg, 10);
		nvgText(vg, textPos.x + 40, textPos.y, intervalValues[interval], NULL);
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
		PITCH_INPUT_1,
		PITCH_INPUT_2,
		PITCH_INPUT_3,
		PITCH_INPUT_4,
		PITCH_INPUT_5,
		PITCH_INPUT_6,
		PITCH_INPUT_7,
		PITCH_INPUT_8,
		PITCH_INPUT_9,
		NUM_INPUTS
	};
	enum OutputIds {
		PITCH_OUTPUT_1,
		PITCH_OUTPUT_2,
		PITCH_OUTPUT_3,
		PITCH_OUTPUT_4,
		PITCH_OUTPUT_5,
		PITCH_OUTPUT_6,
		PITCH_OUTPUT_7,
		PITCH_OUTPUT_8,
		PITCH_OUTPUT_9,
		NUM_OUTPUTS
	};
	EqualDivision() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}
	float intervals[14] = {
		log2f(9.0/8.0) * 12,
		log2f(8.0/7.0) * 12,
		log2f(7.0/6.0) * 12,
		log2f(6.0/5.0) * 12,
		log2f(5.0/4.0) * 12,
		log2f(4.0/3.0) * 12,
		log2f(3.0/2.0) * 12,
		log2f(2.0) * 12,
		log2f(3.0) * 12,
		log2f(4.0) * 12,
		log2f(5.0) * 12,
		log2f(6.0) * 12,
		log2f(7.0) * 12,
		log2f(8.0) * 12
	};
	Display *intervalDisplay;
	void step() override;

	// For more advanced Module features, read Rack's engine.hpp header file
	// - toJson, fromJson: serialization of internal data
	// - onSampleRateChange: event triggered by a change of sample rate
	// - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};

void EqualDivision::step() {
	float multiplier = intervals[(int) params[INTERVAL_PARAM].value] / params[STEPS_PARAM].value * params[FINE_PARAM].value;
	if (outputs[PITCH_OUTPUT_1].active)
		outputs[PITCH_OUTPUT_1].value = clampf(inputs[PITCH_INPUT_1].value * multiplier, -5.0, 5.0);
	if (outputs[PITCH_OUTPUT_2].active)
		outputs[PITCH_OUTPUT_2].value = clampf(inputs[PITCH_INPUT_2].value * multiplier, -5.0, 5.0);
	if (outputs[PITCH_OUTPUT_3].active)
		outputs[PITCH_OUTPUT_3].value = clampf(inputs[PITCH_INPUT_3].value * multiplier, -5.0, 5.0);
	if (outputs[PITCH_OUTPUT_4].active)
		outputs[PITCH_OUTPUT_4].value = clampf(inputs[PITCH_INPUT_4].value * multiplier, -5.0, 5.0);
	if (outputs[PITCH_OUTPUT_5].active)
		outputs[PITCH_OUTPUT_5].value = clampf(inputs[PITCH_INPUT_5].value * multiplier, -5.0, 5.0);
	if (outputs[PITCH_OUTPUT_6].active)
		outputs[PITCH_OUTPUT_6].value = clampf(inputs[PITCH_INPUT_6].value * multiplier, -5.0, 5.0);
	if (outputs[PITCH_OUTPUT_7].active)
		outputs[PITCH_OUTPUT_7].value = clampf(inputs[PITCH_INPUT_7].value * multiplier, -5.0, 5.0);
	if (outputs[PITCH_OUTPUT_8].active)
		outputs[PITCH_OUTPUT_8].value = clampf(inputs[PITCH_INPUT_8].value * multiplier, -5.0, 5.0);
	if (outputs[PITCH_OUTPUT_9].active)
		outputs[PITCH_OUTPUT_9].value = clampf(inputs[PITCH_INPUT_9].value * multiplier, -5.0, 5.0);
	intervalDisplay->step = (int) params[STEPS_PARAM].value;
	intervalDisplay->interval = (int) params[INTERVAL_PARAM].value;
}


EqualDivisionWidget::EqualDivisionWidget() {
	EqualDivision *module = new EqualDivision();
	int initStep = 33;
	int initInterval = 9;
	setModule(module);
	box.size = Vec(6 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

	{
		SVGPanel *panel = new SVGPanel();
		panel->box.size = box.size;
		panel->setBackground(SVG::load(assetPlugin(plugin, "res/EqualDivision.svg")));
		addChild(panel);
	}

	{
		module->intervalDisplay = new Display();
		module->intervalDisplay->box.pos = Vec(8.5, 98);
		module->intervalDisplay->box.size = Vec(74, 18);
		module->intervalDisplay->step = initStep;
		module->intervalDisplay->interval = initInterval;
		addChild(module->intervalDisplay);
	}
	
	addChild(createScrew<ScrewSilver>(Vec(0, 0)));
	//addChild(createScrew<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
	//addChild(createScrew<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
	addChild(createScrew<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

	addParam(createParam<Davies1900hBlackKnob>(Vec(46.5, 47), module, EqualDivision::FINE_PARAM, 0.97, 1.03, 1.0));//28
	addParam(createParam<SnapKnob>(Vec(10, 124), module, EqualDivision::STEPS_PARAM, 1, 99, initStep));
	addParam(createParam<SnapKnob>(Vec(box.size.x - 37, 124), module, EqualDivision::INTERVAL_PARAM, 0, 13, initInterval));

	addInput(createInput<PJ301MPort>(Vec( 4, 167), module, EqualDivision::PITCH_INPUT_1));
	addInput(createInput<PJ301MPort>(Vec(33, 167), module, EqualDivision::PITCH_INPUT_2));
	addInput(createInput<PJ301MPort>(Vec(62, 167), module, EqualDivision::PITCH_INPUT_3));
	addInput(createInput<PJ301MPort>(Vec( 4, 226 + 7.5), module, EqualDivision::PITCH_INPUT_4));
	addInput(createInput<PJ301MPort>(Vec(33, 226 + 3.75), module, EqualDivision::PITCH_INPUT_5));
	addInput(createInput<PJ301MPort>(Vec(62, 226), module, EqualDivision::PITCH_INPUT_6));
	addInput(createInput<PJ301MPort>(Vec( 4, 300), module, EqualDivision::PITCH_INPUT_7));
	addInput(createInput<PJ301MPort>(Vec(33, 292.5), module, EqualDivision::PITCH_INPUT_8));
	addInput(createInput<PJ301MPort>(Vec(62, 285), module, EqualDivision::PITCH_INPUT_9));
	addOutput(createOutput<PJ301MPort>(Vec( 4, 194), module, EqualDivision::PITCH_OUTPUT_1));
	addOutput(createOutput<PJ301MPort>(Vec(33, 194), module, EqualDivision::PITCH_OUTPUT_2));
	addOutput(createOutput<PJ301MPort>(Vec(62, 194), module, EqualDivision::PITCH_OUTPUT_3));
	addOutput(createOutput<PJ301MPort>(Vec( 4, 253 + 7.5), module, EqualDivision::PITCH_OUTPUT_4));
	addOutput(createOutput<PJ301MPort>(Vec(33, 253 + 3.75), module, EqualDivision::PITCH_OUTPUT_5));
	addOutput(createOutput<PJ301MPort>(Vec(62, 253), module, EqualDivision::PITCH_OUTPUT_6));
	addOutput(createOutput<PJ301MPort>(Vec( 4, 327), module, EqualDivision::PITCH_OUTPUT_7));
	addOutput(createOutput<PJ301MPort>(Vec(33, 319.5), module, EqualDivision::PITCH_OUTPUT_8));
	addOutput(createOutput<PJ301MPort>(Vec(62, 312), module, EqualDivision::PITCH_OUTPUT_9));
}
