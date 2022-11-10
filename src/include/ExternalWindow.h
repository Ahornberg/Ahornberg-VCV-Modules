struct ExternalWindow {
	GLFWwindow* window;
	NVGcontext* context;
	
	ExternalWindow();
	~ExternalWindow();
  	void create();
  	void show();
	void hide();
	void destroy();
};

ExternalWindow::ExternalWindow() {
	
}

void ExternalWindow::create() {
	if (window == nullptr) {
		// Tell GLFW the properties of the window we want to create.
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
		glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

		// Create the window.
		window = glfwCreateWindow(400, 300, "Opsylloscope", NULL, NULL);


		// Don't wait for vsync when rendering 'cos it slows down the Rack UI thread.
		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);

		// If you want your window to stay on top of other windows.
//			glfwSetWindowAttrib(_window, GLFW_FLOATING, true);

		// Create a NanoVG context for painting the popup window.
//			context = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
		context = nvgCreateGL2(0);

		// Hand OpenGL back to Rack.
		glfwMakeContextCurrent(APP->window->win);
	}
}

void ExternalWindow::show() {
	if (window != nullptr) {
		
	}
}

void ExternalWindow::hide() {
	if (window != nullptr) {
		
	}
}

void ExternalWindow::destroy() {
	if (window != nullptr) {
		glfwMakeContextCurrent(window);
		nvgDeleteGL2(context);
		glfwDestroyWindow(window);
		glfwMakeContextCurrent(APP->window->win);
		window = nullptr;
	}
}

ExternalWindow::~ExternalWindow() {
	destroy();
}
