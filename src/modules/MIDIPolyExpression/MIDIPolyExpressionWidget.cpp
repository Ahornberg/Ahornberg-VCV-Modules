#include "MIDIPolyExpressionWidget.hpp"

MIDIPolyExpressionGateVelocityModeMenuItem::MIDIPolyExpressionGateVelocityModeMenuItem(MIDIPolyExpression* midipolyExpression) {
	this->midipolyExpression = midipolyExpression;
	text = "Gate Velocity Mode";
	if (midipolyExpression) {
		rightText = CHECKMARK(midipolyExpression->params[MIDIPolyExpression::GATE_VELOCITY_MODE_PARAM].getValue());
	}
}

void MIDIPolyExpressionGateVelocityModeMenuItem::onAction(const event::Action& e) {
	if (midipolyExpression) {
		if (midipolyExpression->params[MIDIPolyExpression::GATE_VELOCITY_MODE_PARAM].getValue()) {
			midipolyExpression->params[MIDIPolyExpression::GATE_VELOCITY_MODE_PARAM].setValue(0);
		} else {
			midipolyExpression->params[MIDIPolyExpression::GATE_VELOCITY_MODE_PARAM].setValue(1);			
		}
	}	
}

MIDIPolyExpressionPanicMenuItem::MIDIPolyExpressionPanicMenuItem(MIDIPolyExpression* midipolyExpression) {
	this->midipolyExpression = midipolyExpression;
	text = "Panic";
}

void MIDIPolyExpressionPanicMenuItem::onAction(const event::Action& e) {
	if (midipolyExpression) {
		midipolyExpression->onReset();
	}
}

MIDIPolyExpressionWidget::MIDIPolyExpressionWidget(MIDIPolyExpression* module) {
	setModule(module);
	setPanel("res/MIDIPolyExpression.svg");
	setWidthInHP(2);
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, NO_SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);

	addParam(createParamCentered<KnobTinySnap>(Vec(15, 110), module, MIDIPolyExpression::MIDI_CHANNEL_FIRST_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(15, 135), module, MIDIPolyExpression::MIDI_CHANNEL_COUNT_PARAM));
		
	addOutput(createOutputCentered<OutPort>(Vec(15, 234), module, MIDIPolyExpression::GATE_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15, 268), module, MIDIPolyExpression::PITCH_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15, 302), module, MIDIPolyExpression::MODULATION_OUTPUT));
	
	addParam(createParamCentered<KnobScrew>(Vec(30.f - 8.3f, 320.75f), module, MIDIPolyExpression::DECAY_PARAM));
	
	addParam(createParamCentered<KnobScrew>(Vec(8.3f, 324.25f), module, MIDIPolyExpression::RELEASE_PARAM));
	
	addOutput(createOutputCentered<OutPort>(Vec(15, 343), module, MIDIPolyExpression::VOLUME_OUTPUT));
	
	MidiButton* midiButton = createWidgetCentered<MidiButtonSmall>(Vec(15, 62));
	midiButton->setMidiPort(module ? &module->midiInput : NULL);
	addChild(midiButton);
}

void MIDIPolyExpressionWidget::appendContextMenu(Menu* menu) {
	MIDIPolyExpression* midipolyExpression = dynamic_cast<MIDIPolyExpression*>(this->module);
	menu->addChild(new MenuSeparator);
	menu->addChild(new MIDIPolyExpressionGateVelocityModeMenuItem(midipolyExpression));
	menu->addChild(new MIDIPolyExpressionPanicMenuItem(midipolyExpression));
}

Model* modelMIDIPolyExpression = createModel<MIDIPolyExpression, MIDIPolyExpressionWidget>("MIDIPolyExpression");
