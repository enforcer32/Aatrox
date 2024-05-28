#pragma once

#include "ATRXEngine/Core/Window.h"

namespace ATRX
{
	class WindowImplWin32 : public Window
	{
	public:
		~WindowImplWin32() = default;

		virtual bool OnInit(const WindowProperties& props) override;
		virtual void OnDestroy() override;
		virtual void OnUpdate() override;

		virtual const WindowProperties& GetProperties() const override;
		virtual void SetProperties(const WindowProperties& props) override;
		virtual bool IsVSync() const override;
		virtual void SetVSync(bool vsync) override;
		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual const void* GetNativeWindow() const override;

	private:
		struct WindowPrivateData
		{
			std::string Name;
			uint32_t Width;
			uint32_t Height;
			bool VSync;
		};

		bool m_Initialized;
		void* m_NativeWindow;
		WindowPrivateData m_WinPrivData;
	};
}
