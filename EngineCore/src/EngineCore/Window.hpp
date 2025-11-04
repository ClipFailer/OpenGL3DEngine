#pragma once

#include <cstdint>
#include <string>
#include <functional>

#include "EngineCore/Event.hpp"

struct GLFWwindow;

namespace Engine {

	using EventCallback = std::function<void(Event&)>;

	/**
	 * @brief Структура, хранящая информацию об окне.
	 * 
	 * Структура хранит такие данные окна как ширина окна,
	 * высота, название и т.д.
	 */
	struct WindowData {
		uint16_t 		width;
		uint16_t 		height;
		std::string 	name;
		EventCallback	eventCallback;
	};

	/**
	 * @brief Класс, представляющий окно приложения.
	 * 
	 * Класс `Window` инкапсулирует создание окна через GLFW,
	 * управление его событиями, обработку интерфейса через ImGui.
	 * 
	 * @note Копирование и перемещение запрещено.
	 */
	class Window {
	public:
		/**
		 * @brief Конструктор класса `Window`.
		 * 
		 * Конструктор инициализирует новый объект `Window`
		 * с необходимыми для его работы параметрами.
		 * 
		 * @param name название окна.
		 * @param width ширина окна.
		 * @param height высота окна.
		 * 
		 * @note Используется move для переданной строки `name`.
		 */
		Window(
			const std::string& 	name, 
			uint16_t 			width,
			uint16_t 			height
		);
		~Window();

		Window(const Window&) 				= delete;		/**< Копирование запрещено */
		Window& operator=(const Window&) 	= delete;		/**< Копирование запрещено */
		Window(Window&&)					= delete;		/**< Move-конструктор запрещен */
		Window& operator=(Window&&)			= delete;		/**< Move-копирование запрещено */

		/**
		 * @brief Обновляет окно и интерфейс ImGui.
		 * 
		 * Метод должен вызываться каждый кадр. Он выполняет необходимые настройки,
		 * изменения, обработку событий окна.
		 */
		void update();

		/**
		 * @brief Возвращает ширину окна в пикселях.
		 * @return Ширина окна в пикселях (uint16_t)
		 */
		uint16_t getWidth() const noexcept { return m_data.width; }

		/**
		 * @brief Возвращает высоту окна в пикселях.
		 * @return Высота окна в пикселях (uint16_t)
		 */
		uint16_t getHeight() const noexcept { return m_data.height; }

		/**
		 * @brief Устанавливает callback-функцию для обработки событий окна.
		 * 
		 * Переданная функция будет вызвана при возникновении событий,
		 * таких как нажатие клавиши, движение мыши, изменение размера окна,
		 * закрытие окна и т.д.
		 * 
		 * @param [in] callback Функция-обработчик std::function<void(@ref Event&)
		 * , принимающая ссылку на объект.
		 * 
		 * @see Event, EventType
		 */
		void setEventCallback(const EventCallback& callback);

	private:
		int8_t init();
		int8_t shutdown();

		GLFWwindow*		m_id 			= nullptr;
		WindowData		m_data;
		float 			m_bgColor[4]	= {0.f, 0.f, 0.f, 1.f};
	};

} // namespace Engine 
