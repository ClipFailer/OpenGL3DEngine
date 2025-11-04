#pragma once

#include <cstdint>
#include <functional>
#include <array>

namespace Engine {

	/**
	 * @brief Перечисление, описывающее тип события `Event`
	 * 
	 * Описывает константы - типы событий окна:
	 * - @ref WindowResize - Изменение размера окна
	 * - @ref WindowClose - закрытие окна
	 * - @ref KeyPress - нажатие клавиш клавиатуры
	 * - @ref MouseMove - движение мыши
	 * - @ref MouseButton - нажатие кнопок мыши
	 */
	enum class EventType : uint8_t {
		WindowResize = 0,
		WindowClose,

		KeyPress,
		MouseMove,
		MouseButton,
	};

	constexpr uint8_t EventTypeCount = 5; 
	
	/**
	 * @brief Базовая структура события
	 */
	struct Event {
		virtual ~Event() = default;

		/** 
		 * @brief Чистый виртуальный метод получения типа события
		 */
		virtual EventType getType() const noexcept = 0;
	};

	using EventCallback = std::function<void(Event&)>;
	using EventCallbacksArray = std::array<
		EventCallback, 
		static_cast<size_t>(EventTypeCount)
	>;

	/**
	 * @brief Класс для управления обработкой событий приложения.
	 * 
	 * `EventDispatcher` позволяет:
	 * - Регистрировать обработчики событий для различных типов событий.
	 * - Вызывать обработчики при возникновении события.
	 * 
	 * Пример использования 
	 * @code
	 * // Создание обработчика событий.
	 * EventDispatcher eventDispatcher;
	 * 
	 * // Добавление обработчика события движения мыши.
	 * eventDispatcher.addListener<EventMouseMove>([](EventMouseMove& event) {
	 *     std::cout << "Mouse moved to " << event.x << "x" << event.y << std::endl;
	 * });
	 * @endcode
	 */
	class EventDispatcher {
	public:

		/**
		 * @brief Регистрирует обработчик для конкретного типа события.
		 * 
		 * @tparam TEvent Тип события, которое нужно обрабатывать.
		 * **Тип должен наследоваться от `Event` и иметь статическое поле `type`
		 * типа `EventType`.**
		 * @param callback Функция-обработчик события типа `std::function<void(TEvent&)>`.
		 * Функция вызывается при срабатывании соответствующего события.
		 */
		template<typename TEvent>
		void addListener(std::function<void(TEvent&)> callback) {
			size_t index = static_cast<size_t>(TEvent::type);

			auto baseCallback = [func = std::move(callback)](Event& e) {
				if (e.getType() == TEvent::type) {
					func(static_cast<TEvent&>(e));
				}
			};

			m_eventCallbacks[index] = std::move(baseCallback);
		}

		/**
		 * @brief Вызывает функцию-обработчик, которая зарегистрирована под соответствующим
		 * типом события.
		 * 
		 * @param event событие `Event`, для которого нужно вызвать соответствующий обработчик.
		 */
		void dispatch(Event& event) {
			auto& callback = m_eventCallbacks[static_cast<size_t>(event.getType())];
			if (callback) {
				callback(event);
			}
		}

	private:
		EventCallbacksArray m_eventCallbacks;
	};

	/**
	 * @brief Структура, описывающая событие нажатия кнопки мыши
	 */
	struct EventMouseMove : public Event {
		/**
		 * @brief Конструктор события
		 * 
		 * @param _x положения курсора мыши по оси X
		 * @param _y положения курсора мыши по оси Y
		 */
		EventMouseMove(const double _x, const double _y) : x(_x), y(_y) {

		}

		EventType getType() const noexcept override  { return type; }

		double x, y;
		static const EventType type = EventType::MouseMove;
	};

	/**
	 * @brief Структура, описывающая событие изменения размера окна
	 */
	struct EventWindowResize : public Event {
		/**
		 * @brief Конструктор события
		 * 
		 * @param _width ширина окна в пикселях
		 * @param _height высота окна в пикселях
		 */
		EventWindowResize(const uint16_t _width, const uint16_t _height) 
			: width(_width)
			, height(_height) {

		}

		EventType getType() const noexcept override  { return type; }

		uint16_t width, height;
		static const EventType type = EventType::WindowResize;
	};

	/**
	 * @brief Структура, описывающая событие закрытия окна
	 */
	struct EventCloseWindow : public Event {
		EventType getType() const noexcept override  { return type; }

		static const EventType type = EventType::WindowClose;
	};
	
} // namespace Engine
