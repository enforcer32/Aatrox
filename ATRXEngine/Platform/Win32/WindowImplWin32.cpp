#include "ATRXEngine/Platform/Win32/WindowImplWin32.h"
#include "ATRXEngine/Core/Logger.h"
#include "ATRXEngine/Input/Input.h"

#include <windowsx.h>

namespace ATRX
{
	LRESULT WindowImplWin32WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		// Window Events
		case WM_CLOSE:
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ERASEBKGND:
			return 1;
		case WM_SIZE:
		{
			RECT size;
			GetClientRect(hwnd, &size);
			uint32_t width = size.right - size.left;
			uint32_t height = size.bottom - size.top;
			//Event WindowResize
			break;
		}

		// Input Events
		case WM_KEYUP:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_SYSKEYDOWN:
		{
			uint8_t pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
			KeyEventType type = (pressed) ? KeyEventType::Press : KeyEventType::Release;
			KeyCode key = (KeyCode)wParam;
			Input::OnKeyEvent(type, key);
			break;
		}

		// Mouse Events
		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MBUTTONDOWN:
		{
			uint8_t pressed = (msg == WM_LBUTTONUP || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN);
			MouseEventType type = (pressed) ? MouseEventType::Press : MouseEventType::Release;
			MouseCode button = MouseCode::ButtonLast;
			if (msg == WM_LBUTTONUP || msg == WM_LBUTTONDOWN)
				button = MouseCode::ButtonLeft;
			if (msg == WM_RBUTTONUP || msg == WM_RBUTTONDOWN)
				button = MouseCode::ButtonRight;
			if (msg == WM_MBUTTONUP || msg == WM_MBUTTONDOWN)
				button = MouseCode::ButtonMiddle;
			if(button != ButtonLast)
				Input::OnMouseButtonEvent(type, button);
			break;
		}
		case WM_MOUSEMOVE:
		{
			int32_t xPos = GET_X_LPARAM(lParam);
			int32_t yPos = GET_Y_LPARAM(lParam);
			Input::OnMouseMoveEvent(MousePoint{ (double)xPos, (double)yPos });
			break;
		}
		case WM_MOUSEWHEEL:
		{
			int32_t zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			if (zDelta)
			{
				zDelta = (zDelta < 0) ? -1 : 1;
				Input::OnMouseScrollEvent(zDelta);
			}
			break;
		}
		default:
			break;
		}

		return DefWindowProcA(hwnd, msg, wParam, lParam);
	}

	std::unique_ptr<Window> Window::CreateInstance()
	{
		return std::make_unique<WindowImplWin32>();
	}

	bool WindowImplWin32::OnInit(const WindowProperties& props)
	{
		ATRX_LOG_INFO("ATRXWindow->Initializing...");
		ATRX_LOG_INFO("ATRXWindowImplWin32->Initializing...");

		m_WinPrivData.Name = props.Name;
		m_WinPrivData.Width = props.Width;
		m_WinPrivData.Height = props.Height;
		m_WinPrivData.XPos = props.XPos;
		m_WinPrivData.YPos = props.YPos;
		m_WinPrivData.VSync = props.VSync;
		m_WinPrivData.HInstance = GetModuleHandleA(0);

		if (!RegisterWindowClass())
		{
			ATRX_LOG_ERROR("ATRXWindowImplWin32->Error RegisterWindowClass!");
			return false;
		}

		if (!CreateWindowContext())
		{
			ATRX_LOG_ERROR("ATRXWindowImplWin32->Error CreateWindowContext!");
			return false;
		}

		if (!ShowTheWindow())
		{
			ATRX_LOG_ERROR("ATRXWindowImplWin32->Error ShowTheWindow!");
			return false;
		}

		ATRX_LOG_INFO("ATRXWindowImplWin32->Initialized!");
		ATRX_LOG_INFO("ATRXWindow->Initialized!");
		return m_Initialized = true;
	}

	void WindowImplWin32::OnDestroy()
	{
		if (m_Initialized)
		{
			ATRX_LOG_INFO("ATRXWindowImplWin32->Destroying...");
			ATRX_LOG_INFO("ATRXWindow->Destroying...");

			if (m_WinPrivData.HWnd)
			{
				UnregisterClass(m_WinPrivData.Name.c_str(), m_WinPrivData.HInstance);
				DestroyWindow(m_WinPrivData.HWnd);
				m_WinPrivData.HWnd = NULL;
			}

			ATRX_LOG_INFO("ATRXWindowImplWin32->Destroyed!");
			ATRX_LOG_INFO("ATRXWindow->Destroyed!");
			m_Initialized = false;
		}
	}

	void WindowImplWin32::OnUpdate()
	{
		// PollEvents
		ProcessMessages();
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

	bool WindowImplWin32::RegisterWindowClass()
	{
		ATRX_LOG_INFO("ATRXWindowImplWin32->Registering Window Class...");
		HICON icon = LoadIconA(m_WinPrivData.HInstance, IDI_APPLICATION);

		WNDCLASSEXA windowClass;
		windowClass.style = CS_DBLCLKS;
		windowClass.lpfnWndProc = WindowImplWin32WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = m_WinPrivData.HInstance;
		windowClass.hIcon = icon;
		windowClass.hIconSm = icon;
		windowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = m_WinPrivData.Name.c_str();
		windowClass.cbSize = sizeof(WNDCLASSEXA);
		
		if (!RegisterClassExA(&windowClass))
		{
			MessageBoxA(0, "Failed to RegisterWindowClass", "Error", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		ATRX_LOG_INFO("ATRXWindowImplWin32->Registered Window Class!");
		return true;
	}

	bool WindowImplWin32::CreateWindowContext()
	{
		ATRX_LOG_INFO("ATRXWindowImplWin32->Creating Window Context...");
		uint32_t windowWidth = m_WinPrivData.Width, windowHeight = m_WinPrivData.Height, windowX = m_WinPrivData.XPos, windowY = m_WinPrivData.YPos;
		uint32_t windowStyle = WS_CAPTION | WS_OVERLAPPED | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
		uint32_t windowStyleEx = WS_EX_APPWINDOW;

		// Adjust Client Area
		RECT borderSize{};
		AdjustWindowRectEx(&borderSize, windowStyle, 0, windowStyleEx);
		windowWidth += borderSize.right - borderSize.left;
		windowHeight += borderSize.bottom - borderSize.top;
		windowX += borderSize.left;
		windowY += borderSize.top;

		HWND handle = CreateWindowExA(windowStyleEx, m_WinPrivData.Name.c_str(), m_WinPrivData.Name.c_str(), windowStyle, windowX, windowY, windowWidth, windowHeight, NULL, NULL, m_WinPrivData.HInstance, nullptr);
		if (!handle)
		{
			MessageBoxA(0, "Failed to CreateWindowContext", "Error", MB_ICONEXCLAMATION | MB_OK);
			return false;
		}

		m_WinPrivData.HWnd = handle;
		ATRX_LOG_INFO("ATRXWindowImplWin32->Created! Window Context!");
		return true;
	}

	bool WindowImplWin32::ShowTheWindow()
	{
		uint32_t activate = 1;
		uint32_t showWindowFlags = activate ? SW_SHOW : SW_SHOWNOACTIVATE;
		ShowWindow(m_WinPrivData.HWnd, showWindowFlags);
		SetForegroundWindow(m_WinPrivData.HWnd);
		SetFocus(m_WinPrivData.HWnd);
		return true;
	}

	bool WindowImplWin32::ProcessMessages()
	{
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));
		while (PeekMessageA(&msg, m_WinPrivData.HWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg); // Get CHAR Down Events
			DispatchMessageA(&msg); // Dispatch to WindowProc
		}
		return true;
	}
}
