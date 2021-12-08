#include "MIDIPolyEnvelope.hpp"

struct MIDIPolyEnvelopeWidget : ModuleWidgetWithScrews {
	MIDIPolyEnvelopeWidget(MIDIPolyEnvelope* module);
	void appendContextMenu(Menu* menu) override;
};

// struct MIDIPolyEnvelopeDriverMenuItem : MenuItem {
	// MIDIPolyEnvelope* midipolyEnvelope;
	
	// MIDIPolyEnvelopeDriverMenuItem(MIDIPolyEnvelope* midipolyEnvelope);
	// void onAction(const event::Action& e) override;
// };

struct MIDIPolyEnvelopePanicMenuItem : MenuItem {
	MIDIPolyEnvelope* midipolyEnvelope;
	
	MIDIPolyEnvelopePanicMenuItem(MIDIPolyEnvelope* midipolyEnvelope);
	void onAction(const event::Action& e) override;
};
