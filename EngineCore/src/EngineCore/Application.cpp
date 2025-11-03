#include "EngineCore/Application.hpp"

#include "EngineCore/Log.hpp"
#include "EngineCore/Window.hpp"

namespace Engine {

	Application::Application() {
        LOG_INFO("Starting application");
	};

	Application::~Application() {
        LOG_INFO("Closing application");
	};

	int8_t Application::run(
		uint16_t			windowWidth,
		uint16_t			windowHeight,
		const std::string&  windowTitle
	) {
        m_pWindow = std::make_unique<Window>(
            windowTitle,
            windowWidth,
            windowHeight
        );

        m_eventDispatcher.addListener<EventMouseMove>(
            [](EventMouseMove& e) { 
                LOG_INFO("[Event] mouse moved to {0}x{1}", e.x, e.y);
            }
        );

        m_eventDispatcher.addListener<EventWindowResize>(
            [](EventWindowResize& e) { 
                LOG_INFO("[Event] Changed window size to {0}x{1}", e.width, e.height);
            }
        );

        m_eventDispatcher.addListener<EventCloseWindow>(
            [&](EventCloseWindow& e) { 
                m_bCloseWindow = true;
            }
        );

        m_pWindow->setEventCallback(
            [&](Event& event) {
                m_eventDispatcher.dispatch(event);
            }
        );

        while (!m_bCloseWindow) {
            m_pWindow->update();
            update();
        }
        
        return 0;
	};
}