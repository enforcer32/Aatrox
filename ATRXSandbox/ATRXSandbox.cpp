#include <iostream>

#include <ATRXEngine/Core/Logger.h>
#include <ATRXEngine/Core/Assertion.h>
#include <ATRXEngine/Core/Window.h>

int main()
{
	ATRX::Logger::Init();
	ATRX_LOG_INFO("ATRXSandbox");

	ATRX::WindowProperties windowProperties;
	windowProperties.Name = "ATRXSandbox";
	windowProperties.Width = 640;
	windowProperties.Height = 480;
	windowProperties.XPos = 100;
	windowProperties.YPos = 100;
	windowProperties.VSync = false;

	std::unique_ptr<ATRX::Window> window = ATRX::Window::CreateInstance();
	window->OnInit(windowProperties);

	while (true)
	{
		window->OnUpdate();
	}

	return 0;
}
