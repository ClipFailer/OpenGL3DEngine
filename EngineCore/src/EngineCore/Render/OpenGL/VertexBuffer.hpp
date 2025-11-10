#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace Engine { 

	/**
	 * @brief Перечисление типов данных для вершинных атрибутов.
	 * 
	 * Используется при описании структуры вершины (BufferLayout).
	 * Каждое значение определяет кол-во компонентов и их тип.
	 * 
	 * Это перечисление не является типом OpenGL напрямую, но служит
	 * абстракцией, на основе которой движок вычисляет:
	 *  - количество компонент в атрибуте
	 *  - используемый тип (GL_FLOAT, GL_INT)
	 *  - размер данных в байтах
	 */
	enum class ShaderDataType {
		Float,		///< float
		Float2,		///< vec2 (2 float)
		Float3,		///< vec3 (3 float)	
		Float4,		///< vec4 (4 float)
		Int,		///< int
		Int2,		///< vec2 (2 int)
		Int3,		///< vec3 (3 int)
		Int4		///< vec4 (4 int)
	};

	/**
	 * @brief Структура, описывающая атрибут вершины.
	 * 
	 * Представляет собой атрибут вершины(например, позицию, цвет и т.д.). 
	 * в составе `BufferLayout`.
	 * 
	 * @see BufferLayout
	 */
	struct BufferElement {
		ShaderDataType 	type;				///< Тип данных атрибута `ShaderDataType`.
		uint32_t		componentType;		///< Тип компонента OpenGL (GLenum).
		size_t			componentCount;		///< Кол-во компонентов в атрибуте
		size_t			size;				///< Размер Атрибута в байтах.
		size_t			offset;				///< Смещение в байтах внутри структуры вершины.

		/// @brief Конструктор атрибута вершины.
		/// Конструктор получает тип данных структуры вершин
		/// и заполняет все поля, исходя из выбранного типа.
		/// @param type Тип данных структуры вершины.
		BufferElement(const ShaderDataType type);
	};

	class BufferLayout {
	public:
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elements(std::move(elements))
		{
			size_t offset = 0;
			m_stride = 0;
			for (auto& element : m_elements) {
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}

		const std::vector<BufferElement>& getElements() const noexcept { return m_elements; };
		size_t getStride() const noexcept { return m_stride; }

	private:
		std::vector<BufferElement>	m_elements;
		size_t 						m_stride;
	};

	/// @internal
	/// @brief Класс, инкапсулирующий реализацию объекта буфера вершин.
	class VertexBuffer {
	public:
		/**
		 * @internal
		 * @brief Определяет предполагаемый тип использования данных в буфере.
		 *
		 * Значение используется при создании буфера и передается в OpenGL
		 * (glBufferData) для выбора оптимальной стратегии хранения.
		 *
		 */ 
		enum class EUsage {
			Static, 	///< Данные почти не изменяются (по умолчанию). 
			Dynamic,	///< Данные изменяются регулярно.
			Stream		///< Данные обновляются каждый кадр.
		};

		/// @internal
		/// @brief Конструктор создаёт готовый VBO объект.
		/// @param data Данные буфера.
		/// @param size Размер данных буфера.
		/// @param usage Тип использования буфера (`EUsage`)
		/// @see EUsage
		VertexBuffer(
			const void* 	data,
			const size_t	size,
			BufferLayout	layout,
			const EUsage	usage = EUsage::Static
		);
		~VertexBuffer();

		/** 
		 * @internal
		 * @brief Перемещающий конструктор.
		 *
		 * Передаёт владение GPU-ресурсом (`m_id`) от правого объекта к левому.
		 * После перемещения `rhs` оказывается в валидном, но пустом состоянии:
		 * его буфер считается неактивным и не должен использоваться.
		 *
		 * @param rhs Объект `VertexBuffer`, ресурсы которого следует переместить.
		 */
		VertexBuffer(VertexBuffer&& rhs);

		/** 
		 * @internal
		 * @brief Перемещающий оператор присваивания.
		 *
		 * Передаёт владение GPU-ресурсом (`m_id`) от правого объекта к левому.
		 * После перемещения `rhs` оказывается в валидном, но пустом состоянии:
		 * его буфер считается неактивным и не должен использоваться.
		 *
		 * @param rhs Объект `VertexBuffer`, ресурсы которого следует переместить.
		 * @return Ссылка на текущий объект.
		 */
		VertexBuffer& operator=(VertexBuffer&&);

		VertexBuffer(const VertexBuffer&) 				= delete;
		VertexBuffer& operator=(const VertexBuffer&) 	= delete;

		/// @internal
		/// @brief Устанавливает буфер в контексте OpenGL для дальнейшей работы.
		void bind() const noexcept;

		/// @internal
		/// @brief Сбрасывает буфер из контекста OpenGL.
		static void unbind() noexcept;

		/// @internal
		/// @brief Возвращает `BufferLayout`
		/// @return Возвращает `BufferLayout`
		const BufferLayout& getLayout() const { return m_layout; }

	private:
		unsigned int 		m_id;
		BufferLayout		m_layout;
	};

} // namespace Engine

