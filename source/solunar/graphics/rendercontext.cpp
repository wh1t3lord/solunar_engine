#include "graphicspch.h"
#include "graphics/rendercontext.h"

namespace solunar
{
	RenderContext* RenderContext::ms_instance = nullptr;

	void RenderContext::Init()
	{
		if (!ms_instance)
			ms_instance = mem_new<RenderContext>();
	}

	void RenderContext::Shutdown()
	{
		if (ms_instance)
		{
			mem_delete(ms_instance);
			ms_instance = nullptr;
		}
	}

	RenderContext& RenderContext::GetContext()
	{
		return *ms_instance;
	}

	void RenderContext::SetContext(RenderContext& ctx)
	{
		*ms_instance = ctx;
	}
}