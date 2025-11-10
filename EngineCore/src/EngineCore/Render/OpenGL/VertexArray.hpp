#pragma once 

#include <initializer_list>

namespace Engine {

	class VertexBuffer;
	
	/// @internal
	/// @brief Класс, инкапсулирующий реализацию объекта VAO.
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(VertexArray&&) 				noexcept;
		VertexArray& operator=(VertexArray&&) 	noexcept;

		VertexArray(const VertexArray&) 			= delete;
		VertexArray& operator=(const VertexArray&) 	= delete;

		/// @internal
		/// @brief Добавляет вершинный буфер к VAO и настраивает формат атрибутов вершин.
		///
		/// Функция привязывает переданный VertexBuffer и, опираясь на его BufferLayout,
		/// вызывает glEnableVertexAttribArray и glVertexAttribPointer для каждого
		/// атрибута. Таким образом VAO запоминает, как интерпретировать данные вершин.
		///
		/// @param vertexBuffer Вершинный буфер, содержащий данные и их раскладку.
		void addBuffer(const VertexBuffer& vertexBuffer);
		void bind() const noexcept;
		static void unbind() noexcept;

	private:
		unsigned int m_id 				= 0;
		unsigned int m_elements_count 	= 0;
	};

} // namespace Engine

