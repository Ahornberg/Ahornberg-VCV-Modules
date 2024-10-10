#include "MIDIPolyExpressionWidget.hpp"

MIDIPolyExpressionGateVelocityModeMenuItem::MIDIPolyExpressionGateVelocityModeMenuItem(MIDIPolyExpression* midiPolyExpression) {
	this->midiPolyExpression = midiPolyExpression;
	text = "Gate Velocity Mode";
	if (midiPolyExpression) {
		rightText = CHECKMARK(midiPolyExpression->params[MIDIPolyExpression::GATE_VELOCITY_MODE_PARAM].getValue());
	}
}

void MIDIPolyExpressionGateVelocityModeMenuItem::onAction(const event::Action& e) {
	if (midiPolyExpression) {
		if (midiPolyExpression->params[MIDIPolyExpression::GATE_VELOCITY_MODE_PARAM].getValue()) {
			midiPolyExpression->params[MIDIPolyExpression::GATE_VELOCITY_MODE_PARAM].setValue(0);
		} else {
			midiPolyExpression->params[MIDIPolyExpression::GATE_VELOCITY_MODE_PARAM].setValue(1);			
		}
	}	
}

MIDIPolyExpressionPreservePitchAfterNoteoffMenuItem::MIDIPolyExpressionPreservePitchAfterNoteoffMenuItem(MIDIPolyExpression* midiPolyExpression) {
	this->midiPolyExpression = midiPolyExpression;
	text = "Preserve Pitch after Note-Off";
	if (midiPolyExpression) {
		rightText = CHECKMARK(midiPolyExpression->params[MIDIPolyExpression::PRESERVE_PITCH_AFTER_NOTEOFF_PARAM].getValue());
	}
}

void MIDIPolyExpressionPreservePitchAfterNoteoffMenuItem::onAction(const event::Action& e) {
	if (midiPolyExpression) {
		if (midiPolyExpression->params[MIDIPolyExpression::PRESERVE_PITCH_AFTER_NOTEOFF_PARAM].getValue()) {
			midiPolyExpression->params[MIDIPolyExpression::PRESERVE_PITCH_AFTER_NOTEOFF_PARAM].setValue(0);
		} else {
			midiPolyExpression->params[MIDIPolyExpression::PRESERVE_PITCH_AFTER_NOTEOFF_PARAM].setValue(1);			
		}
	}	
}

MIDIPolyExpressionPanicMenuItem::MIDIPolyExpressionPanicMenuItem(MIDIPolyExpression* midiPolyExpression) {
	this->midiPolyExpression = midiPolyExpression;
	text = "Panic";
}

void MIDIPolyExpressionPanicMenuItem::onAction(const event::Action& e) {
	if (midiPolyExpression) {
		midiPolyExpression->onReset();
	}
}

MIDIPolyExpressionWidget::MIDIPolyExpressionWidget(MIDIPolyExpression* module) {
	setModule(module);
	setPanel("res/MIDIPolyExpression.svg", "res/MIDIPolyExpression-dark.svg");
	setWidthInHP(2);

	addParam(createParamCentered<KnobTinySnap>(Vec(15, 108), module, MIDIPolyExpression::MIDI_CHANNEL_FIRST_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(15, 132), module, MIDIPolyExpression::MIDI_CHANNEL_COUNT_PARAM));
		
	addParam(createParamCentered<KnobScrew>(Vec(8.3f, 155.25f), module, MIDIPolyExpression::VOLUME_SHAPE_PARAM));
	
	addOutput(createOutputCentered<OutPort>(Vec(15, 174), module, MIDIPolyExpression::GATE_OUTPUT));

	addParam(createParamCentered<KnobScrew>(Vec(8.3f, 192.75f), module, MIDIPolyExpression::PITCH_SHAPE_PARAM));
	
	addOutput(createOutputCentered<OutPort>(Vec(15, 211.5f), module, MIDIPolyExpression::PITCH_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15, 236.5f), module, MIDIPolyExpression::NOTE_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15, 261.5f), module, MIDIPolyExpression::PITCHBEND_OUTPUT));

	addOutput(createOutputCentered<OutPort>(Vec(15, 291), module, MIDIPolyExpression::MODULATION_OUTPUT));
	
	addParam(createParamCentered<KnobScrew>(Vec(8.3f, 310.0f), module, MIDIPolyExpression::DECAY_Y_PARAM));
	addParam(createParamCentered<KnobScrew>(Vec(30.f - 8.3f, 320.75f), module, MIDIPolyExpression::RELEASE_PARAM));
	addParam(createParamCentered<KnobScrew>(Vec(8.3f, 324.25f), module, MIDIPolyExpression::DECAY_PARAM));
	
	addOutput(createOutputCentered<OutPort>(Vec(15, 343), module, MIDIPolyExpression::VOLUME_OUTPUT));
	addChild(createLightCentered<SmallLight<GreenLight>>(Vec(5, 355), module, MIDIPolyExpression::VOLUME_14_BIT_LIGHT));
	
	MidiButtonSmall* midiButton = createWidgetCentered<MidiButtonSmall>(Vec(15, 62));
	midiButton->setMidiPort(module ? &module->midiInput : NULL);
	addChild(midiButton);
}

void MIDIPolyExpressionWidget::contextMenu(Menu* menu) {
	MIDIPolyExpression* midiPolyExpression = dynamic_cast<MIDIPolyExpression*>(this->module);
	menu->addChild(new MIDIPolyExpressionGateVelocityModeMenuItem(midiPolyExpression));
	menu->addChild(new MIDIPolyExpressionPreservePitchAfterNoteoffMenuItem(midiPolyExpression));
	menu->addChild(new MIDIPolyExpressionPanicMenuItem(midiPolyExpression));
}

Model* modelMIDIPolyExpression = createModel<MIDIPolyExpression, MIDIPolyExpressionWidget>("MIDIPolyExpression");
