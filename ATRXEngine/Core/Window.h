#pragma once

#include "ATRXEngine/Core/Typedef.h"

#include <string>
#include <memory>

namespace ATRX
{
	struct WindowProperties
	{
		std::string Name;
		uint32_t Width;
		uint32_t Height;
		uint32_t XPos;
		uint32_t YPos;
		bool VSync;
	};

	class ATRX_API Window
	{
	public:
		static std::unique_ptr<Window> CreateInstance();

		virtual ~Window() = default;
		virtual bool OnInit(const WindowProperties& props) = 0;
		virtual void OnDestroy() = 0;
		virtual void OnUpdate() = 0;

		virtual const WindowProperties& GetProperties() const = 0;
		virtual void SetProperties(const WindowProperties& props) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetVSync(bool vsync) = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const void* GetNativeWindow() const = 0;
	};
}
