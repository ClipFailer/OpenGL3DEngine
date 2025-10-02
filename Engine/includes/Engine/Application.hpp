#pragma once

#include <string>
#include <memory>


namespace Engine {

	class Window;

	class Application {
	public:
		Application(
			const std::string& 	title,
			const unsigned int 	windowWidth,
			const unsigned int 	windowHeight
		);
		virtual ~Application();

		Application(const Application&) 			= delete;
		Application& operator=(const Application&)	= delete;
		Application(Application&&) 					= delete;
		Application& operator=(Application&&)		= delete;

		virtual int run();

		virtual void update();

	private:
		std::unique_ptr<Engine::Window>		m_pWindow;
	};
} // namespace Engine
