#pragma once

#include "../engine/windowconfig.h"

#include <iostream>
#include <GLFW/glfw3.h>

namespace zakix {
	class Core;
#define MAX_KEYS	1024
#define MAX_BUTTONS	32

	class Window
	{
	private:
		Core *core;
		GLFWwindow *m_Window;
		WindowConfiguration config;
		bool m_Closed;

		bool m_Keys[MAX_KEYS];
		bool m_KeyState[MAX_KEYS];
		bool m_MouseButtons[MAX_BUTTONS];
		bool m_MouseState[MAX_BUTTONS];
		double mx, my;
		double sx, sy;
	public:
		Window(Core* core, const WindowConfiguration& config);
		~Window();
		void clear() const;
		void update();
		bool closed() const;

		inline int getWidth() const { return config.width; }
		inline int getHeight() const { return config.height; }
		inline GLFWwindow* getID() const { return m_Window; }

		bool isKeyPressed(unsigned int keycode) const;
		void setKeyPressed(unsigned int keycode, bool pressed);
		bool isMouseButtonPressed(unsigned int button) const;
		void setMouseButtonPressed(unsigned int button, bool pressed);
		void getMousePosition(double& x, double& y) const;
		double getScrollX() const;
		double getScrollY() const;
		
		void getGLError();
	private:
		bool init(const WindowConfiguration& config);
		friend static void window_resize(GLFWwindow* window, int width, int height);
		friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		friend static void cursor_scroll_callback(GLFWwindow* window, double xpos, double ypos);

	};
}