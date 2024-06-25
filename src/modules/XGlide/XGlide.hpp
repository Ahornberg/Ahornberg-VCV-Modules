#include "../../Ahornberg.hpp"

struct XGlideChannelMapping {
	float volume;
	float pitch;
	float modulation;
	int channelOut;
	int glideTarget;
	float oldVolume;
};

struct XGlide : Module {
	constexpr static int MAX_CHANNELS = 16;
	constexpr static int NO_GLIDE = -1;
	constexpr static int GLIDE_TARGET = -2;
	const static std::vector<std::string> NOTE_NAMES;
	enum ParamIds {
		MIN_INTERVAL_PARAM,
		MAX_INTERVAL_PARAM,
		MIN_NOTE_PARAM,
		MAX_NOTE_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		VOLUME_INPUT,
		PITCH_INPUT,
		PEDAL_INPUT,
		MODULATION_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		VOLUME_OUTPUT,
		PITCH_OUTPUT,
		MODULATION_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		EXCLUDE_INTERVALS_LIGHT,
		EXCLUDE_NOTES_LIGHT,
		NUM_LIGHTS
	};

	XGlideChannelMapping channelMapping[MAX_CHANNELS];
	bool isResetChannelMapping;

	XGlide();
	void process(const ProcessArgs& args) override;
	bool checkRange(int channelA, int channelB);
	void remapToGlideSourceChannelOut(int channel, int maxUsedChannels);
	void remapToFirstUnusedChannelOut(int channel, int maxUsedChannels);
	void resetChannelMapping();
};
