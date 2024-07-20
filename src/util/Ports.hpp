constexpr static float AUDIO_MUTE_SLEW = 350;

struct BasicPort : ThemedSvgPort {
	BasicPort();
	void setSvg(const std::string& filename, const std::string& filenameDark);
};

struct InPort : BasicPort {
	InPort();
};

struct InPortSmall : BasicPort {
	InPortSmall();
};

struct OutPort : BasicPort {
	OutPort();
};

struct OutPortSmall : BasicPort {
	OutPortSmall();
};
