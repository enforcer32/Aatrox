#pragma once

#include "ATRXEngine/Renderer/RendererBackend.h"

namespace ATRX
{
	class Renderer
	{
	public:
		bool OnInit();
		void OnDestroy();

		bool DrawFrame(RenderPacket packet);

	private:
		bool m_Initialized = false;
		std::unique_ptr<RendererBackend> m_RendererBackend;
	};
}
