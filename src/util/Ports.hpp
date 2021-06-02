constexpr static float AUDIO_MUTE_SLEW = 350;

struct BasicPort : SvgPort, ManualText {
	BasicPort();
	void setSvg(const std::string& filename);
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
