#include "hzpch.h"
#include "OpenGlRendererAPI.h"
#include "glad/glad.h"

namespace Hazel
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         HZ_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       HZ_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          HZ_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: HZ_CORE_TRACE(message); return;
		}

		HZ_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGlRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r,color.g,color.b,color.a); //现在由glad提供该函数
	}

	void OpenGlRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlRendererAPI::DrawIndexed(const Ref<VertexArray>& VA,uint32_t indexCount)
	{
		uint32_t DrawindexCount = indexCount ?indexCount: VA->GetIndexBuffer()->GetCount();

		VA->Bind();
		//CHANGED:glDepthMask
		//glDepthMask(false);
		glDrawElements(GL_TRIANGLES, DrawindexCount, GL_UNSIGNED_INT, nullptr);//已经指定过索引序列了,用nullptr,drawelement使用定义的vao/默认的vao
		glBindTexture(GL_TEXTURE_2D, 0);//unbind texture
		//glDepthMask(true);
	}
	void OpenGlRendererAPI::DrawLines(const Ref<VertexArray>& VA,uint32_t vertexCount)
	{

		VA->Bind();
		glDrawArrays(GL_LINES, 0 ,vertexCount);//已经指定过索引序列了,用nullptr,drawelement使用定义的vao/默认的vao
	}

	void OpenGlRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

	void OpenGlRendererAPI::Init()
	{
		HZ_PROFILE_FUNCTION();

#ifdef HZ_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif // HZ_DEBUG
		//sbssss

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//设置混合方式 src_alpha * texture + 1-src_a*theCurrentImage

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH); //启用画线抗锯齿,抗锯齿对线宽有要求,

	}

	void OpenGlRendererAPI::SetViewPort(uint32_t x, uint32_t y,uint32_t width, uint32_t height)
	{
		glViewport(x,y,width,height);
		glBindTexture(GL_TEXTURE_2D, 0);//unbind texture
	}
	;


}
