#include <iostream>
#include <memory>

#include "EngineCore/Application.hpp"

class App : public Engine::Application {
public:
	virtual void update() override {
		
	}

private:
	int m_frame = 0;
};

int main() {
	auto app = std::make_unique<App>();

	int returnCode = app->run(600, 400, "Test");

	return returnCode;
}