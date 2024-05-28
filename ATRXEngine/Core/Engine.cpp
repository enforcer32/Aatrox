#include "ATRXEngine/Core/Engine.h"
#include "ATRXEngine/Core/Assertion.h"
#include "ATRXEngine/Core/Logger.h"
#include "ATRXEngine/Core/Timer.h"

namespace ATRX
{
	Engine* Engine::s_Instance = nullptr;

	Engine::Engine(const EngineProperties& engineProps)
	{
		ATRX_ASSERT(!s_Instance, "ATRXEngine Already Initialized!");
		s_Instance = this;
		m_Properties = engineProps;

		Logger::OnInit();

		m_Window = Window::CreateInstance();
		if (!m_Window->OnInit(m_Properties.WindowProperties))
			ATRX_LOG_CRITICAL("ATRXEngine Failed to Initialize Window!");

		Timer::OnInit();
	}

	Engine::~Engine()
	{
		if (m_Initialized)
		{
			m_Window->OnDestroy();
		}
	}

	void Engine::OnStart()
	{
		if (!OnInit())
			ATRX_LOG_CRITICAL("Engine OnInit Failed!");

		m_Initialized = true;
		m_Running = true;

		while (m_Running)
		{
			OnUpdate(1);
			m_Window->OnUpdate();
		}

		OnDestroy();
	}

	void Engine::OnShutdown()
	{
		m_Running = false;
	}

	Engine* Engine::Get()
	{
		return s_Instance;
	}

	const EngineProperties& Engine::GetProperties() const
	{
		return m_Properties;
	}

	const Window& Engine::GetWindow() const
	{
		return *m_Window;
	}
}
