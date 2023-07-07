#include "TapeInspector.hpp"

struct TapeAudioDisplay : Display {
	const static NVGcolor TAPE_TEXT_COLORS[];
	constexpr static int DISPLAY_WIDTH = 52;
	constexpr static int DISPLAY_HEIGHT = 291;//219;
	
	TapeInspector *tapeInspector;

	TapeAudioDisplay(Rect box, TapeInspector *tapeInspector);
	void drawText(const DrawArgs& disp) override;
};

struct TapeInspectorWidget : BaseModuleWidget {
	TapeInspectorWidget(TapeInspector* module);
};
