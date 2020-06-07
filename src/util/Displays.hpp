struct Display : SizedTransparentWidget {
	std::shared_ptr<Font> font;
	NVGcolor backgroundColor;
	NVGcolor borderColor;
	NVGcolor textColorLight;
	NVGcolor textColorDark;
	NVGcolor textColorRed;

	Display(Rect box) : SizedTransparentWidget(box) {
		font = APP->window->loadFont(asset::plugin(pluginInstance, FONT_SEGMENT_14));
		backgroundColor = COLOR_BLACK;
		borderColor = COLOR_GREY;
		textColorLight = COLOR_YELLOW;
		textColorDark = COLOR_GREEN_DARK;
		textColorRed = COLOR_RED;
	}

	void drawBackground(const DrawArgs &disp) {
		nvgBeginPath(disp.vg);
		nvgRoundedRect(disp.vg, 0.0, 0.0, box.size.x, box.size.y, 3.0);
		nvgFillColor(disp.vg, backgroundColor);
		nvgFill(disp.vg);
		nvgStrokeWidth(disp.vg, 1.0);
		nvgStrokeColor(disp.vg, borderColor);
		nvgStroke(disp.vg);
		// init font
		nvgFontSize(disp.vg, 10);
		nvgFontFaceId(disp.vg, font->handle);
		nvgTextLetterSpacing(disp.vg, 1);
	}

	virtual void drawText(const DrawArgs &disp) {}

	void draw(const DrawArgs &disp) override {
		drawBackground(disp);
		drawText(disp);
	}
};
