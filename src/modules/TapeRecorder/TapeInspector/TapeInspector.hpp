#include "../TapeRecorder.hpp"

struct TapeInspector : Module {

	enum ParamIds {
		TIME_PARAM,
		AMPLITUDE_PARAM,
		NUM_PARAMS
	};
	
	enum InputIds {
		NUM_INPUTS
	};
	
	enum OutputIds {
		NUM_OUTPUTS
	};
	
	enum LightIds {
		NUM_LIGHTS
	};
	
	TapeRecorder* tapeRecorder;
	float sampleTime;
	
	TapeInspector();
	void process(const ProcessArgs& args) override;
	void onExpanderChange(const ExpanderChangeEvent& e) override;
};
