#define NDEBUG
#include <assert.h>
#include <osdialog.h>
#include <regex>
#include "rack.hpp"

using namespace rack;
using simd::float_4;

extern Plugin* pluginInstance;

extern Model* modelTracker;
extern Model* modelTapeRecorder;
extern Model* modelTapeRecorderMixer;
extern Model* modelTapeInspector;
extern Model* modelFlyingFader;
extern Model* modelEqualDivision;
extern Model* modelCVFreqShift;
extern Model* modelFunWithFrames;
extern Model* modelMoreFunWithFrames;
extern Model* modelMetronome;
extern Model* modelHarmonizer;
extern Model* modelMIDIController;
extern Model* modelMIDIPolyExpression;
extern Model* modelXGlide;
extern Model* modelMIDIOverAudio;
extern Model* modelCopyPastePreset;
// extern Model* modelManual;

#include "util/Manuals.hpp"
#include "util/Colors.hpp"
#include "util/Fonts.hpp"
#include "util/Ports.hpp"
#include "util/Knobs.hpp"
#include "util/Switches.hpp"
#include "util/Widgets.hpp"
#include "util/Displays.hpp"
#include "util/Drivers.hpp"

#ifndef NDEBUG
#  include "test/Asserts.hpp"
#endif
