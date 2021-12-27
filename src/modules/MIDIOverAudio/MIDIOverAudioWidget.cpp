#include <fstream>
#include <osdialog.h>
#include "MIDIOverAudioWidget.hpp"

std::vector<MIDIOverAudioWidget*>* MIDIOverAudioWidget::widgets;

// MIDIOverAudioDriverMenuItem::MIDIOverAudioDriverMenuItem(MIDIOverAudio* midiOverAudio) {
	// this->midiOverAudio = midiOverAudio;
	// if (MIDIOverAudioDriver::driver->enabled) {
		// text = "disable MIDI over Audio driver";
	// } else {
		// text = "enable MIDI over Audio driver";
	// }
// }

// void MIDIOverAudioDriverMenuItem::onAction(const event::Action& e) {
	// if (midiOverAudio) {
		// if (!MIDIOverAudioDriver::driver->enabled) {
			// std::string text = "After enabling the MIDI over Audio driver, you will get an annoying error message every time you close the Rack. This will not harm your patches in any way. Furthermore it is possible to disable the MIDI over Audio driver at anytime you like.\n\nDo you want to enable the MIDI over Audio driver now?";
			// if (osdialog_message(OSDIALOG_INFO, OSDIALOG_OK_CANCEL, text.c_str())) {
				// saveMIDIOverAudioDriverEnabled(true);
				// new MIDIOverAudioDriver();
				// for (auto widget : *MIDIOverAudioWidget::widgets) {
					// widget->setPanel("res/MIDIOverAudio.svg");
				// }
			// }
		// } else {
			// std::string text = "You're about to disable the MIDI over Audio driver. For this to take effect, it is required to close and re-launch the Rack.\n\nDo you want to close an re-launch the Rack in oder to disable the MIDI over Audio driver now?";
			// if (osdialog_message(OSDIALOG_INFO, OSDIALOG_OK_CANCEL, text.c_str())) {
				// saveMIDIOverAudioDriverEnabled(false);
				// APP->window->close();
			// }
		// }
	// }
// }

MIDIOverAudioPanicMenuItem::MIDIOverAudioPanicMenuItem(MIDIOverAudio* midiOverAudio) {
	this->midiOverAudio = midiOverAudio;
	text = "Panic";
}

void MIDIOverAudioPanicMenuItem::onAction(const event::Action& e) {
	if (midiOverAudio) {
		midiOverAudio->panic();
	}
}

MIDIOverAudioWidget::MIDIOverAudioWidget(MIDIOverAudio* module) {
	init();
	setModule(module);
	// if (MIDIOverAudioDriver::driver->enabled) {
		// setPanel("res/MIDIOverAudio.svg");
	// } else {
		// setPanel("res/MIDIOverAudio_disabled.svg");
	// }
	setPanel("res/MIDIOverAudio.svg");
	setWidthInHP(2);
	setScrews(SCREW_TOP_LEFT, NO_SCREW_TOP_RIGHT, NO_SCREW_BOTTOM_LEFT, SCREW_BOTTOM_RIGHT);

	for (auto i = 0; i < MIDI_OVER_AUDIO_MAX_DEVICES; ++i) {
		addInput(createInputCentered<InPort>(Vec(15, 95 - 9 + i * 37), module, MIDIOverAudio::MIDI_OVER_AUDIO_INPUT + i));
		addParam(createParamCentered<KnobScrew>(Vec(8.3f, 77 - 9 + i * 37),   module, MIDIOverAudio::TRIM_PARAM + i));
		addChild(createLightCentered<SmallLight<RedGreenBlueLight>>(Vec(19, 77 - 9 + i * 37), module, MIDIOverAudio::RGB_LIGHT + i * 3));
	}
}

MIDIOverAudioWidget::~MIDIOverAudioWidget() {
	auto it = std::find(MIDIOverAudioWidget::widgets->begin(), MIDIOverAudioWidget::widgets->end(), this);
	if (it != MIDIOverAudioWidget::widgets->end()) {
		MIDIOverAudioWidget::widgets->erase(it);
	}
}

void MIDIOverAudioWidget::init() {
	if (MIDIOverAudioWidget::widgets == nullptr) {
		MIDIOverAudioWidget::widgets = new std::vector<MIDIOverAudioWidget*>;
	}
	MIDIOverAudioWidget::widgets->push_back(this);
}

void MIDIOverAudioWidget::contextMenu(Menu* menu) {
	MIDIOverAudio* midiOverAudio = dynamic_cast<MIDIOverAudio*>(this->module);
	// menu->addChild(new MIDIOverAudioDriverMenuItem(midiOverAudio));
	if (MIDIOverAudioDriver::driver->enabled) {
		menu->addChild(new MIDIOverAudioPanicMenuItem(midiOverAudio));
	}
}

Model* modelMIDIOverAudio = createModel<MIDIOverAudio, MIDIOverAudioWidget>("MIDIOverAudio");
