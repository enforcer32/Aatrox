#pragma once

#include "ATRXEngine/Core/Window.h"
#include "ATRXEngine/Events/KeyEvents.h"
#include "ATRXEngine/Core/DeltaTime.h"
#include "ATRXEngine/Renderer/Renderer.h"

namespace ATRX
{
	struct EngineProperties
	{
		WindowProperties WindowProperties;
	};

	class ATRX_API Engine
	{
	public:
		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(const Engine&) = delete;
		Engine& operator=(Engine&&) = delete;
		virtual ~Engine();

		void OnStart();
		void OnShutdown();
		virtual bool OnInit() = 0;
		virtual void OnDestroy() = 0;
		virtual void OnUpdate(DeltaTime dt) = 0;
		virtual void OnRender(DeltaTime dt) = 0;

		static Engine* Get();
		const EngineProperties& GetProperties() const;
		const Window& GetWindow() const;

	protected:
		Engine(const EngineProperties& engineProps);
		virtual void OnKeyPressEvent(KeyPressEvent& ev);

	protected:
		EngineProperties m_Properties;

	private:
		static Engine* s_Instance;
		bool m_Initialized = false, m_Running = false;
		std::unique_ptr<Window> m_Window;
		std::shared_ptr<Renderer> m_Renderer;
	};
}
