#include <iostream>

#include <ATRXEngine/Core/Logger.h>
#include <ATRXEngine/Core/Engine.h>

namespace ATRX
{
	class ATRXSandbox : public Engine
	{
	public:
		ATRXSandbox(const EngineProperties& engineProps)
			: Engine(engineProps)
		{
		}

		virtual bool OnInit() override
		{
			ATRX_LOG_INFO("ATRXSandbox->Initialized!");
			return true;
		}

		virtual void OnDestroy() override
		{
			ATRX_LOG_INFO("ATRXSandbox->Destroyed!");
		}

		virtual void OnUpdate(DeltaTime dt) override
		{
		}

		virtual void OnRender(DeltaTime dt) override
		{
		}
	};
}

int main()
{
	ATRX::WindowProperties windowProperties;
	windowProperties.Name = "ATRXSandbox";
	windowProperties.Width = 640;
	windowProperties.Height = 480;
	windowProperties.XPos = 100;
	windowProperties.YPos = 100;
	windowProperties.VSync = false;

	ATRX::EngineProperties engineProperties;
	engineProperties.WindowProperties = windowProperties;

	ATRX::ATRXSandbox sandbox(engineProperties);
	sandbox.OnStart();
	return 0;
}
