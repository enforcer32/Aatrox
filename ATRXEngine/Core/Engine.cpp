#include "ATRXEngine/Core/Engine.h"
#include "ATRXEngine/Core/Assertion.h"
#include "ATRXEngine/Core/Logger.h"
#include "ATRXEngine/Core/Timer.h"
#include "ATRXEngine/Core/Memory.h"
#include "ATRXEngine/Event/EventBus.h"
#include "ATRXEngine/Input/Input.h"

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

		if(!MemoryAllocator::OnInit())
			ATRX_LOG_CRITICAL("ATRXEngine Failed to Initialize MemoryAllocator!");

		m_Window = Window::CreateInstance();
		if (!m_Window->OnInit(m_Properties.WindowProperties))
			ATRX_LOG_CRITICAL("ATRXEngine Failed to Initialize Window!");

		if(!EventBus::OnInit())
			ATRX_LOG_CRITICAL("ATRXEngine Failed to Initialize EventBus!");

		if (!Input::OnInit())
			ATRX_LOG_CRITICAL("ATRXEngine Failed to Initialize Input!");
	}

	Engine::~Engine()
	{
		if (m_Initialized)
		{
			Input::OnDestroy();
			EventBus::OnDestroy();
			m_Window->OnDestroy();
			MemoryAllocator::OnDestroy();
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
		MemoryAllocator::Allocate(1024, AllocateType::Array);
		MemoryAllocator::Allocate(1024, AllocateType::Array);
		MemoryAllocator::Allocate(4096, AllocateType::Unknown);
		MemoryAllocator::DebugPrintStatistics();

		// Events
		EventBus::Subscribe<KeyPressEvent>(this, &Engine::OnKeyPressEvent);

		while (m_Running)
		{
			m_Window->OnUpdate();
			OnUpdate(1);
			Input::OnUpdate();
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

	void Engine::OnKeyPressEvent(KeyPressEvent& ev)
	{
		ATRX_LOG_INFO("PRESS FIRED({})!\n", (int)ev.GetKey());

		if (ev.GetKey() == KeyCode::Escape)
		{
			ATRX_LOG_INFO("Quitting Engine....");
			OnShutdown();
		}
	}
}
