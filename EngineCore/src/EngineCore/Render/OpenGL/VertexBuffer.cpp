#include "EngineCore/Render/OpenGL/VertexBuffer.hpp"

#include <glad/glad.h>

#include "EngineCore/Log.hpp"

namespace Engine {

	/// @internal
	/// @brief Переводит `EUsage` в `GLenum`.
	/// @param usage Тип использования способа отрисовки OpenGL.
	/// @return 
	GLenum usageToGLenum(const VertexBuffer::EUsage usage) {
		switch (usage)
		{
		case VertexBuffer::EUsage::Static :
			return GL_STATIC_DRAW;
		case VertexBuffer::EUsage::Dynamic :
			return GL_DYNAMIC_DRAW;
		case VertexBuffer::EUsage::Stream :
			return GL_STREAM_DRAW;
		}

		LOG_ERR("Unknown VertexBuffer usage!");
		return GL_STATIC_DRAW;
	}

	/// @internal
	/// @brief Возвращает кол-во компонентов атрибута.
	/// @param type Тип данных атрибута.
	/// @return кол-во компонентов атрибута.
	constexpr uint8_t getShaderDataComponentsCount(const ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Int:
			return 1;		
		case ShaderDataType::Float2:
		case ShaderDataType::Int2:
			return 2;		
		case ShaderDataType::Float3:
		case ShaderDataType::Int3:
			return 3;		
		case ShaderDataType::Float4:
		case ShaderDataType::Int4:
			return 4;		
		}

		LOG_ERR("Unknown shader data type!");
		return 0;
	}
	
	/// @internal
	/// @brief Возвращает размер атрибута в байтах.
	/// @param type Тип данных атрибута.
	/// @return Размера атрибута в байтах.
	constexpr size_t getShaderDataComponentsSize(const ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			return sizeof(GLfloat) * getShaderDataComponentsCount(type);
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return sizeof(GLint) * getShaderDataComponentsCount(type);
		}

		LOG_ERR("Unknown shader data type!");
		return 0;
	}

	/// @brief Переводит тип данных атрибута `ShaderDataType` в тип OpenGL.
	/// @param type Тип данных атрибута.
	/// @return Соответствующий `ShaderDataType` тип OpenGL.
	constexpr unsigned int getOpenGLTypeFromShaderDataType(const ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
			return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		}

		LOG_ERR("Unknown shader data type!");
		return 0;
	}

	BufferElement::BufferElement(const ShaderDataType type) 
		: type(type)
		, size(getShaderDataComponentsSize(type))
		, componentCount(getShaderDataComponentsCount(type))
		, componentType(getOpenGLTypeFromShaderDataType(type))
		, offset(0)
	{}
	
	VertexBuffer::VertexBuffer(
		const void* 		data,
		const size_t		size,
		BufferLayout 		layout,
		const EUsage		usage
	)	: m_layout(std::move(layout))
	 {
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, usageToGLenum(usage));
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& rhs) 
		: m_layout(std::move(rhs.m_layout))
	{
		m_id = rhs.m_id;
		rhs.m_id = 0;
	} 	

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& rhs) {
		m_id = rhs.m_id;
		rhs.m_id = 0;

		return *this;
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_id);
	}

	void VertexBuffer::bind() const noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() noexcept {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} // namespace Engine
