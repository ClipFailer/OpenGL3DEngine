#pragma once 

#include <cstdint>
#include <string>
#include <memory>

#include "EngineCore/Event.hpp"

namespace Engine {

	/**
	 * @brief Класс, представляющий приложение.
	 * 
	 * Класс отвечает за:
	 * - Создание окна `Window`
	 * - Подписка и обработка событий (движение курсора, нажатие клавиш и т.д.)
	 * - Основной цикл обновления окна и логики приложения
	 */
	class Application {
	public:
		Application();											/**< Конструктор класса */
		~Application();											/**< Деструктор класса */

		Application(const Application&)				= delete; 	/**< Копирование запрещено */
		Application(Application&&)					= delete;	/**< Копирование запрещено */
		Application& operator=(const Application&)	= delete;	/**< Move-конструктор запрещен */
		Application& operator=(Application&&)		= delete;	/**< Move-копирование запрещено */

		/**
		 * @brief Запускает приложение.
		 * 
		 * Метод начинает работу приложения: 
		 * - Создаёт экземпляр окна
		 * - Подписывается на события и обрабатывает их
		 * - Запускает основной цикл программы
		 * 
		 * @param windowWidth ширина окна в пикселях.
		 * @param windowHeight высота окна в пикселях.
		 * @param windowTitle название окна.
		 * @return Код завершения (0 - успешно).
		 */
		virtual int8_t run(
			uint16_t			windowWidth, 
			uint16_t			windowHeight, 
			const std::string&	windowTitle
		);

		/**
		 * @brief Обновляет состояние приложения.
		 * 
		 * Виртуальный метод, который можно переопределить в классах наследниках.
		 * По-умолчанию ничего не делает.
		 * 
		 * @note Метод вызывается каждый кадр в основном цыкле в методе `run()`.
		 */
		virtual void update() {};
	
	private:
		std::unique_ptr<class Window>	m_pWindow;
		EventDispatcher 				m_eventDispatcher;
		bool 							m_bCloseWindow 		= false;
	};

}