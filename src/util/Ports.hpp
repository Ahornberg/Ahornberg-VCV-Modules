struct BasicPort : SvgPort {
	BasicPort() {
		shadow->hide();
	}

	void setSvg(const std::string& filename) {
		SvgPort::setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, filename)));
	}	
};

struct InPort : BasicPort {
	InPort() {
		BasicPort::setSvg("res/ports/In.svg");
	}	
};

struct OutPort : BasicPort {
	OutPort() {
		BasicPort::setSvg("res/ports/Out.svg");
	}	
};
