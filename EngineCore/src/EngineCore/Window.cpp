#include "EngineCore/Window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "EngineCore/Log.hpp"

namespace Engine {

	static bool s_glfwInitialized = false;

	GLfloat verteces[] = {
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
	};

	const char* vertexShader = 
	"#version 460\n"
	"layout(location = 0) in vec3 vertex_position;\n"
	"layout(location = 1) in vec3 vertex_color;\n"
	"out vec3 color;\n"
	"void main() {\n"
	"	color = vertex_color;\n"
	"	gl_Position = vec4(vertex_position, 1.0f);\n"
	"}\n";

	const char* fragmentShader = 
	"#version 460\n"
	"in vec3 color;\n"
	"out vec4 fragment_color;\n"
	"void main() {\n"
	"	fragment_color = vec4(color, 1.0f);\n"
	"}\n";

	GLuint shaderProgram 	= 0;
	GLuint vao 				= 0;

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

		// ImGui инициализация.
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(m_id, true);

		LOG_INFO("ImGui was successfully initialized");
	}

	Window::~Window() {
		shutdown();
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

		glfwSetFramebufferSizeCallback(
			m_id,
			[](GLFWwindow* pWindow, int width, int height) {
				glViewport(0, 0, width, height);
			}
		);

		m_pShaderProgram = std::make_unique<ShaderProgram>(vertexShader, fragmentShader);
		if (!m_pShaderProgram->isCompiled()) {
			return -1;
		}

		GLuint vertecesVBO = 0;
		glGenBuffers(1, &vertecesVBO);
		glBindBuffer(GL_ARRAY_BUFFER, vertecesVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verteces), verteces, GL_STATIC_DRAW);

		GLuint colorsVBO = 0;
		glGenBuffers(1, &colorsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertecesVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        return 0;
	}
	
	void Window::update() {
		glClearColor(m_bgColor[0], m_bgColor[1], m_bgColor[2], m_bgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);

		m_pShaderProgram->bind();
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Получаем структуру, хранящую информацию для работы ImGui
		ImGuiIO& io = ImGui::GetIO();
		
		io.DisplaySize.x = static_cast<float>(getWidth());
		io.DisplaySize.y = static_cast<float>(getHeight());

		// Подготовка нового кадра OpenGL через ImGui
		ImGui_ImplOpenGL3_NewFrame();
		// Обработка событий ImGui
		ImGui_ImplGlfw_NewFrame();
		// Начало кадра
		ImGui::NewFrame();

		ImGui::Begin("Выбор цвета фона");
		ImGui::ColorEdit4("Цвет фона", m_bgColor);
		ImGui::End();

		// Отрисовка кадра 
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(m_id);
		glfwPollEvents();
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
