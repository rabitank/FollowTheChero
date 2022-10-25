#include "hzpch.h"
#include "RenderCommand.h"

#include "Platform/openGl/OpenGlRendererAPI.h"

namespace Hazel
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGlRendererAPI();



}