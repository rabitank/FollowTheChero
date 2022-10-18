#pragma once


//extern 
class IndexBuffer;

//extern 
class Shader;

namespace Hazel
{

	enum class RendererAPI
	{
		None=0,OpenGl,

	};

	class Renderer
	{
	public:

		static RendererAPI s_rendererAPI;

		inline static RendererAPI GetAPI() { return s_rendererAPI; };

		virtual void Drawer(const IndexBuffer& ib, const Shader& shader)const =0;//const VertexArray& va
		virtual void clear() const =0;






	};

}


