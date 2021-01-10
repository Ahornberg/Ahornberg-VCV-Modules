#include "MIDIOverAudio.hpp"

struct MIDIOverAudioWidget : ModuleWidgetWithScrews {
	static std::vector<MIDIOverAudioWidget*>* widgets;
	
	MIDIOverAudioWidget(MIDIOverAudio* module);
	~MIDIOverAudioWidget();
	void init();
	void appendContextMenu(Menu* menu) override;
};

struct MIDIOverAudioDriverMenuItem : MenuItem {
	MIDIOverAudio* midiOverAudio;
	
	MIDIOverAudioDriverMenuItem(MIDIOverAudio* midiOverAudio);
	void onAction(const event::Action& e) override;
};
