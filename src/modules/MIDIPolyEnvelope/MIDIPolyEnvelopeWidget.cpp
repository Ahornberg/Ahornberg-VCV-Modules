#include "MIDIPolyEnvelopeWidget.hpp"


MIDIPolyEnvelopePanicMenuItem::MIDIPolyEnvelopePanicMenuItem(MIDIPolyEnvelope* midipolyEnvelope) {
	this->midipolyEnvelope = midipolyEnvelope;
	text = "Panic";
}

void MIDIPolyEnvelopePanicMenuItem::onAction(const event::Action& e) {
	if (midipolyEnvelope) {
		midipolyEnvelope->onReset();
	}
}

MIDIPolyEnvelopeWidget::MIDIPolyEnvelopeWidget(MIDIPolyEnvelope* module) {
	setModule(module);
	setPanel("res/MIDIPolyEnvelope.svg");
	setWidthInHP(4);
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, NO_SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);

	addParam(createParamCentered<KnobTinySnap>(Vec(15, 92), module, MIDIPolyEnvelope::MIDI_CHANNEL_FIRST_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(15, 117), module, MIDIPolyEnvelope::MIDI_CHANNEL_COUNT_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(45, 235), module, MIDIPolyEnvelope::VOLUME_ATTACK_THRESHOLD_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(45, 270), module, MIDIPolyEnvelope::VOLUME_DECAY_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(45, 305), module, MIDIPolyEnvelope::VOLUME_MIX_PARAM));
	addParam(createParamCentered<KnobTinySnap>(Vec(45, 340), module, MIDIPolyEnvelope::VOLUME_SMOOTH_PARAM));
		
	addOutput(createOutputCentered<OutPort>(Vec(15, 235), module, MIDIPolyEnvelope::GATE_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15, 270), module, MIDIPolyEnvelope::PITCH_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15, 305), module, MIDIPolyEnvelope::MODULATION_OUTPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15, 340), module, MIDIPolyEnvelope::VOLUME_OUTPUT));
	
	MidiButton* midiButton = createWidget<MidiButtonSmall>(Vec(30, 20));
	midiButton->setMidiPort(module ? &module->midiInput : NULL);
	addChild(midiButton);
}

void MIDIPolyEnvelopeWidget::appendContextMenu(Menu* menu) {
	MIDIPolyEnvelope* midipolyEnvelope = dynamic_cast<MIDIPolyEnvelope*>(this->module);
	menu->addChild(new MenuSeparator);
	// menu->addChild(new MIDIPolyEnvelopeDriverMenuItem(midipolyEnvelope));
	menu->addChild(new MIDIPolyEnvelopePanicMenuItem(midipolyEnvelope));
}

Model* modelMIDIPolyEnvelope = createModel<MIDIPolyEnvelope, MIDIPolyEnvelopeWidget>("MIDIPolyEnvelope");
