#include "MIDIPolyExpression.hpp"

struct MIDIPolyExpressionWidget : ModuleWidgetWithScrews {
	MIDIPolyExpressionWidget(MIDIPolyExpression* module);
	void contextMenu(Menu* menu) override;
};

struct MIDIPolyExpressionGateVelocityModeMenuItem : MenuItem {
	MIDIPolyExpression* midipolyExpression;
	
	MIDIPolyExpressionGateVelocityModeMenuItem(MIDIPolyExpression* midipolyExpression);
	void onAction(const event::Action& e) override;
};

struct MIDIPolyExpressionPanicMenuItem : MenuItem {
	MIDIPolyExpression* midipolyExpression;
	
	MIDIPolyExpressionPanicMenuItem(MIDIPolyExpression* midipolyExpression);
	void onAction(const event::Action& e) override;
};
