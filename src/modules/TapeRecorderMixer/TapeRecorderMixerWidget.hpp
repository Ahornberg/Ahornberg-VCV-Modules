#include "TapeRecorderMixer.hpp"

struct VolumeDisplay : Display {
	TapeRecorderMixer* tapeRecorderMixer;
	// double tapePosition;
	// int beatsPerBar;
	// int loopMode;
	// int loopStart;
	// int loopEnd;
	// bool loopStartConnected;
	// bool loopEndConnected;
	// bool loopStartOnTapePosition;
	// bool loopEndOnTapePosition;
	// bool playStatus;
	// bool cueStatus;
	// bool playForwardStatus;
	// bool cueForwardStatus;

	VolumeDisplay(Rect box, TapeRecorderMixer* tapeRecorderMixer);
	void drawText(const DrawArgs& disp) override;
};

struct TapeRecorderMixerWidget : ModuleWidgetWithScrews {
	TapeRecorderMixerWidget(TapeRecorderMixer* module);
	void appendContextMenu(Menu* menu) override;
	void step() override;
};

struct TapeRecorderMixerMenuItem : MenuItem {
	TapeRecorderMixer* tapeRecorderMixer;
	TapeRecorderMixerWidget* tapeRecorderMixerWidget;
	
	TapeRecorderMixerMenuItem(TapeRecorderMixer* tapeRecorderMixer, TapeRecorderMixerWidget* tapeRecorderMixerWidget);
};

struct ChangeInputMuteModeMenuItem : TapeRecorderMixerMenuItem {
	ChangeInputMuteModeMenuItem(TapeRecorderMixer* tapeRecorderMixer, TapeRecorderMixerWidget* tapeRecorderMixerWidget);
	void onAction(const event::Action& e) override;
};

