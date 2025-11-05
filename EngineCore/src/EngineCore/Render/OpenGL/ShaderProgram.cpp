#include "EngineCore/Render/OpenGL/ShaderProgram.hpp"

#include <string>

#include <glad/glad.h>

#include "EngineCore/Log.hpp"

namespace Engine {

	ShaderProgram::ShaderProgram(ShaderProgram&& rhs) {
		m_id = rhs.m_id;
		m_isCompiled = rhs.m_isCompiled;

		rhs.m_id = 0;
		rhs.m_isCompiled = false;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& rhs) {
		glDeleteProgram(m_id);
		m_id = rhs.m_id;
		m_isCompiled = rhs.m_isCompiled;

		rhs.m_id = 0;
		rhs.m_isCompiled = false;
		return *this;
	}

	/// @internal
	/// @brief Вспомогательная функция для компиляции шейдера.
	/// @param source Код шейдера.
	/// @param type Тип шейдера.
	/// @param shaderId Идентификатор шейдерной программы.
	/// @return Результат компиляции шейдера (true - успех).
	bool createShader(
		const char* 	source,
		const GLuint	type,
		GLuint&			shaderId
	) {
		shaderId = glCreateShader(type);
		glShaderSource(shaderId, 1, &source, nullptr);
		glCompileShader(shaderId);

		GLint success;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			char infoLog[1024];
			glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);

			std::string log(infoLog);
			LOG_CRIT("Shader compilation error:\n{}", log);
			return false;
		}
		return true;
	}
	
	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_id);
	}

	ShaderProgram::ShaderProgram(
		const char* vertexShaderSource,
		const char* FragmentShaderSource
	) {
		GLuint vertexShader = 0;
		if (!createShader(vertexShaderSource, GL_VERTEX_SHADER, vertexShader)) {
			LOG_CRIT("Vertex shader comlile-time error!");
			glDeleteShader(vertexShader);
			return;
		}

		GLuint fragmentShader = 0;
		if (!createShader(FragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader)) {
			LOG_CRIT("Fragment shader comlile-time error!");
			glDeleteShader(fragmentShader);
			return;
		}

		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShader);
		glAttachShader(m_id, fragmentShader);
		glLinkProgram(m_id);

		GLint success;
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE) {
			char infoLog[1024];
			glGetProgramInfoLog(m_id, 1024, nullptr, infoLog);

			std::string log(infoLog);
			LOG_CRIT("Shader program link error:\n{}", log);
			glDeleteProgram(m_id);
			m_id = 0;
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			return;
		}

		m_isCompiled = true;

		glDetachShader(m_id, vertexShader);
		glDetachShader(m_id, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void ShaderProgram::bind() const {
		glUseProgram(m_id);
	}

	void ShaderProgram::unbind() const {
		glUseProgram(0);
	}

	

} // namespace Engine
