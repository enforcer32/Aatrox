#include "ATRXEngine/Platform/Win32/WindowImplWin32.h"
#include "ATRXEngine/Core/Logger.h"

namespace ATRX
{
	std::unique_ptr<Window> Window::CreateInstance()
	{
		return std::make_unique<WindowImplWin32>();
	}

	bool WindowImplWin32::OnInit(const WindowProperties& props)
	{
		m_WinPrivData.Name = props.Name;
		m_WinPrivData.Width = props.Width;
		m_WinPrivData.Height = props.Height;
		m_WinPrivData.VSync = props.VSync;
		ATRX_LOG_INFO("WindowImplWin32 Initialized!");
		return m_Initialized = true;
	}

	void WindowImplWin32::OnDestroy()
	{
		if (m_Initialized)
		{
			m_Initialized = false;
		}
	}

	void WindowImplWin32::OnUpdate()
	{
		// PollEvents & SwapBuffers
	}

	const WindowProperties& WindowImplWin32::GetProperties() const
	{
		return (WindowProperties&)m_WinPrivData;
	}

	void WindowImplWin32::SetProperties(const WindowProperties& props)
	{
		m_WinPrivData.Name = props.Name;
		m_WinPrivData.Width = props.Width;
		m_WinPrivData.Height = props.Height;
		m_WinPrivData.VSync = props.VSync;
	}

	bool WindowImplWin32::IsVSync() const
	{
		return m_WinPrivData.VSync;
	}

	void WindowImplWin32::SetVSync(bool vsync)
	{
		m_WinPrivData.VSync = vsync;
	}

	uint32_t WindowImplWin32::GetWidth() const
	{
		return m_WinPrivData.Width;
	}

	uint32_t WindowImplWin32::GetHeight() const
	{
		return m_WinPrivData.Height;
	}

	const void* WindowImplWin32::GetNativeWindow() const
	{
		return m_NativeWindow;
	}
}
