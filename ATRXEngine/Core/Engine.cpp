#include "ATRXEngine/Core/Engine.h"
#include "ATRXEngine/Core/Assertion.h"
#include "ATRXEngine/Core/Logger.h"
#include "ATRXEngine/Core/Timer.h"
#include "ATRXEngine/Core/Memory.h"
#include "ATRXEngine/Event/EventBus.h"

namespace ATRX
{
	Engine* Engine::s_Instance = nullptr;

	Engine::Engine(const EngineProperties& engineProps)
	{
		ATRX_ASSERT(!s_Instance, "ATRXEngine Already Initialized!");
		s_Instance = this;
		m_Properties = engineProps;

		Logger::OnInit();
		Timer::OnInit();

		if(!MemoryManager::OnInit())
			ATRX_LOG_CRITICAL("ATRXEngine Failed to Initialize MemoryManager!");

		m_Window = Window::CreateInstance();
		if (!m_Window->OnInit(m_Properties.WindowProperties))
			ATRX_LOG_CRITICAL("ATRXEngine Failed to Initialize Window!");

		if(!EventBus::OnInit())
			ATRX_LOG_CRITICAL("ATRXEngine Failed to Initialize m_EventManager!");
	}

	Engine::~Engine()
	{
		if (m_Initialized)
		{
			EventBus::OnDestroy();
			m_Window->OnDestroy();
			MemoryManager::OnDestroy();
			Timer::OnDestroy();
			Logger::OnDestroy();
		}
	}

	void Engine::OnStart()
	{
		if (!OnInit())
			ATRX_LOG_CRITICAL("Engine OnInit Failed!");

		m_Initialized = true;
		m_Running = true;

		// TMP
		MemoryManager::Allocate(1024, AllocateType::Array);
		MemoryManager::Allocate(1024, AllocateType::Array);
		MemoryManager::Allocate(4096, AllocateType::Unknown);
		MemoryManager::DebugPrintStatistics();

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
