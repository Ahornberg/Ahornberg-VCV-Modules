#include "TapeInspectorWidget.hpp"

const NVGcolor TapeAudioDisplay::TAPE_TEXT_COLORS[] = {
	COLOR_YELLOW,
	COLOR_GREEN_DARK,
	COLOR_RED,
	COLOR_BROWN
};

TapeAudioDisplay::TapeAudioDisplay(Rect box, TapeInspector *tapeInspector) : Display(box) {
	this->tapeInspector = tapeInspector;
}

void TapeAudioDisplay::drawText(const DrawArgs& disp) {
	// TODO "Tape Inspector - Expander For Tape Recorder" when not connected
	// TODO lines for beats, bars, loop points
	// maybe input for time and volume
	if (tapeInspector && tapeInspector->tapeRecorder && !tapeInspector->tapeRecorder->changeTapeInterrupt) {
		TapeRecorder* tapeRecorder = tapeInspector->tapeRecorder;
		int audioBufferPosition = tapeRecorder->audioBufferPosition;
		float volume = pow(DISPLAY_WIDTH / 2.f, tapeInspector->params[TapeInspector::AMPLITUDE_PARAM].getValue());
		double time = pow(tapeInspector->params[TapeInspector::TIME_PARAM].getValue(), 16);
		int step = std::max(1.0, 0.004 / time);

		nvgLineCap(disp.vg, NVG_ROUND);
		nvgMiterLimit(disp.vg, 2.f);
		nvgStrokeWidth(disp.vg, 1.0f);
		nvgScissor(disp.vg, 2, 2, DISPLAY_WIDTH - 4, DISPLAY_HEIGHT - 4);
		// bars and beats
		double beatsPerBar60 = tapeRecorder->params[TapeRecorder::BEATS_PER_BAR_PARAM].getValue() * 60.0;
		int bar = 0;
		int beat = 0;
		for (int i = audioBufferPosition - (DISPLAY_HEIGHT / 2.0) / time; i <= audioBufferPosition + (DISPLAY_HEIGHT / 2.0) / time; i += step) {
			if (i < 0) {
				continue;
			}
			if (i >= tapeRecorder->sizeAudioBuffer) {
				break;
			}
			double tapePosition = (tapeInspector->sampleTime * i * tapeRecorder->params[TapeRecorder::TEMPO_PARAM].getValue()) / beatsPerBar60;
			int newBar = (int) tapePosition;
			int newBeat = (int) ((tapePosition - bar) * tapeRecorder->params[TapeRecorder::BEATS_PER_BAR_PARAM].getValue());
			if (newBar != bar || tapePosition == 0) {
				nvgStrokeColor(disp.vg, COLOR_GREY);
				nvgBeginPath(disp.vg);
				nvgMoveTo(disp.vg, 0,  DISPLAY_HEIGHT / 2.0 - time * (i - audioBufferPosition) - 0.5);
				nvgLineTo(disp.vg, DISPLAY_WIDTH,  DISPLAY_HEIGHT / 2.0 - time * (i - audioBufferPosition) - 0.5);
				nvgStroke(disp.vg);
				bar = newBar;
			} else if ((newBeat > 0 && newBeat != beat) || tapePosition == 0) {
				nvgStrokeColor(disp.vg, COLOR_GREY);
				nvgBeginPath(disp.vg);
				nvgMoveTo(disp.vg, DISPLAY_WIDTH / 2.0,  DISPLAY_HEIGHT / 2.0 - time * (i - audioBufferPosition) - 0.5);
				nvgLineTo(disp.vg, DISPLAY_WIDTH,  DISPLAY_HEIGHT / 2.0 - time * (i - audioBufferPosition) - 0.5);
				nvgStroke(disp.vg);
				beat = newBeat;
			}
			
		}
		// audio
		for (int channel = 0; channel < tapeRecorder->trackCountParam; ++channel) {
			nvgStrokeColor(disp.vg, TAPE_TEXT_COLORS[channel]);
			nvgBeginPath(disp.vg);
			bool first = true;
			for (int i = audioBufferPosition - (DISPLAY_HEIGHT / 2.0) / time; i <= audioBufferPosition + (DISPLAY_HEIGHT / 2.0) / time; i += step) {
				if (i < 0) {
					continue;
				}
				if (i >= tapeRecorder->sizeAudioBuffer) {
					break;
				}
				if (first) {
					nvgMoveTo(disp.vg, tapeRecorder->audioFile.samples[channel][i] * volume + DISPLAY_WIDTH / 2.f, DISPLAY_HEIGHT / 2.0 - time * (i - audioBufferPosition) - 0.5);
					first = false;
				} else {
					nvgLineTo(disp.vg, tapeRecorder->audioFile.samples[channel][i] * volume + DISPLAY_WIDTH / 2.f, DISPLAY_HEIGHT / 2.0 - time * (i - audioBufferPosition) - 0.5);
				}
			}
			nvgStroke(disp.vg);
		}
		// tape head
		nvgStrokeColor(disp.vg, COLOR_WHITE);
		nvgBeginPath(disp.vg);
		nvgMoveTo(disp.vg, 0, DISPLAY_HEIGHT / 2);
		nvgLineTo(disp.vg, DISPLAY_WIDTH, DISPLAY_HEIGHT / 2);
		nvgStroke(disp.vg);
	} else {
		nvgBeginPath(disp.vg);
		nvgRotate(disp.vg, (-90 * NVG_PI) / 180);
		nvgFillColor(disp.vg, textColorLight);
		nvgFontSize(disp.vg, 10);
		nvgText(disp.vg, -204, 32, "tape inspector", NULL);
	}
}

TapeInspectorWidget::TapeInspectorWidget(TapeInspector* module) {
	setModule(module);
	setPanel("res/TapeInspector.svg");
	setWidthInHP(4);

	addParam(createParamCentered<KnobSmall>(Vec(40, 30), module, TapeInspector::TIME_PARAM));
	addParam(createParamCentered<KnobSmall>(Vec(20, 60), module, TapeInspector::AMPLITUDE_PARAM));

	addChild(new TapeAudioDisplay(Rect(30 - TapeAudioDisplay::DISPLAY_WIDTH / 2.f, 227.5 - TapeAudioDisplay::DISPLAY_HEIGHT / 2.f, TapeAudioDisplay::DISPLAY_WIDTH, TapeAudioDisplay::DISPLAY_HEIGHT), module));
}

Model* modelTapeInspector = createModel<TapeInspector, TapeInspectorWidget>("TapeInspector");
