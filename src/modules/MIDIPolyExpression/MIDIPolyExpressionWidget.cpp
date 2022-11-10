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

	addParam(createParamCentered<KnobTinySnap>(Vec(15, 108), module, MIDIPolyExpression::MIDI_CHANNEL_FIRST_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(15, 132), module, MIDIPolyExpression::MIDI_CHANNEL_COUNT_PARAM));
		
	addParam(createParamCentered<KnobScrew>(Vec(8.3f, 215.25f), module, MIDIPolyExpression::VOLUME_SHAPE_PARAM));
	
	addOutput(createOutputCentered<OutPort>(Vec(15, 234), module, MIDIPolyExpression::GATE_OUTPUT));

	addParam(createParamCentered<KnobScrew>(Vec(8.3f, 252.75f), module, MIDIPolyExpression::PITCH_SHAPE_PARAM));
	
	addOutput(createOutputCentered<OutPort>(Vec(15, 271.5f), module, MIDIPolyExpression::PITCH_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15, 302), module, MIDIPolyExpression::MODULATION_OUTPUT));
	
	addParam(createParamCentered<KnobScrew>(Vec(30.f - 8.3f, 320.75f), module, MIDIPolyExpression::DECAY_PARAM));
	addParam(createParamCentered<KnobScrew>(Vec(8.3f, 324.25f), module, MIDIPolyExpression::RELEASE_PARAM));
	
	addOutput(createOutputCentered<OutPort>(Vec(15, 343), module, MIDIPolyExpression::VOLUME_OUTPUT));
	
	MidiButton* midiButton = createWidgetCentered<MidiButtonSmall>(Vec(15, 62));
	midiButton->setMidiPort(module ? &module->midiInput : NULL);
	addChild(midiButton);
}

void MIDIPolyExpressionWidget::contextMenu(Menu* menu) {
	MIDIPolyExpression* midipolyExpression = dynamic_cast<MIDIPolyExpression*>(this->module);
	menu->addChild(new MIDIPolyExpressionGateVelocityModeMenuItem(midipolyExpression));
	menu->addChild(new MIDIPolyExpressionPanicMenuItem(midipolyExpression));
}

Model* modelMIDIPolyExpression = createModel<MIDIPolyExpression, MIDIPolyExpressionWidget>("MIDIPolyExpression");
