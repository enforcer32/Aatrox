#include <iostream>

#include <ATRXEngine/Core/Logger.h>
#include <ATRXEngine/Core/Assertion.h>
#include <ATRXEngine/Core/Window.h>

int main()
{
	ATRX::Logger::Init();

	ATRX_LOG_INFO("INFO");

	std::unique_ptr<ATRX::Window> window = ATRX::Window::CreateInstance();
	window->OnInit({});

	return 0;
}
