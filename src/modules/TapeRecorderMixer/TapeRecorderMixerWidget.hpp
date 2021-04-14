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
};

