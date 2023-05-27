struct WindowManager;

struct TopSideBarContainer : OpaqueWidget {
	WindowManager* windowManager;
	
	void draw(const DrawArgs& disp) override {
		nvgBeginPath(disp.vg);
		nvgRect(disp.vg, 0, 0, box.size.x, box.size.y);
		nvgFillColor(disp.vg, nvgTransRGBA(COLOR_BLACK, 0xC0));
		nvgFill(disp.vg);
	}

};

struct TopBarContainer : TopSideBarContainer {
	
	void step() override;
};

struct SideBarContainer : TopSideBarContainer {
	
	void step() override;
};

struct WindowManager : TransparentWidget {
	TopBarContainer* top;
	SideBarContainer* side;
	
	static WindowManager* getInstance();
	void step() override;
};


void TopBarContainer::step() {
	box = Rect(windowManager->side->box.size.x, 0, windowManager->box.size.x - windowManager->side->box.size.x, std::min(windowManager->box.size.y, 100.0f));
	TopSideBarContainer::step();
}

void SideBarContainer::step() {
	box = Rect(0, 0, std::min(windowManager->box.size.x, 400.0f), windowManager->box.size.y);
	TopSideBarContainer::step();
}

WindowManager* WindowManager::getInstance() {
	WindowManager* instance = APP->scene->getFirstDescendantOfType<WindowManager>();
	if (!instance) {
		instance = new WindowManager;
		instance->side = new SideBarContainer();
		instance->side->windowManager = instance;
		instance->addChild(instance->side);
		instance->top = new TopBarContainer();
		instance->top->windowManager = instance;
		instance->addChild(instance->top);
		APP->scene->addChildBelow(instance, APP->scene->menuBar);
	}
	return instance;
}
	
void WindowManager::step() {
	box = Rect(0, 31, parent->box.size.x - 13, parent->box.size.y - 31 - 13);
	TransparentWidget::step();
}
