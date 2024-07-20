#include "CopyPastePresetWidget.hpp"

CopyPastePresetWidget::CopyPastePresetWidget(CopyPastePreset* module) {
	setModule(module);
	setPanel("res/CopyPastePreset.svg", "res/CopyPastePreset-dark.svg");
	setWidthInHP(2);
	waitCounter = WAIT_COUNTER;
	
	addParam(createParamCentered<RoundSwitchMedium>(Vec(15, 130), module, CopyPastePreset::DO_COPY_PASTE_PARAM));
	addInput(createInputCentered<InPort>(Vec(15, 275), module, CopyPastePreset::LINK_CV_INPUT));
	addOutput(createOutputCentered<OutPort>(Vec(15, 305), module, CopyPastePreset::LINK_CV_OUTPUT));
}

void CopyPastePresetWidget::step() {
	if (module && module->params[CopyPastePreset::DO_COPY_PASTE_PARAM].getValue()) {
		if (waitCounter > 0) {
			--waitCounter;
		} else {
			const char* clipboardContent = glfwGetClipboardString(APP->window->win);
			Module* leftExpander = module->leftExpander.module;
			if (leftExpander) {
				for (auto const& i : parent->children) {
					ModuleWidget* leftWidget = dynamic_cast<ModuleWidget*>(i);
					if (leftWidget->module == leftExpander) {
						leftWidget->copyClipboard();
						break;
					}
				}
			}
			Module* rightExpander = module->rightExpander.module;
			while (leftExpander && rightExpander && leftExpander->model == rightExpander->model) {
				for (auto const& i : parent->children) {
					ModuleWidget* rightWidget = dynamic_cast<ModuleWidget*>(i);
					if (rightWidget->module == rightExpander) {
						rightWidget->pasteClipboardAction();
						break;
					}
				}
				leftExpander = rightExpander;
				rightExpander = leftExpander->rightExpander.module;
			}	
			module->params[CopyPastePreset::DO_COPY_PASTE_PARAM].setValue(0);
			if (clipboardContent) {
				glfwSetClipboardString(APP->window->win, clipboardContent);
			}
			waitCounter = WAIT_COUNTER;
		}
	}
	BaseModuleWidget::step();
}

Model* modelCopyPastePreset = createModel<CopyPastePreset, CopyPastePresetWidget>("CopyPastePreset");
