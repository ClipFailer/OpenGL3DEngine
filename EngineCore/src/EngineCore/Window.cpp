#include "EngineCore/Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "EngineCore/Log.hpp"

namespace Engine {

	static bool s_glfwInitialized = false;

	Window::Window(
		const std::string& 	name, 
		uint16_t 			width,
		uint16_t 			height
	) 
		: m_data({
			width,
			height,
			std::move(name)
		})
	{
		int8_t resultCode = init();
	}

	Window::~Window() {
		shutdown();
	}

	void Window::update() {
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(m_id);
		glfwPollEvents();
	}

	int8_t Window::init() {
		LOG_INFO("Creating window {0} ({1}x{2})", m_data.name, m_data.width, m_data.height);

        /* Инициализация GLFW */
        if (!s_glfwInitialized) {
			if (!glfwInit()) {
				LOG_CRIT("FAILED TO INITIALIZE GLFW!");
				return -1;
			}
			s_glfwInitialized = true;
		}

        m_id = glfwCreateWindow(m_data.width, m_data.height, m_data.name.c_str(), NULL, NULL);
        if (!m_id)
        {
            LOG_CRIT("FAILED TO CREATE WINDOW {0}!!!", m_data.name);
            glfwTerminate();
            return -1;
        }

        LOG_INFO("Window {0} was successfully created!", m_data.name);

        glfwMakeContextCurrent(m_id);

        // Инициализация GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRIT("FAILED TO LOAD GLAD!!!");
            return -1;
        }
        LOG_INFO("GLAD was successfully loaded!");

		glfwSetWindowUserPointer(m_id, &m_data);

		glfwSetWindowSizeCallback(
			m_id, 
			[](GLFWwindow* pWindow, int width, int height) {
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
				data->width = width;
				data->height = height;
				
				EventWindowResize e(width, height);
				
				data->eventCallback(e);
			}
		);

		glfwSetCursorPosCallback(
			m_id,
			[](GLFWwindow* pWindow, double x, double y) {
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
				
				EventMouseMove e(x, y);
				
				data->eventCallback(e);
			}
		);

		glfwSetWindowCloseCallback(
			m_id,
			[](GLFWwindow* pWindow) {
				WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
				
				EventCloseWindow e;
				
				data->eventCallback(e);
			}
		);

        return 0;
	}

	int8_t Window::shutdown() {
		glfwDestroyWindow(m_id);
		glfwTerminate();

		return 0;
	}

	void Window::setEventCallback(const EventCallback& callback) {
		m_data.eventCallback = callback;
	}
	
} // namespace Engine
