#include "MIDIOverAudioWidget.hpp"

MIDIOverAudioWidget::MIDIOverAudioWidget(MIDIOverAudio* module) {
	setModule(module);
	setPanel("res/MIDIOverAudio.svg");
	setSize(Vec(30, 380));
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, NO_SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);

	addInput(createInputCentered<InPort>(Vec(15,  95), module, MIDIOverAudio::MIDI_OVER_AUDIO_INPUT + 0));
	addInput(createInputCentered<InPort>(Vec(15, 130), module, MIDIOverAudio::MIDI_OVER_AUDIO_INPUT + 1));
	addInput(createInputCentered<InPort>(Vec(15, 165), module, MIDIOverAudio::MIDI_OVER_AUDIO_INPUT + 2));
	addInput(createInputCentered<InPort>(Vec(15, 200), module, MIDIOverAudio::MIDI_OVER_AUDIO_INPUT + 3));
	addInput(createInputCentered<InPort>(Vec(15, 235), module, MIDIOverAudio::MIDI_OVER_AUDIO_INPUT + 4));
	addInput(createInputCentered<InPort>(Vec(15, 270), module, MIDIOverAudio::MIDI_OVER_AUDIO_INPUT + 5));
	addInput(createInputCentered<InPort>(Vec(15, 305), module, MIDIOverAudio::MIDI_OVER_AUDIO_INPUT + 6));
	addInput(createInputCentered<InPort>(Vec(15, 340), module, MIDIOverAudio::MIDI_OVER_AUDIO_INPUT + 7));
}

Model* modelMIDIOverAudio = createModel<MIDIOverAudio, MIDIOverAudioWidget>("MIDIOverAudio");
