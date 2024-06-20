#include "XGlideWidget.hpp"

XGlideWidget::XGlideWidget(XGlide* module) {
	setModule(module);
	setPanel("res/XGlide.svg");
	setWidthInHP(4);

	addParam(createParamCentered<KnobTinySnap>(Vec(15, 85), module, XGlide::MIN_NOTE_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(45, 85), module, XGlide::MAX_NOTE_PARAM));
	addChild(createLightCentered<SmallLight<RedLight>>(Vec(30, 85), module, XGlide::EXCLUDE_NOTES_LIGHT));
	
	addParam(createParamCentered<KnobTinySnap>(Vec(15, 132), module, XGlide::MIN_INTERVAL_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(45, 132), module, XGlide::MAX_INTERVAL_PARAM));
	addChild(createLightCentered<SmallLight<RedLight>>(Vec(30, 132), module, XGlide::EXCLUDE_INTERVALS_LIGHT));

	addInput(createInputCentered<InPort>(Vec(15, 211.5f), module, XGlide::PITCH_INPUT));
	addOutput(createOutputCentered<OutPort>(Vec(45, 211.5f), module, XGlide::PITCH_OUTPUT));
	
	addInput(createInputCentered<InPort>(Vec(15, 261.5f), module, XGlide::VOLUME_INPUT));
	addOutput(createOutputCentered<OutPort>(Vec(45, 261.5f), module, XGlide::VOLUME_OUTPUT));

	addInput(createInputCentered<InPort>(Vec(15, 311.5f), module, XGlide::PEDAL_INPUT));
}

Model* modelXGlide = createModel<XGlide, XGlideWidget>("XGlide");
