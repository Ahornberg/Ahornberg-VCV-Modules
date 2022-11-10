static const char SELECTION_FILTERS[] = "VCV Rack module selection (.vcvs):vcvs";

struct SizedTransparentWidget : TransparentWidget {
	SizedTransparentWidget(Rect box);
};

struct TextFieldMenuItem : TextField {
	TextFieldMenuItem();
	void onAction(const event::Action& e) override;
};

struct BaseModuleWidget : ModuleWidget {
	void setPanel(const std::string& filename);
	void setWidthInHP(int hp);
	bool isBypassed();
	void appendContextMenu(Menu* menu) override;
	virtual void contextMenu(Menu* menu);
	void loadSelectionDialog();
	void loadSelection(std::string path);
	void onButton(const ButtonEvent& e) override;
	virtual void createCustomContextMenu();
};
