#include "XGlideWidget.hpp"

XGlideWidget::XGlideWidget(XGlide* module) {
	setModule(module);
	setPanel("res/XGlide.svg");
	setWidthInHP(4);

	addParam(createParamCentered<KnobTinySnap>(Vec(15, 68), module, XGlide::MIN_NOTE_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(45, 68), module, XGlide::MAX_NOTE_PARAM));
	addChild(createLightCentered<SmallLight<RedLight>>(Vec(30, 68), module, XGlide::EXCLUDE_NOTES_LIGHT));
	
	addParam(createParamCentered<KnobTinySnap>(Vec(15, 118), module, XGlide::MIN_INTERVAL_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(45, 118), module, XGlide::MAX_INTERVAL_PARAM));
	addChild(createLightCentered<SmallLight<RedLight>>(Vec(30, 118), module, XGlide::EXCLUDE_INTERVALS_LIGHT));

	addInput(createInputCentered<InPort>(Vec(15, 174), module, XGlide::PITCH_INPUT));
	addOutput(createOutputCentered<OutPort>(Vec(45, 174), module, XGlide::PITCH_OUTPUT));
	
	addInput(createInputCentered<InPort>(Vec(15, 229), module, XGlide::MODULATION_INPUT));
	addOutput(createOutputCentered<OutPort>(Vec(45, 229), module, XGlide::MODULATION_OUTPUT));
	
	addInput(createInputCentered<InPort>(Vec(15, 282), module, XGlide::VOLUME_INPUT));
	addOutput(createOutputCentered<OutPort>(Vec(45, 282), module, XGlide::VOLUME_OUTPUT));

	addInput(createInputCentered<InPort>(Vec(15, 335), module, XGlide::PEDAL_INPUT));
}

Model* modelXGlide = createModel<XGlide, XGlideWidget>("XGlide");
