struct Display : SizedTransparentWidget, ManualText {
	std::string fontPath;
	NVGcolor backgroundColor;
	NVGcolor borderColor;
	NVGcolor textColorLight;
	NVGcolor textColorDark;
	NVGcolor textColorRed;

	Display(Rect box);
	virtual void drawText(const DrawArgs& disp);
	void draw(const DrawArgs& disp) override;
	void drawLayer(const DrawArgs& disp, int layer) override;
};
