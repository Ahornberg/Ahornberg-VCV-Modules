#include "MIDIPolyExpression.hpp"

struct MIDIPolyExpressionWidget : BaseModuleWidget {
	MIDIPolyExpressionWidget(MIDIPolyExpression* module);
	void contextMenu(Menu* menu) override;
};

struct MIDIPolyExpressionGateVelocityModeMenuItem : MenuItem {
	MIDIPolyExpression* midiPolyExpression;
	
	MIDIPolyExpressionGateVelocityModeMenuItem(MIDIPolyExpression* midiPolyExpression);
	void onAction(const event::Action& e) override;
};

struct MIDIPolyExpressionPanicMenuItem : MenuItem {
	MIDIPolyExpression* midiPolyExpression;
	
	MIDIPolyExpressionPanicMenuItem(MIDIPolyExpression* midiPolyExpression);
	void onAction(const event::Action& e) override;
};
