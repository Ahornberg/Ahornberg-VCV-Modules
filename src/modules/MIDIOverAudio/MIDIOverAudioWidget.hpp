#include "MIDIOverAudio.hpp"

struct MIDIOverAudioWidget : BaseModuleWidget {
	static std::vector<MIDIOverAudioWidget*>* widgets;
	
	MIDIOverAudioWidget(MIDIOverAudio* module);
	~MIDIOverAudioWidget();
	void init();
	void contextMenu(Menu* menu) override;
};

// struct MIDIOverAudioDriverMenuItem : MenuItem {
	// MIDIOverAudio* midiOverAudio;
	
	// MIDIOverAudioDriverMenuItem(MIDIOverAudio* midiOverAudio);
	// void onAction(const event::Action& e) override;
// };

struct MIDIOverAudioPanicMenuItem : MenuItem {
	MIDIOverAudio* midiOverAudio;
	
	MIDIOverAudioPanicMenuItem(MIDIOverAudio* midiOverAudio);
	void onAction(const event::Action& e) override;
};
