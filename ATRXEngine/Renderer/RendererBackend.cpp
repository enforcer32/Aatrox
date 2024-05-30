#include "ATRXEngine/Renderer/RendererBackend.h"

namespace ATRX
{
	RendererBackendAPI RendererBackend::s_BackendAPI = RendererBackendAPI::None;

	// TEMPORARY
	std::unique_ptr<RendererBackend> RendererBackend::CreateInstance()
	{
		return nullptr;
	}
}
