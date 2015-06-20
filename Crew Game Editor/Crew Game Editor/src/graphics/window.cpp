#include "window.h"
#include "../engine/core.h"

namespace zakix {

	Window::Window(Core* core, const WindowConfiguration& config) {
		this->core = core;
		this->config = config;
		if (!init(config))
			glfwTerminate();

		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_Keys[i] = false;
			m_KeyState[i] = false;
		}

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			m_MouseButtons[i] = false;
			m_MouseState[i] = false;
		}
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
	}

	bool Window::init(const WindowConfiguration& config) {
		glfwWindowHint(GLFW_RESIZABLE, config.resizable);
		glfwWindowHint(GLFW_VISIBLE, config.visible);
		glfwWindowHint(GLFW_DECORATED, config.decorated);

		glfwWindowHint(GLFW_RED_BITS, config.redBits);
		glfwWindowHint(GLFW_GREEN_BITS, config.greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, config.blueBits);
		glfwWindowHint(GLFW_ALPHA_BITS, config.alphaBits);
		glfwWindowHint(GLFW_DEPTH_BITS, config.depthBits);
		glfwWindowHint(GLFW_STENCIL_BITS, config.stencilBits);
		glfwWindowHint(GLFW_SAMPLES, config.samples);

		glfwWindowHint(GLFW_REFRESH_RATE, config.refreshRate);
		glfwWindowHint(GLFW_STEREO, config.stereo);

		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, config.forwardCompatibility);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, config.debug);
		m_Window = glfwCreateWindow(config.width, config.height, config.title, NULL, NULL);
		if(config.x != -2 && config.y != -2) glfwSetWindowPos(m_Window, config.x, config.y);

		if (!m_Window)
		{
			std::cout << "Failed to create GLFW window!" << std::endl;
			return false;
		}
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);
		glfwSetFramebufferSizeCallback(m_Window, window_resize);
		glfwSetKeyCallback(m_Window, key_callback);
		glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
		glfwSetCursorPosCallback(m_Window, cursor_position_callback);
		glfwSetScrollCallback(m_Window, cursor_scroll_callback);
		glfwSwapInterval(config.vsync);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Could not initialize GLEW!" << std::endl;
			return false;
		}

		std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
		return true;
	}

	bool Window::isKeyPressed(unsigned int keycode) const
	{
		if (keycode >= MAX_KEYS)
			return false;

		return m_Keys[keycode];
	}

	void Window::setKeyPressed(unsigned int keycode, bool pressed) {
		m_Keys[keycode] = pressed;
	}


	bool Window::isMouseButtonPressed(unsigned int button) const {
		if (button >= MAX_BUTTONS)
			return false;

		return m_MouseButtons[button];
	}

	void Window::setMouseButtonPressed(unsigned int button, bool pressed) {
		m_MouseButtons[button] = pressed;
	}

	void Window::getMousePosition(double& x, double& y) const
	{
		x = mx;
		y = my;
	}

	double Window::getScrollX() const {
		return sx;
	}

	double Window::getScrollY() const {
		return sy;
	}

	void Window::getGLError() {
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			std::cout << "OpenGL Error: " << error << std::endl;
	}

	void Window::clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::update()
	{
		sx = 0;
		sy = 0;
		
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	bool Window::closed() const
	{
		return glfwWindowShouldClose(m_Window) == 1;
	}

	void window_resize(GLFWwindow *window, int width, int height)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		if (width == win->getWidth() && height == win->getHeight()) return;
		glViewport(0, 0, width, height);
		win->config.width = width;
		win->config.height = height;
		win->core->resized(width, height);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_REPEAT) return;
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_Keys[key] = action != GLFW_RELEASE;
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_MouseButtons[button] = action != GLFW_RELEASE;
	}

	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->mx = xpos;
		win->my = ypos;
	}

	void cursor_scroll_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->sx = xpos;
		win->sy = ypos;
	}

}