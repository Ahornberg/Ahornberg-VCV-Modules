#include "MIDIControllerWidget.hpp"

MIDIControllerPanicMenuItem::MIDIControllerPanicMenuItem(MIDIController* midiController) {
	this->midiController = midiController;
	text = "Panic";
}

void MIDIControllerPanicMenuItem::onAction(const event::Action& e) {
	if (midiController) {
		midiController->onReset();
	}
}

MIDIControllerWidget::MIDIControllerWidget(MIDIController* module) {
	setModule(module);
	setPanel("res/MIDIController.svg", "res/MIDIController-dark.svg");
	setWidthInHP(4);

	addParam(createParamCentered<KnobTiny>(Vec(15, 108), module, MIDIController::MOVEMENT_ADJUST_PARAM));
	
	addParam(createParamCentered<KnobTiny>(Vec(45, 108), module, MIDIController::TEST_PARAM));
	
	addInput(createInputCentered<InPort>(Vec(45, 234), module, MIDIController::TEST_INPUT));

	addOutput(createOutputCentered<OutPort>(Vec(45, 271.5f), module, MIDIController::TEST_OUTPUT));

	MidiButtonSmall* midiInButton = createWidgetCentered<MidiButtonSmall>(Vec(15, 62));
	midiInButton->setMidiPort(module ? &module->midiInput : NULL);
	addChild(midiInButton);
	
	MidiButtonSmall* midiOutButton = createWidgetCentered<MidiButtonSmall>(Vec(45, 62));
	midiOutButton->setMidiPort(module ? &module->midiOutput : NULL);
	addChild(midiOutButton);
}

void MIDIControllerWidget::contextMenu(Menu* menu) {
	MIDIController* midiController = dynamic_cast<MIDIController*>(this->module);
	menu->addChild(new MIDIControllerPanicMenuItem(midiController));
}

Model* modelMIDIController = createModel<MIDIController, MIDIControllerWidget>("MIDIController");
