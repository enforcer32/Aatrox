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
		ATRX_ASSERT(!s_Instance, "ATRXEngine->Already Initialized!");
		s_Instance = this;
		m_Properties = engineProps;

		Logger::OnInit();
		Timer::OnInit();

		if(!MemoryAllocator::OnInit())
			ATRX_LOG_CRITICAL("ATRXEngine->Failed to Initialize MemoryAllocator!");

		m_Window = Window::CreateInstance();
		if (!m_Window->OnInit(m_Properties.WindowProperties))
			ATRX_LOG_CRITICAL("ATRXEngine->Failed to Initialize Window!");

		if(!EventBus::OnInit())
			ATRX_LOG_CRITICAL("ATRXEngine->Failed to Initialize EventBus!");

		if (!Input::OnInit())
			ATRX_LOG_CRITICAL("ATRXEngine->Failed to Initialize Input!");

		m_Renderer = std::make_shared<Renderer>();
		if(!m_Renderer->OnInit())
			ATRX_LOG_CRITICAL("ATRXEngine->Failed to Initialize Renderer!");
	}

	Engine::~Engine()
	{
		if (m_Initialized)
		{
			m_Renderer->OnDestroy();
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
			ATRX_LOG_CRITICAL("ATRXEngine->OnInit Failed!");

		// Events
		EventBus::Subscribe<KeyPressEvent>(this, &Engine::OnKeyPressEvent);

		m_Initialized = true;
		m_Running = true;

		ATRX_LOG_INFO("ATRXEngine->Started!");
		DeltaTime lastDeltaTime = 0;
		while (m_Running)
		{
			DeltaTime time = Timer::GetTimeSeconds();
			DeltaTime dt = time - lastDeltaTime;
			lastDeltaTime = time;

			if (dt > 0)
			{
				OnUpdate(dt);
				OnRender(dt);

				RenderPacket packet;
				packet.DeltaTime = dt;
				m_Renderer->DrawFrame(packet);
			}

			m_Window->OnUpdate();
			Input::OnUpdate();
		}

		OnDestroy();
	}

	void Engine::OnShutdown()
	{
		ATRX_LOG_INFO("ATRXEngine->Shutting Down!");
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
		ATRX_LOG_INFO("ATRXEngine->PRESS FIRED({})!\n", (int)ev.GetKey());

		if (ev.GetKey() == KeyCode::Escape)
		{
			ATRX_LOG_INFO("ATRXEngine->Quitting Engine....");
			OnShutdown();
		}
	}
}
