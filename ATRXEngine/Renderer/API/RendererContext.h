#pragma once

namespace ATRX
{
	class RendererContext
	{
	public:
		virtual bool OnInit() = 0;
		virtual void OnDestroy() = 0;
	};
}
