#include "EngineCore/Render/OpenGL/VertexArray.hpp"

#include <glad/glad.h>

#include "EngineCore/Render/OpenGL/VertexBuffer.hpp"
#include "EngineCore/Log.hpp"

namespace Engine {

	VertexArray::VertexArray() {
		glGenVertexArrays(1, &m_id);
	}

	VertexArray::~VertexArray() {
		glDeleteVertexArrays(1, &m_id);
	}

	VertexArray::VertexArray(VertexArray&& rhs) noexcept {
		m_id = rhs.m_id;
		m_elements_count = rhs.m_elements_count;

		rhs.m_id = 0;
		rhs.m_elements_count = 0;
	}

	VertexArray& VertexArray::operator=(VertexArray&& rhs) noexcept {
		m_id = rhs.m_id;
		m_elements_count = rhs.m_elements_count;

		rhs.m_id = 0;
		rhs.m_elements_count = 0;

		return *this;
	}

	void VertexArray::bind() const noexcept {
		glBindVertexArray(m_id);
	}

	void VertexArray::unbind() noexcept {
		glBindVertexArray(0);
	}

	void VertexArray::addBuffer(const VertexBuffer& vertexBuffer) {
		bind();
		vertexBuffer.bind();

		for (const BufferElement& currentElement : vertexBuffer.getLayout().getElements()) {
			glEnableVertexAttribArray(m_elements_count);
			glVertexAttribPointer(
				m_elements_count,
				currentElement.componentCount,
				currentElement.componentType,
				GL_FALSE,
				vertexBuffer.getLayout().getStride(),
				(void*)(currentElement.offset)
			);
			++m_elements_count;
		}

	}

} // namespace Engine
