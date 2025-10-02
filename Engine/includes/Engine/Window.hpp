#pragma once

#include <functional>
#include <string>

#include "Engine/Event.hpp"

class GLFWwindow;

namespace Engine {

	using EventCallbackFn = std::function<void(Event&)>;

	class Window {
	public:

		
		Window(
			const std::string& title,
			const unsigned int width,
			const unsigned int height
		);
		~Window();

		Window(const Window&) 				= delete;
		Window& operator=(const Window&)	= delete;
		Window(Window&&) 					= delete;
		Window& operator=(Window&&)			= delete;

		void update();
		unsigned int getWidth() const { return m_data.m_width; };
		unsigned int getHeight() const { return m_data.m_height; };

		void setEventCallback(const EventCallbackFn& callback) {
			m_data.eventCallbackFn = callback;
		};

	private:	
		int init();
		void shutdown();

		struct WindowData {
			std::string 		m_title;
			unsigned int 		m_width;
			unsigned int 		m_height;
			EventCallbackFn		eventCallbackFn;
		};

		GLFWwindow* 	m_pWindow	= nullptr;
		WindowData		m_data;
	};
} // namespace Engine
