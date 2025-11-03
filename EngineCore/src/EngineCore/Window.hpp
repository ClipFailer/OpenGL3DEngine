#pragma once

#include <cstdint>
#include <string>
#include <functional>

#include "EngineCore/Event.hpp"

struct GLFWwindow;

namespace Engine {

	using EventCallback = std::function<void(Event&)>;

	struct WindowData {
		uint16_t 		width;
		uint16_t 		height;
		std::string 	name;
		EventCallback	eventCallback;
	};

	class Window {
	public:
		Window(
			const std::string& 	name, 
			uint16_t 			width,
			uint16_t 			height
		);
		~Window();

		Window(const Window&) 				= delete;
		Window& operator=(const Window&) 	= delete;
		Window(Window&&)					= delete;
		Window& operator=(Window&&)			= delete;

		void update();

		uint16_t getWidth() const noexcept { return m_data.width; }
		uint16_t getHeight() const noexcept { return m_data.height; }

		void setEventCallback(const EventCallback& ca);

	private:
		int8_t init();
		int8_t shutdown();

		GLFWwindow*		m_id 	= nullptr;
		WindowData		m_data;
	};

} // namespace Engine 
