struct Display : SizedTransparentWidget, ManualText {
	std::string fontPath;
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
