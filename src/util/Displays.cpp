#include "../Ahornberg.hpp"

Display::Display(Rect box) : SizedTransparentWidget(box) {
	fontPath = asset::plugin(pluginInstance, FONT_SEGMENT_14);
	backgroundColor = COLOR_BLACK;
	borderColor = COLOR_GREY;
	textColorLight = COLOR_YELLOW;
	textColorDark = COLOR_GREEN_DARK;
	textColorRed = COLOR_RED;
}

void Display::draw(const DrawArgs& disp) {
	std::shared_ptr<Font> font = APP->window->loadFont(fontPath);
	if (font && font->handle >= 0) {
		nvgBeginPath(disp.vg);
		nvgRoundedRect(disp.vg, 0.0, 0.0, box.size.x, box.size.y, 3.0);
		nvgFillColor(disp.vg, backgroundColor);
		nvgFill(disp.vg);
		nvgStrokeWidth(disp.vg, 1.0);
		nvgStrokeColor(disp.vg, borderColor);
		nvgStroke(disp.vg);
	}
}

void Display::drawLayer(const DrawArgs& disp, int layer) {
	if (layer == 1) {
		std::shared_ptr<Font> font = APP->window->loadFont(fontPath);
		if (font && font->handle >= 0) {
			// init font
			nvgFontSize(disp.vg, 10);
			nvgFontFaceId(disp.vg, font->handle);
			nvgTextLetterSpacing(disp.vg, 1);
			drawText(disp);
		}
	}
	Widget::drawLayer(disp, layer);
}

void Display::drawText(const DrawArgs& disp) {}
