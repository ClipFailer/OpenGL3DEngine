#pragma once

#include <cstdint>
#include <functional>
#include <array>

namespace Engine {

	
	
	enum class EventType {
		WindowResize = 0,
		WindowClose,

		KeyPress,
		MouseMove,
		MouseButton,

		EventCount
	};
	
	struct Event {
		virtual ~Event() = default;
		virtual EventType getType() const noexcept = 0;
	};

	using EventCallback = std::function<void(Event&)>;
	using EventCallbacksArray = std::array<
		EventCallback, 
		static_cast<size_t>(EventType::EventCount)
	>;

	class EventDispatcher {
	public:
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

		void dispatch(Event& e) {
			auto& callback = m_eventCallbacks[static_cast<size_t>(e.getType())];
			if (callback) {
				callback(e);
			}
		}

	private:
		EventCallbacksArray m_eventCallbacks;
	};

	struct EventMouseMove : public Event {
		EventMouseMove(const double _x, const double _y) : x(_x), y(_y) {

		}

		EventType getType() const noexcept override  { return type; }

		double x, y;
		static const EventType type = EventType::MouseMove;
	};

	struct EventWindowResize : public Event {
		EventWindowResize(const uint16_t _width, const uint16_t _height) 
			: width(_width)
			, height(_height) {

		}

		EventType getType() const noexcept override  { return type; }

		uint16_t width, height;
		static const EventType type = EventType::WindowResize;
	};

	struct EventCloseWindow : public Event {
		EventType getType() const noexcept override  { return type; }

		static const EventType type = EventType::WindowClose;
	};
	
} // namespace Engine
