#include "Engine/Application.hpp"

#include <iostream>

#include "Engine/Log.hpp"
#include "Engine/Window.hpp"

namespace Engine {
	Application::Application(
		const std::string& 	title,
		const unsigned int 	windowWidth,
		const unsigned int 	windowHeight
	) {
		LOG_INFO("Starting application.");

		m_pWindow = std::make_unique<Window>(title, windowWidth, windowHeight);
		m_pWindow->setEventCallback(
			[](Event& event) {
				LOG_INFO("[Event] changed window size to {0}x{1}", event.width, event.height);
			}
		);
	};

	Application::~Application() {
		LOG_INFO("Closing application.");
	};

	int Application::run() {

		while(true) {
			m_pWindow->update();
			update();
		}

		return 0;
	};

	void Application::update() {

	};
} // namespace Engine
