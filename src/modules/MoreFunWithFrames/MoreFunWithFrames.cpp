#include "MoreFunWithFrames.hpp"

const std::string MoreFunWithFrames::TIME_TABLE_NAMES[] = {
	"A = Glide time",
	"B = Glide base note",
	"C = Vibrato",
	"D = Arpeggio"
};

MoreFunWithFrames::MoreFunWithFrames() { 
	config(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS);
	configParam(MAX_CHORD_SIZE_PARAM, 2, NUM_VALUE_IO + 1, 3, "Maximum Chord-Size");
	for (auto i = 0; i < NUM_VALUE_IO; ++i) {
		configSwitch(VALUE_CONVERT_MODE + i, 0, 1, 0, "Convert-Mode", {
			"linear",
			"linear with jump point"
		});
		std::string portLabel = "Time-Table " + TIME_TABLE_NAMES[i];
		configInput(VALUE_INPUT + i, portLabel);
		configOutput(VALUE_OUTPUT + i, portLabel);
		configBypass(VALUE_INPUT + i, VALUE_OUTPUT + i);
		lastPitchChord[i] = 0;
		pitchGate[i] = false;
	}
	configInput(NOTE_INPUT, "Pitch (1V/Octave)");
	configOutput(NOTE_OUTPUT, "Pitch (1V/Octave)");
	configBypass(NOTE_INPUT, NOTE_OUTPUT);
	configInput(GATE_INPUT, "Gate");
	configOutput(GATE_OUTPUT, "Gate");
	configBypass(GATE_INPUT, GATE_OUTPUT);
	configInput(CLOCK_INPUT, "Clock Trigger");
	arpeggioPosition = 0;
	clockInputTrigger.reset();
}

void MoreFunWithFrames::process(const ProcessArgs& args) {
	// arpeggio - on change value in d, send reset
	//            if value 0 then play original note - or not?
	//            ports: d in, arp note in, arp reset in, reset out
	// vibrato  - x = depth, y = speed
	//            if both values 0 or on reset then restart lfo
	//            button for lfo starts up or down
	//            on change value wait until next zero crossing
	//	          to apply or slew limter
	//            ports: c in
	//            params: up/down button, speed knob, depth knob
	// glide    - byte 1 = speed, byte 2 = start note relative to
	//            actual note value - if start note = 0 then
	//            last note is start value
	//            at reset last note = 0 Volts (C-5)
	//            ports: a in, b in
	//            params: speed knob
	// general  - ports: note in, reset in, note out	
	
	// time-table
	for (auto i = 0; i < NUM_VALUE_IO; i++) {
		if (outputs[VALUE_OUTPUT + i].isConnected()) {
			float valueOut = inputs[VALUE_INPUT + i].getVoltage() * 2.125;
			if (params[VALUE_CONVERT_MODE + i].getValue()) {
				if (valueOut > 5 * 2.125) {
					valueOut -= 256. / 12;
				}
			} else {
				valueOut -= 5;
			}
			outputs[VALUE_OUTPUT + i].setVoltage(valueOut);
		}
	}
	// chord-builder
	bool arpeggio = inputs[CLOCK_INPUT].isConnected();
	if (outputs[NOTE_OUTPUT].isConnected()) {
		if (arpeggio) {
			outputs[NOTE_OUTPUT].setChannels(1);
			if (clockInputTrigger.process(rescaleInput(inputs[CLOCK_INPUT]))) {
				++arpeggioPosition;
				if (arpeggioPosition > params[MAX_CHORD_SIZE_PARAM].getValue() || !inputs[GATE_INPUT].getVoltage()) {
					arpeggioPosition = 0;
				}
			}
		} else {
			outputs[NOTE_OUTPUT].setChannels(params[MAX_CHORD_SIZE_PARAM].getValue());
		}
		for (auto i = 0; i < params[MAX_CHORD_SIZE_PARAM].getValue(); i++) {
			float pitchOut = inputs[NOTE_INPUT].getVoltage();
			if (i > 0) {
				// add relative chord pitch from frames parameter value
				float pitchChord = inputs[VALUE_INPUT + i - 1].getVoltage() * 2.125;
				if (pitchChord > 5 * 2.125) {
					pitchChord -= 256. / 12;
				}
				if (pitchChord) {
					pitchOut += pitchChord;
					lastPitchChord[i - 1] = pitchOut;
					pitchGate[i - 1] = true;
				} else {
					pitchOut = lastPitchChord[i - 1];
					pitchGate[i - 1] = false;
				}
			}
			if (arpeggio) {
				if (i + 1 == arpeggioPosition) {
					if (!pitchGate[i - 1]) {
						++arpeggioPosition;
						if (arpeggioPosition > params[MAX_CHORD_SIZE_PARAM].getValue()) {
							arpeggioPosition = 0;
						}
					} else {
						outputs[NOTE_OUTPUT].setVoltage(pitchOut);
					}
				}
			} else {
				outputs[NOTE_OUTPUT].setVoltage(pitchOut, i);
			}
		}
		if (arpeggio && !arpeggioPosition) {
			// first arpeggio-pitch
			outputs[NOTE_OUTPUT].setVoltage(inputs[NOTE_INPUT].getVoltage());
		}
	}
	if (outputs[GATE_OUTPUT].isConnected()) {
		if (arpeggio) {
			outputs[GATE_OUTPUT].setChannels(1);
			outputs[GATE_OUTPUT].setVoltage(inputs[GATE_INPUT].getVoltage());
		} else {
			outputs[GATE_OUTPUT].setChannels(params[MAX_CHORD_SIZE_PARAM].getValue());
			for (auto i = 0; i < params[MAX_CHORD_SIZE_PARAM].getValue(); i++) {
				if (i > 0 && !pitchGate[i - 1]) {
					outputs[GATE_OUTPUT].setVoltage(0, i);
				} else {
					outputs[GATE_OUTPUT].setVoltage(inputs[GATE_INPUT].getVoltage(), i);
				}
			}
		}
	}
}
