#include "../Ahornberg.hpp"

SizedTransparentWidget::SizedTransparentWidget(Rect box) {
	this->box = box;
}

TextFieldMenuItem::TextFieldMenuItem() {
	box.size = Vec(120, 20);
	multiline = false;
}
	
void TextFieldMenuItem::onAction(const event::Action& e) {
	TextField::onAction(e);
	getAncestorOfType<MenuOverlay>()->requestDelete();
}

struct ModuleWidget::Internal {
	/** The module position clicked on to start dragging in the rack.
	*/
	math::Vec dragOffset;

	/** Global rack position the user clicked on.
	*/
	math::Vec dragRackPos;
	bool dragEnabled = true;

	/** The position in the RackWidget when dragging began.
	Used for history::ModuleMove.
	Set by RackWidget::updateModuleOldPositions() when *any* module begins dragging, since force-dragging can move other modules around.
	*/
	math::Vec oldPos;

	widget::Widget* panel = NULL;
};

void BaseModuleWidget::setPanel(const std::string& filename) {
	ModuleWidget::setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}

void BaseModuleWidget::setWidthInHP(int hp) {
	setSize(Vec(hp * 15, 380));
}

bool BaseModuleWidget::isBypassed() {
	if (module) {
		if (module->isBypassed()) {
			return true;
		}
		return false;
	}
	return false;
}

// Create ModulePresetPathItems for each patch in a directory.
static void appendPresetItems(ui::Menu* menu, WeakPtr<ModuleWidget> moduleWidget, std::string presetDir) {
	bool hasPresets = false;
	// Note: This is not cached, so opening this menu each time might have a bit of latency.
	if (system::isDirectory(presetDir)) {
		std::vector<std::string> entries = system::getEntries(presetDir);
		std::sort(entries.begin(), entries.end());
		for (std::string path : entries) {
			std::string name = system::getStem(path);
			// Remove "1_", "42_", "001_", etc at the beginning of preset filenames
			std::regex r("^\\d+_");
			name = std::regex_replace(name, r, "");

			if (system::isDirectory(path)) {
				hasPresets = true;

				menu->addChild(createSubmenuItem(name, "", [=](ui::Menu* menu) {
					if (!moduleWidget)
						return;
					appendPresetItems(menu, moduleWidget, path);
				}));
			}
			else if (system::getExtension(path) == ".vcvm") {
				hasPresets = true;

				menu->addChild(createMenuItem(name, "", [=]() {
					if (!moduleWidget)
						return;
					try {
						moduleWidget->loadAction(path);
					}
					catch (Exception& e) {
						osdialog_message(OSDIALOG_WARNING, OSDIALOG_OK, e.what());
					}
				}));
			}
		}
	}
	if (!hasPresets) {
		menu->addChild(createMenuLabel("(None)"));
	}
};

static void appendSelectionItems(ui::Menu* menu, WeakPtr<ModuleWidget> moduleWidget, std::string presetDir) {
	bool hasPresets = false;
	// Note: This is not cached, so opening this menu each time might have a bit of latency.
	if (system::isDirectory(presetDir)) {
		std::vector<std::string> entries = system::getEntries(presetDir);
		std::sort(entries.begin(), entries.end());
		for (std::string path : entries) {
			std::string name = system::getStem(path);
			// Remove "1_", "42_", "001_", etc at the beginning of preset filenames
			std::regex r("^\\d+_");
			name = std::regex_replace(name, r, "");

			if (system::isDirectory(path)) {
				hasPresets = true;

				menu->addChild(createSubmenuItem(name, "", [=](ui::Menu* menu) {
					if (!moduleWidget) {
						return;
					}
					appendSelectionItems(menu, moduleWidget, path);
				}));
			}
			else if (system::getExtension(path) == ".vcvs") {
				hasPresets = true;

				menu->addChild(createMenuItem(name, "", [=]() {
					if (!moduleWidget) {
						return;
					}
					try {
						FILE* file = std::fopen(path.c_str(), "r");
						if (!file) {
							throw Exception("Could not load selection file %s", path.c_str());
						}
						DEFER({std::fclose(file);});

						INFO("Loading selection %s", path.c_str());

						json_error_t error;
						json_t* rootJ = json_loadf(file, 0, &error);
						if (!rootJ) {
							throw Exception("File is not a valid selection file. JSON parsing error at %s %d:%d %s", error.source, error.line, error.column, error.text);
						}
						DEFER({json_decref(rootJ);});
						
						// iterate modules and recalc position
						json_t* modulesJ = json_object_get(rootJ, "modules");
						if (modulesJ) {
							size_t moduleIndex;
							int minX = 1000000, minY = 100000;
							json_t* moduleJ;
							json_array_foreach(modulesJ, moduleIndex, moduleJ) {
								// read min pos
								json_t* posJ = json_object_get(moduleJ, "pos");
								int x = 0, y = 0;
								json_unpack(posJ, "[i, i]", &x, &y);
								minX = std::min(minX, x);
								minY = std::min(minY, y);
							}
							minX *= -1;
							minX += (int) (moduleWidget->getPosition().x / RACK_GRID_WIDTH) - 2000;
							minY *= -1;
							minY += (int) (moduleWidget->getPosition().y / RACK_GRID_HEIGHT) - 100;
							json_array_foreach(modulesJ, moduleIndex, moduleJ) {
								// recalc pos
								json_t* posJ = json_object_get(moduleJ, "pos");
								int x = 0, y = 0;
								json_unpack(posJ, "[i, i]", &x, &y);
								x += minX;
								y += minY;
								posJ = json_pack("[i, i]", x, y);
								json_object_set(moduleJ, "pos", posJ);
							}
						}

						APP->scene->rack->pasteJsonAction(rootJ);
						// delete calling module if not connected
						/*if (moduleWidget->module) {
							bool hasConnections = false;
							for (auto& port: moduleWidget->module->inputs) {
								if (port.isConnected()) {
									hasConnections = true;
									break;
								}
							}
							if (!hasConnections) {	
								for (auto& port: moduleWidget->module->outputs) {
									if (port.isConnected()) {
										hasConnections = true;
										break;
									}
								}
							}
							if (!hasConnections) {	
								moduleWidget->requestDelete();
							}
						}*/
					} catch (Exception& e) {
						osdialog_message(OSDIALOG_WARNING, OSDIALOG_OK, e.what());
					}
				}));
			}
		}
	}
	if (!hasPresets) {
		menu->addChild(createMenuLabel("(None)"));
	}
};

void BaseModuleWidget::appendContextMenu(Menu* menu) {
	menu->addChild(new MenuSeparator);
	// menu->addChild(createSubmenuItem("Import selection", "", [=](Menu* menu) {
		// WeakPtr<ModuleWidget> weakThis = this;
		// menu->addChild(createMenuLabel("User selections"));
		// appendSelectionItems(menu, weakThis, asset::user("selections"));
		// menu->addChild(new ui::MenuSeparator);
		// menu->addChild(createMenuLabel("Factory selections"));
		// appendSelectionItems(menu, weakThis, asset::plugin(pluginInstance, system::join("selections", model->slug)));
	// }));
	contextMenu(menu);
}

void BaseModuleWidget::contextMenu(Menu* menu) {}

void BaseModuleWidget::onButton(const ButtonEvent& e) {
	bool selected = APP->scene->rack->isSelected(this);

	if (selected) {
		if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
			ui::Menu* menu = createMenu();
			APP->scene->rack->appendSelectionContextMenu(menu);
		}

		e.consume(this);
	}

	OpaqueWidget::onButton(e);

	if (e.getTarget() == this) {
		// Set starting drag position
		if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT) {
			internal->dragOffset = e.pos;
		}
		// Toggle selection on Shift-click
		if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_LEFT && (e.mods & RACK_MOD_MASK) == GLFW_MOD_SHIFT) {
			APP->scene->rack->select(this, !selected);
		}
	}

	if (!e.isConsumed() && !selected) {
		// Open context menu on right-click
		if (e.action == GLFW_PRESS && e.button == GLFW_MOUSE_BUTTON_RIGHT) {
			createCustomContextMenu();
			e.consume(this);
		}
	}
}

void BaseModuleWidget::createCustomContextMenu() {
	ui::Menu* menu = createMenu();
	assert(model);

	WeakPtr<ModuleWidget> weakThis = this;

	// Brand and module name
	menu->addChild(createMenuLabel(model->name));
	menu->addChild(createMenuLabel(model->plugin->brand));

	// Info
	menu->addChild(createSubmenuItem("Info", "", [=](ui::Menu* menu) {
		model->appendContextMenu(menu);
	}));

	// Preset
	menu->addChild(createSubmenuItem("Preset", "", [=](ui::Menu* menu) {
		menu->addChild(createMenuItem("Copy", RACK_MOD_CTRL_NAME "+C", [=]() {
			if (!weakThis)
				return;
			weakThis->copyClipboard();
		}));

		menu->addChild(createMenuItem("Paste", RACK_MOD_CTRL_NAME "+V", [=]() {
			if (!weakThis)
				return;
			weakThis->pasteClipboardAction();
		}));

		menu->addChild(createMenuItem("Open", "", [=]() {
			if (!weakThis)
				return;
			weakThis->loadDialog();
		}));

		menu->addChild(createMenuItem("Save as", "", [=]() {
			if (!weakThis)
				return;
			weakThis->saveDialog();
		}));

		menu->addChild(createMenuItem("Save template", "", [=]() {
			if (!weakThis)
				return;
			weakThis->saveTemplateDialog();
		}));

		menu->addChild(createMenuItem("Clear template", "", [=]() {
			if (!weakThis)
				return;
			weakThis->clearTemplateDialog();
		}, !weakThis->hasTemplate()));

		// Scan `<user dir>/presets/<plugin slug>/<module slug>` for presets.
		menu->addChild(new ui::MenuSeparator);
		menu->addChild(createMenuLabel("User presets"));
		appendPresetItems(menu, weakThis, weakThis->model->getUserPresetDirectory());

		// Scan `<plugin dir>/presets/<module slug>` for presets.
		menu->addChild(new ui::MenuSeparator);
		menu->addChild(createMenuLabel("Factory presets"));
		appendPresetItems(menu, weakThis, weakThis->model->getFactoryPresetDirectory());
	}));

	menu->addChild(createSubmenuItem("Import selection", "", [=](Menu* menu) {
		WeakPtr<ModuleWidget> weakThis = this;
		menu->addChild(createMenuLabel("User selections"));
		appendSelectionItems(menu, weakThis, asset::user("selections"));
		menu->addChild(new ui::MenuSeparator);
		menu->addChild(createMenuLabel("Factory selections"));
		appendSelectionItems(menu, weakThis, asset::plugin(pluginInstance, system::join("selections", model->slug)));
	}));

	// Initialize
	menu->addChild(createMenuItem("Initialize", RACK_MOD_CTRL_NAME "+I", [=]() {
		if (!weakThis)
			return;
		weakThis->resetAction();
	}));

	// Randomize
	menu->addChild(createMenuItem("Randomize", RACK_MOD_CTRL_NAME "+R", [=]() {
		if (!weakThis)
			return;
		weakThis->randomizeAction();
	}));

	// Disconnect cables
	menu->addChild(createMenuItem("Disconnect cables", RACK_MOD_CTRL_NAME "+U", [=]() {
		if (!weakThis)
			return;
		weakThis->disconnectAction();
	}));

	// Bypass
	std::string bypassText = RACK_MOD_CTRL_NAME "+E";
	bool bypassed = module && module->isBypassed();
	if (bypassed)
		bypassText += " " CHECKMARK_STRING;
	menu->addChild(createMenuItem("Bypass", bypassText, [=]() {
		if (!weakThis)
			return;
		weakThis->bypassAction(!bypassed);
	}));

	// Duplicate
	menu->addChild(createMenuItem("Duplicate", RACK_MOD_CTRL_NAME "+D", [=]() {
		if (!weakThis)
			return;
		weakThis->cloneAction(false);
	}));

	// Duplicate with cables
	menu->addChild(createMenuItem("└ with cables", RACK_MOD_SHIFT_NAME "+" RACK_MOD_CTRL_NAME "+D", [=]() {
		if (!weakThis)
			return;
		weakThis->cloneAction(true);
	}));

	// Delete
	menu->addChild(createMenuItem("Delete", "Backspace/Delete", [=]() {
		if (!weakThis)
			return;
		weakThis->removeAction();
	}, false, true));

	appendContextMenu(menu);
}

