#include "Ahornberg.hpp"

using simd::float_4;

int initStep = 33;
int initInterval = 9;

struct CoirtInPort : app::SvgPort {
	CoirtInPort() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Coirt_InPort.svg")));
		shadow->box.pos = Vec(0, sw->box.size.y * 0.05f);

	}
};

struct CoirtOutPort : app::SvgPort {
	CoirtOutPort() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Coirt_OutPort.svg")));
		shadow->box.pos = Vec(0, sw->box.size.y * 0.05f);

	}
};


struct CoirtKnob28 : app::SvgKnob {
	CoirtKnob28() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Coirt_BlackKnob28.svg")));
		minAngle = -.8f * M_PI;
		maxAngle = .8f * M_PI;
		snap = true;
		shadow->box.pos = Vec(0, sw->box.size.y * 0.05f);
		//speed = 0.5f;
	}
};

struct CoirtKnob36 : app::SvgKnob {
	CoirtKnob36() {
		setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Coirt_BlackKnob36.svg")));
		minAngle = -.8f * M_PI;
		maxAngle = .8f * M_PI;
		shadow->box.pos = Vec(0, sw->box.size.y * 0.05f);
		speed = 0.7f;
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
		font = APP->window->loadFont(asset::plugin(pluginInstance, "res/hdad-segment14-1.002/Segment14.ttf"));
		backgroundColor = nvgRGB(0x28, 0x28, 0x28);
		borderColor = nvgRGB(0x00, 0x00, 0x00);
		textColor = nvgRGB(0xe8, 0xe8, 0x2c);
		smallTextColor = nvgRGB(0xaf, 0xe2, 0x2c);
		
	}

	void draw(const DrawArgs &disp) override {
		// Background
		nvgBeginPath(disp.vg);
		nvgRoundedRect(disp.vg, 0.0, 0.0, box.size.x, box.size.y, 2.0);
		nvgFillColor(disp.vg, backgroundColor);
		nvgFill(disp.vg);
		nvgStrokeWidth(disp.vg, 1.0);
		nvgStrokeColor(disp.vg, borderColor);
		nvgStroke(disp.vg);

		nvgFontSize(disp.vg, 10);
		nvgFontFaceId(disp.vg, font->handle);
		nvgTextLetterSpacing(disp.vg, 1);

		Vec textPos = Vec(4, 14);
		nvgFillColor(disp.vg, textColor);
		if (step < 10) {
			nvgText(disp.vg, textPos.x, textPos.y, (" " + std::to_string(step)).c_str(), NULL);
		} else {
			nvgText(disp.vg, textPos.x, textPos.y, (std::to_string(step)).c_str(), NULL);
		}
		nvgFillColor(disp.vg, smallTextColor);
		nvgFontSize(disp.vg, 8);
		nvgText(disp.vg, textPos.x + 21, textPos.y, "ed", NULL);
		nvgFillColor(disp.vg, textColor);
		nvgFontSize(disp.vg, 10);
		nvgText(disp.vg, textPos.x + 40, textPos.y, intervalValues[interval], NULL);
		//---------Gradient Screen
		NVGcolor gradStartCol = nvgRGBA(255, 255, 244, 27);
		NVGcolor gradEndCol = nvgRGBA(0, 0, 0, 25);
		nvgRoundedRect(disp.vg, 0.f, 0.f, box.size.x, box.size.y, .75f);
		float gradHeight = 13.673f;
		nvgFillPaint(disp.vg, nvgLinearGradient(disp.vg, 71.5f, gradHeight - 4.98f, 70.61f, gradHeight - 10.11f, gradStartCol, gradEndCol));
		nvgFill(disp.vg);
	}
};

struct tpInterval : ParamQuantity {
	std::string getDisplayValueString() override {
		//return intervalValues[(int) getValue()];
		if (getValue() == 0.f) { return "Major Second"; } 
		else if (getValue() == 1.f) { return "Supermajor Second"; } 
		else if (getValue() == 2.f) { return "Subminor Third"; } 
		else if (getValue() == 3.f) { return "Minor Third"; } 
		else if (getValue() == 4.f) { return "Major Third"; } 
		else if (getValue() == 5.f) { return "Perfect Fourth"; } 
		else if (getValue() == 6.f) { return "Perfect Fifth"; } 
		else if (getValue() == 7.f) { return "Octave"; } 
		else if (getValue() == 8.f) { return "Tritave (Octave + Perfect Fifth)"; } 
		else if (getValue() == 9.f) { return "2 Octaves"; } 
		else if (getValue() == 10.f) { return "2 Octaves + Major Third"; } 
		else if (getValue() == 11.f) { return "2 Octaves + Perfect Fifth"; } 
		else if (getValue() == 12.f) { return "2 Octaves + Subminor Seventh"; } 
		else return  "3 Octaves";
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
		configParam(STEPS_PARAM, 1, 99, initStep, "Steps");
		configParam<tpInterval>(INTERVAL_PARAM, 0, 13, initInterval, "Division");

	}
	
	void process(const ProcessArgs &args) override {
		float multiplier = intervals[(int)params[INTERVAL_PARAM].getValue()] / params[STEPS_PARAM].getValue() * params[FINE_PARAM].getValue();

		for (int i = 0; i < 9; i++) {
			if (outputs[PITCH_OUTPUT + i].isConnected()) {
				int channels = inputs[PITCH_INPUT + i].getChannels();
				outputs[PITCH_OUTPUT + i].setChannels(channels);
				
				for (int c = 0; c < channels; c += 4) {
					outputs[PITCH_OUTPUT + i].setVoltageSimd(inputs[PITCH_INPUT + i].getPolyVoltageSimd<float_4>(c) * multiplier, c);
				}
			}
		}
		
		intervalDisplay->step = (int)params[STEPS_PARAM].getValue();
		intervalDisplay->interval = (int)params[INTERVAL_PARAM].getValue();
	}

	float intervals[14] = {
		log2f(9.f / 8.f) * 12,
		log2f(8.f / 7.f) * 12,
		log2f(7.f / 6.f) * 12,
		log2f(6.f / 5.f) * 12,
		log2f(5.f / 4.f) * 12,
		log2f(4.f / 3.f) * 12,
		log2f(3.f / 2.f) * 12,
		log2f(2.f) * 12,
		log2f(3.f) * 12,
		log2f(4.f) * 12,
		log2f(5.f) * 12,
		log2f(6.f) * 12,
		log2f(7.f) * 12,
		log2f(8.f) * 12
	};

	Display *intervalDisplay;

};

struct EqualDivisionWidget : ModuleWidget {
	EqualDivisionWidget(EqualDivision *module) {
		setModule(module);
		setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, "res/EqualDivision.svg")));
		box.size = Vec(90, 380);

		addChild(createWidget<ScrewSilver>(Vec(0, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParam<CoirtKnob36>(Vec(46.5, 47), module, EqualDivision::FINE_PARAM));
		addParam(createParam<CoirtKnob28>(Vec(10, 124), module, EqualDivision::STEPS_PARAM));
		addParam(createParam<CoirtKnob28>(Vec(box.size.x - 37, 124), module, EqualDivision::INTERVAL_PARAM));
	
		addInput(createInput<CoirtInPort>(Vec(4, 167), module, EqualDivision::PITCH_INPUT + 0));
		addInput(createInput<CoirtInPort>(Vec(33, 167), module, EqualDivision::PITCH_INPUT + 1));
		addInput(createInput<CoirtInPort>(Vec(62, 167), module, EqualDivision::PITCH_INPUT + 2));
		addInput(createInput<CoirtInPort>(Vec(4, 226 + 7.5), module, EqualDivision::PITCH_INPUT + 3));
		addInput(createInput<CoirtInPort>(Vec(33, 226 + 3.75), module, EqualDivision::PITCH_INPUT + 4));
		addInput(createInput<CoirtInPort>(Vec(62, 226), module, EqualDivision::PITCH_INPUT + 5));
		addInput(createInput<CoirtInPort>(Vec(4, 300), module, EqualDivision::PITCH_INPUT + 6));
		addInput(createInput<CoirtInPort>(Vec(33, 292.5), module, EqualDivision::PITCH_INPUT + 7));
		addInput(createInput<CoirtInPort>(Vec(62, 285), module, EqualDivision::PITCH_INPUT + 8));
	
		addOutput(createOutput<CoirtOutPort>(Vec(4, 194), module, EqualDivision::PITCH_OUTPUT + 0));
		addOutput(createOutput<CoirtOutPort>(Vec(33, 194), module, EqualDivision::PITCH_OUTPUT + 1));
		addOutput(createOutput<CoirtOutPort>(Vec(62, 194), module, EqualDivision::PITCH_OUTPUT + 2));
		addOutput(createOutput<CoirtOutPort>(Vec(4, 253 + 7.5), module, EqualDivision::PITCH_OUTPUT + 3));
		addOutput(createOutput<CoirtOutPort>(Vec(33, 253 + 3.75), module, EqualDivision::PITCH_OUTPUT + 4));
		addOutput(createOutput<CoirtOutPort>(Vec(62, 253), module, EqualDivision::PITCH_OUTPUT + 5));
		addOutput(createOutput<CoirtOutPort>(Vec(4, 327), module, EqualDivision::PITCH_OUTPUT + 6));
		addOutput(createOutput<CoirtOutPort>(Vec(33, 319.5), module, EqualDivision::PITCH_OUTPUT + 7));
		addOutput(createOutput<CoirtOutPort>(Vec(62, 312), module, EqualDivision::PITCH_OUTPUT + 8));

		if (module) {
			module->intervalDisplay = new Display();
			module->intervalDisplay->box.pos = Vec(8.5, 98);
			module->intervalDisplay->box.size = Vec(74, 18);
			module->intervalDisplay->step = initStep;
			module->intervalDisplay->interval = initInterval;
			addChild(module->intervalDisplay);
		}
	}
};

Model *modelEqualDivision = createModel<EqualDivision, EqualDivisionWidget>("EqualDivision");
