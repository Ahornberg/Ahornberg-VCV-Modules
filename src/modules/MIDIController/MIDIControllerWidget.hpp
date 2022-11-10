#include "MIDIController.hpp"
#include "../../include/ExternalWindow.h"

struct MIDIControllerWidget : BaseModuleWidget {
	ExternalWindow* externalWindow;
	
	MIDIControllerWidget(MIDIController* module);
	void contextMenu(Menu* menu) override;
};

struct MIDIControllerPanicMenuItem : MenuItem {
	MIDIController* midiController;
	
	MIDIControllerPanicMenuItem(MIDIController* midiController);
	void onAction(const event::Action& e) override;
};
