struct Display : SizedTransparentWidget, ManualText {
	std::shared_ptr<Font> font;
	NVGcolor backgroundColor;
	NVGcolor borderColor;
	NVGcolor textColorLight;
	NVGcolor textColorDark;
	NVGcolor textColorRed;

	Display(Rect box);
	void drawBackground(const DrawArgs& disp);
	virtual void drawText(const DrawArgs& disp);
	void draw(const DrawArgs& disp) override;
};
