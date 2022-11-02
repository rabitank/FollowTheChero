#include "hzpch.h"
#include "OpenGlVertexArray.h"
#include "Glad/glad.h"

namespace Hazel
{
	static GLenum ShaderDataTypeToGlType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float: return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat3: return GL_FLOAT;
		case ShaderDataType::Mat4: return GL_FLOAT;
		case ShaderDataType::Int: return GL_INT;
		case ShaderDataType::Int2: return GL_INT;
		case ShaderDataType::Int3: return GL_INT;
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Bool: return GL_BOOL;
		case ShaderDataType::None: return 0;
		}
		HZ_CORE_ASSERT(false, "Not Defined Type in ShaderDatatype ");
		return 0;
	}


	OpenGlVertexArray::OpenGlVertexArray()
	{
		HZ_PROFILE_FUNCTION();
		glGenVertexArrays(1, &m_rendererID); //不Gen就会隐式地使用默认的vao,总之没事
	}

	OpenGlVertexArray::~OpenGlVertexArray()
	{
		glDeleteVertexArrays(1, &m_rendererID);
	}

	void OpenGlVertexArray::Bind()  
	{
		HZ_PROFILE_FUNCTION(); //这里的另一个好处是可以看到绑定次数.
		glBindVertexArray(m_rendererID);
	}

	void OpenGlVertexArray::UnBind()
	{
		HZ_PROFILE_FUNCTION();
		glBindVertexArray(0);

	}

	void OpenGlVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexbuffer)
	{
		HZ_PROFILE_FUNCTION();
		HZ_CORE_ASSERT(vertexbuffer->GetLayout().GetLayoutElement().size(),"VertexBuffer didn't add any Layout yet")

		glBindVertexArray(m_rendererID);
		vertexbuffer->Bind();
		const auto& Layout = vertexbuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : Layout) //将应用自己的Layout 和 被添加到顶点数组事件绑定起来....为了省资源?
		{
			//倒数第二个参数:每个顶点之间的字节偏移量(顶点大小), //实际意义上 类型,组件数量 两个参数
			//倒数第一个参数:偏移量,或者说pointer指针的从0到该属性起始位的移位(就是属性的指针啊....)
			glEnableVertexAttribArray(index);//启用零号顶点属性
			glVertexAttribPointer(index, element.GetElementCount(), ShaderDataTypeToGlType(element.type), element.ifNormalized ? GL_TRUE : GL_FALSE, Layout.GetStride(), (const void*)element.offset);
			index++;
		}
		m_vertexBuffers.push_back(vertexbuffer);
	}

	void OpenGlVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexbuffer)
	{
		HZ_PROFILE_FUNCTION();
		indexbuffer->Bind();

		m_indexBuffer=indexbuffer;

	}



}


