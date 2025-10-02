#include "Engine/Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Engine/Log.hpp"

namespace Engine {
	Window::Window(
		const std::string& title,
		const unsigned int width,
		const unsigned int height
	)	: m_data({std::move(title), width, height})
	{
		int resultCode = init();
	}

	Window::~Window() {
		shutdown();
	}

	void Window::update() {
		glClearColor(0.f, 1.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_pWindow);
		glfwPollEvents();
	}

	int Window::init() {
		LOG_INFO(
			"Creating window {0} with size {1}x{2}", 
			m_data.m_title,  
			m_data.m_width,  
			m_data.m_height
		);

		/* Initialize the library */
		if (!glfwInit()) {
			LOG_CRIT("Failed to initialize GLFW!");
			return -1;
		}

		/* Create a windowed mode window and its OpenGL context */
		m_pWindow = glfwCreateWindow(
			m_data.m_width, 
			m_data.m_height, 
			m_data.m_title.c_str(), 
			NULL, 
			NULL
		);

		const char* description;
		int code = glfwGetError(&description);
		if (code != GLFW_NO_ERROR) {
			LOG_CRIT("GLFW error {0}: {1}", code, description ? description : "Unknown");
		}


		if (!m_pWindow) {
			LOG_CRIT("Failed to create window {0}!", m_data.m_title);
			glfwTerminate();
			return -1;
		}

		/* Make the window's context current */
		glfwMakeContextCurrent(m_pWindow);

		LOG_INFO("Window {0} has successfully created.", m_data.m_title);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			LOG_CRIT("Failed to load GLAD!");
			return -1;
		}

		glfwSetWindowUserPointer(m_pWindow, &m_data);

		glfwSetWindowSizeCallback(
			m_pWindow, 
			[](GLFWwindow* pWindow, int width, int height) {
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
				if (data) {
					data->m_width = width;
					data->m_height = height;

					Event event;
					event.width = width;
					event.height = height; 
					data->eventCallbackFn(event);
				}
			}
		);

		LOG_INFO("GLAD successfully loaded.");
		return 0;
	}

	void Window::shutdown() {
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}
} // namespace Engine
