#include "graphicspch.h"
#include "graphics/rendercontext.h"

namespace engine
{
	RenderContext* RenderContext::ms_instance = nullptr;

	void RenderContext::init()
	{
		if (!ms_instance)
			ms_instance = mem_new<RenderContext>();
	}

	void RenderContext::shutdown()
	{
		if (ms_instance)
		{
			mem_delete(ms_instance);
			ms_instance = nullptr;
		}
	}

	RenderContext& RenderContext::getContext()
	{
		return *ms_instance;
	}

	void RenderContext::setContext(RenderContext& ctx)
	{
		*ms_instance = ctx;
	}
}