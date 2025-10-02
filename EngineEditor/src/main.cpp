#include <iostream>
#include <memory>

#include "Engine/Application.hpp"

class EngineApp : public Engine::Application {
public:
	EngineApp(
		std::string  	title,
		unsigned int 	windowWidth,
		unsigned int 	windowHeight
	) : Application(title, windowWidth, windowHeight) {}

	virtual void update() override {
		// std::cout << "Update frame: " << m_frame << std::endl;
		// ++m_frame;
	}

private:
	int m_frame = 0;
};

int main() {
	auto pEngineApp = std::make_unique<EngineApp>("Engine", 800, 600);

	int returnCode = pEngineApp->run();

	std::cin.get();

	return 0;
}