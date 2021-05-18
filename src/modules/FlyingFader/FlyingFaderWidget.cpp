#include "FlyingFaderWidget.hpp"

MotorizedFader::MotorizedFader() {
	minHandlePos = Vec(0, 230);
	maxHandlePos = Vec(0, 2);
	setBackgroundSvg("res/knobs/MotorizedFaderBackground.svg");
	setHandleSvg("res/knobs/MotorizedFaderHandle.svg");
}

void MotorizedFader::onDragStart(const event::DragStart& e) {
	if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
		Knob::onDragStart(e);
		if (flyingFader) {
			flyingFader->faderDragged = true;
		}
	}
}

void MotorizedFader::onDragEnd(const event::DragEnd& e) {
	if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
		if (flyingFader) {
			flyingFader->faderDragged = false;
		}
		Knob::onDragEnd(e);
	}
}

FlyingFaderWidget::FlyingFaderWidget(FlyingFader* module) {
	setModule(module);
	setPanel("res/FlyingFader.svg");
	setSize(Vec(60, 380));

	fader = dynamic_cast<MotorizedFader*>(createParam<MotorizedFader>(Vec(18, 50), module, FlyingFader::FADER_PARAM));
	fader->flyingFader = module;
	addParam(fader);

	addInput(createInputCentered<InPort>(Vec(15, 350), module, FlyingFader::AUDIO_INPUT));
	addInput(createInputCentered<InPort>(Vec(45, 350), module, FlyingFader::CV_INPUT));
	
	addOutput(createOutputCentered<OutPort>(Vec(15, 30), module, FlyingFader::AUDIO_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(45, 30), module, FlyingFader::CV_OUTPUT));
}

Model* modelFlyingFader = createModel<FlyingFader, FlyingFaderWidget>("FlyingFader");
