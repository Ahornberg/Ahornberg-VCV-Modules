#include "rack.hpp"
#include <settings.hpp>

#include "util/Colors.hpp"
#include "util/Fonts.hpp"

using namespace rack;
using simd::float_4;

extern Plugin *pluginInstance;

extern Model* modelEqualDivision;
// extern Model* modelTapeRecorder;
extern Model* modelMetronome;

#include "util/Ports.hpp"
#include "util/Knobs.hpp"
#include "util/Switches.hpp"
#include "util/Widgets.hpp"
#include "util/Displays.hpp"
