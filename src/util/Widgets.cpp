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

// Module with intern params and Widget with screws

void BaseModule::configScrewParams() {
	for (auto i = 0; i < NUM_MAX_SCREWS; ++i) {
		configSwitch(SCREW_PARAM + i, 0, 5, 5, "Screw", {
			"I warn you, UNDO won't help you here!",
			"If you continue doing that, the module will fall out of the rack!",
			"Stop doing that!",
			"Don't pull it out!",
			"loose",
			"fixed"
		});
		getParamQuantity(SCREW_PARAM + i)->randomizeEnabled = false;
	}
}

ModuleWidgetWithScrews::ModuleWidgetWithScrews() {
	hasScrews = false;
}

void ModuleWidgetWithScrews::setPanel(const std::string& filename) {
	ModuleWidget::setPanel(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
}

void ModuleWidgetWithScrews::setScrews(ScrewTopLeft topLeft, ScrewTopRight topRight, ScrewBottomLeft bottomLeft, ScrewBottomRight bottomRight) {
	if (topLeft) {
		addScrew(Vec(0, 0), BaseModule::SCREW_PARAM);
	} else if (module) {
		module->params[BaseModule::SCREW_PARAM].setValue(0);
	}
	if (topRight) {
		addScrew(Vec(box.size.x - RACK_GRID_WIDTH, 0), BaseModule::SCREW_PARAM + 1);
	} else if (module) {
		module->params[BaseModule::SCREW_PARAM + 1].setValue(0);
	}
	if (bottomLeft) {
		if (bottomLeft == SCREW_BOTTOM_LEFT_INDENTED) {
			addScrew(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), BaseModule::SCREW_PARAM + 2);
		} else {
			addScrew(Vec(0, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), BaseModule::SCREW_PARAM + 2);
		}
	} else if (module) {
		module->params[BaseModule::SCREW_PARAM + 2].setValue(0);
	}
	if (bottomRight) {
		addScrew(Vec(box.size.x - RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH), BaseModule::SCREW_PARAM + 3);
	} else if (module) {
		module->params[BaseModule::SCREW_PARAM + 3].setValue(0);
	}
}

void ModuleWidgetWithScrews::setWidthInHP(int hp) {
	setSize(Vec(hp * 15, 380));
}

void ModuleWidgetWithScrews::addScrew(Vec pos, int screwParam) {
	KnobScrewMountModule* screw = dynamic_cast<KnobScrewMountModule*>(createParam<KnobScrewMountModule>(pos, module, screwParam));
	screw->module = module;
	screw->param = screwParam;
	addParam(screw);
	hasScrews = true;
}

void ModuleWidgetWithScrews::step() {
	ModuleWidget::step();
	if (hasScrews && module) {
		for (auto i = 0; i < NUM_MAX_SCREWS; ++i) {
			if (module->params[BaseModule::SCREW_PARAM + i].getValue()) {
				return;
			}
		}
		removeAction();
	}
}

bool ModuleWidgetWithScrews::isBypassed() {
	if (module) {
		if (module->isBypassed()) {
			return true;
		}
		return false;
	}
	return false;
}

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

void ModuleWidgetWithScrews::appendContextMenu(Menu* menu) {
	menu->addChild(new MenuSeparator);
	menu->addChild(createSubmenuItem("Import selection", "", [=](Menu* menu) {
		WeakPtr<ModuleWidget> weakThis = this;
		menu->addChild(createMenuLabel("User selections"));
		appendSelectionItems(menu, weakThis, asset::user("selections"));
		menu->addChild(new ui::MenuSeparator);
		menu->addChild(createMenuLabel("Factory selections"));
		appendSelectionItems(menu, weakThis, asset::plugin(pluginInstance, system::join("selections", model->slug)));
	}));
	contextMenu(menu);
}

void ModuleWidgetWithScrews::contextMenu(Menu* menu) {}
