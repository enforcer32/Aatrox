#pragma once

#include "ATRXEngine/Core/Window.h"

#include <Windows.h>

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
		bool RegisterWindowClass();
		bool CreateWindowContext();
		bool ShowTheWindow();
		bool ProcessMessages();

	private:
		struct WindowPrivateData
		{
			std::string Name;
			uint32_t Width;
			uint32_t Height;
			uint32_t XPos;
			uint32_t YPos;
			bool VSync;

			HINSTANCE HInstance; // Application Instance
			HWND HWnd; // Window Handle
		};

		bool m_Initialized = false;
		void* m_NativeWindow = nullptr;
		WindowPrivateData m_WinPrivData;
	};
}
