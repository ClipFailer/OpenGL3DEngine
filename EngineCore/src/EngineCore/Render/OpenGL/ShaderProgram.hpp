#pragma once

namespace Engine {
	
	/**
	 * @internal
	 * @brief Класс, инкапслулирующий шейдерную программу.
	 * 
	 * Позволяет:
	 * - Компилировать вершиный и фрагментный шейдеры.
	 * - Использовать шейдер для отрисовки.
	 * - Устанавливать uniform-переменные.
	 */
	class ShaderProgram {
	public:
		/**
		 * @internal
		 * @brief Конструктор класса
		 * 
		 * Конструктор принемает две строки `const char*` с кодом
		 * шейдеров.
		 * 
		 * @param [in] vertexShaderSource Код вершинного шейдера.
		 * @param [in] FragmentShaderSource Код фрагментного шейдера.
		 */
		ShaderProgram(
			const char* vertexShaderSource,
			const char* FragmentShaderSource
		);
		ShaderProgram(ShaderProgram&&);
		ShaderProgram& operator=(ShaderProgram&&);
		~ShaderProgram();
		
		ShaderProgram()									= delete;
		ShaderProgram(const ShaderProgram&) 			= delete;
		ShaderProgram& operator=(const ShaderProgram&) 	= delete;

		/// @internal
		/// @brief Активирует шейдерную программу.
		void bind() const noexcept;

		///@internal
		/// @brief Деактивирует шейдерную программу.
		static void unbind() noexcept;

		/// @internal
		/// @brief Возвращает состояние компиляции шейдеров.
		/// @return Состояние компиляции (true - успешно).
		bool isCompiled() const noexcept { return m_isCompiled; }

	private:
		unsigned int	m_id 			= 0;
		bool 			m_isCompiled	= false;
	};

} // namespace Engine
