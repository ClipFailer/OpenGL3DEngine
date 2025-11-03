#pragma once 

#include <cstdint>
#include <string>
#include <memory>

#include "EngineCore/Event.hpp"

namespace Engine {


	class Application {
	public:
		Application();
		~Application();

		Application(const Application&)				= delete;
		Application(Application&&)					= delete;
		Application& operator=(const Application&)	= delete;
		Application& operator=(Application&&)		= delete;

		virtual int8_t run(
			uint16_t			windowWidth, 
			uint16_t			windowHeight, 
			const std::string&	windowTitle
		);

		virtual void update() {};
	
	private:
		std::unique_ptr<class Window>	m_pWindow;
		EventDispatcher 				m_eventDispatcher;
		bool 							m_bCloseWindow 		= false;
	};

}