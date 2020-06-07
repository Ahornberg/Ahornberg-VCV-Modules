struct SizedTransparentWidget : TransparentWidget {
	SizedTransparentWidget(Rect box) {
		this->box = box;
	}
};

struct TextFieldMenuItem : TextField {
	
	TextFieldMenuItem() {
		box.size = Vec(120, 20);
		multiline = false;
	}
	
	void onAction(const event::Action& e) override {
		TextField::onAction(e);
		getAncestorOfType<MenuOverlay>()->requestDelete();
	}
};

struct ModuleWidgetWithScrews : ModuleWidget {
	void setPanel(const std::string& filename) {
		ModuleWidget::setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
	}
	
	void setScrews(bool topLeft, bool topRight, bool bottomLeft, bool bottomRight) {
		if (topLeft) {
			addChild(createWidget<ScrewSilver>(Vec(0, 0)));
		}
		if (topRight) {
			addChild(createWidget<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, 0)));
		}
		if (bottomLeft) {
			addChild(createWidget<ScrewSilver>(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		}
		if (bottomRight) {
			addChild(createWidget<ScrewSilver>(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		}
	}
};