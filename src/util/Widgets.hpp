static const char PRESET_FILTERS[] = "VCV Rack module preset (.vcvm):vcvm";
static const char SELECTION_FILTERS[] = "VCV Rack module selection (.vcvs):vcvs";

struct ThemedSvgWidget : SvgWidget {
	std::shared_ptr<window::Svg> svgDark;
	
	void setSvg(std::shared_ptr<window::Svg> svg, std::shared_ptr<window::Svg> svgDark);
	void draw(const DrawArgs& args) override;
};

struct SizedTransparentWidget : TransparentWidget {
	SizedTransparentWidget(Rect box);
};

struct TextFieldMenuItem : TextField {
	TextFieldMenuItem();
	void onAction(const event::Action& e) override;
};

struct BaseModuleWidget : ModuleWidget {
	void setPanel(const std::string& filename, const std::string& filenameDark);
	void setWidthInHP(int hp);
	bool isBypassed();
	void appendContextMenu(Menu* menu) override;
	virtual void contextMenu(Menu* menu);
	void loadSelectionDialog();
	void loadSelection(std::string path);
	void onButton(const ButtonEvent& e) override;
	virtual void createCustomContextMenu();
	void loadDialogCustom();
	virtual void loadActionCustom(std::string filename);
	virtual void saveDialogCustom();
	virtual void saveTemplateCustom();
	void saveTemplateDialogCustom();
};
