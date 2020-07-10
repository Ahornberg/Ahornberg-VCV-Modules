struct BasicPort : SvgPort {
	BasicPort();
	void setSvg(const std::string& filename);
};

struct InPort : BasicPort {
	InPort();
};

struct OutPort : BasicPort {
	OutPort();
};
