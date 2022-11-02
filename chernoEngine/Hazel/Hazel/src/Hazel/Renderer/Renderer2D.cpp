#include "hzpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"

#include "glm/gtc/matrix_transform.hpp"

namespace Hazel
{
	//TODO:Add Vertex Content 
	struct Vertex
	{

	};
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		//TODO:color ,texid
		float TexIndex ;
		float TilingFactor;
	};

	

	struct  Renderer2DData
	{
		const uint32_t  MaxQuads = 1000;
		const uint32_t  MaxVertics = MaxQuads * 4;
		const uint32_t  MaxIndeics = MaxQuads * 6;
		static const uint32_t  MaxTextureSlots = 32; //TODO;renderer caps


		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Texture2D> WhitBlock ;
		Ref<Shader> TextureShader ;

		uint32_t quadIndexCount = 0; //统计多少正方形被画了
		QuadVertex* QuadVertexBufferBase =nullptr; //begin of qiadvertexArray 
		QuadVertex* QuadVertexBufferPtr = nullptr;	//now of quadvertexArray

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;// 0  is for white texture; and we will traverse the slots ,one slot set a texture coord?

		glm::vec4 quadVertexPosition[4];
	};

	static Renderer2DData s_Data ; //又成整个结构了..,,这样的话整个结构都是静态,其成员就不能直接初始化而是应该在外初始化
	//将指针作为静态变量而不是将整个结构作为静态,这样方便管理(能够回收内存),只占一个指针的内存

	void Renderer2D::Init()
	{

		HZ_PROFILE_FUNCTION();


		s_Data.QuadVA = (Hazel::VertexArray::Create());



		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertics * sizeof(QuadVertex));

		s_Data.QuadVB->SetLayout({
			{Hazel::ShaderDataType::Float3 ,"a_Position"},
			{Hazel::ShaderDataType::Float4 ,"a_Color"},
			{Hazel::ShaderDataType::Float2 ,"a_UVCoord"},
			{Hazel::ShaderDataType::Float ,"a_TexIndex"},
			{Hazel::ShaderDataType::Float ,"a_TilingFactor"}
			});

		s_Data.QuadVA->AddVertexBuffer(s_Data.QuadVB);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertics]; //记得这是指向第一个元素

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndeics];

		{
			uint32_t offset = 0;
			for (uint32_t i = 0; i < s_Data.MaxIndeics; i += 6)
			{
				quadIndices[i + 0] = offset + 0;
				quadIndices[i + 1] = offset + 1;
				quadIndices[i + 2] = offset + 2;

				quadIndices[i + 3] = offset + 2;
				quadIndices[i + 4] = offset + 3;
				quadIndices[i + 5] = offset + 0;

				offset += 4;
			}
		}

		Ref<IndexBuffer> SquraIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndeics);

		s_Data.QuadVA->SetIndexBuffer(SquraIB); //是实际上上传到了Gpu

		delete[] quadIndices;

		//TODO: one white pixel Texture set for Shader sample;
		s_Data.WhitBlock = Texture2D::Create(1, 1);
		uint32_t OnePixel = 0xffffffff; //因为是采样...采样不在乎你原图的..
		s_Data.WhitBlock->SetData(&OnePixel, 4);

		s_Data.TextureShader = Shader::Create("Assets/shader/texture.glsl");//TODO : texure2d renderer
		s_Data.TextureShader->Bind();



		int samples[s_Data.MaxTextureSlots];
		for ( int i = 0; i < s_Data.MaxTextureSlots; i++)
			samples[i] = i;

		s_Data.TextureShader->SetIntArray("u_Texture", samples,s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhitBlock;


		//rotate around about the center
		s_Data.quadVertexPosition[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.quadVertexPosition[1] = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.quadVertexPosition[2] = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		s_Data.quadVertexPosition[3] = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
	

	}

	
	void Renderer2D::BeginScene(const OrthographicCamera& Camera)
	{
		HZ_PROFILE_FUNCTION();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjection", Camera.GetViewProjectionMat());//我们

		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.quadIndexCount = 0;

		s_Data.TextureSlotIndex = 1; //begin
	
	}


	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();
		uint32_t datasize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;//是空指针的步长.
		s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase,datasize);


		Flush();

	}

	void Renderer2D::Flush()
	{
		HZ_PROFILE_FUNCTION();


		s_Data.TextureShader->Bind();
		s_Data.QuadVA->Bind();
		//TODO : Bind texture;
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.QuadVA,s_Data.quadIndexCount);


	}

	void Renderer2D::ShutDown()
	{
		HZ_PROFILE_FUNCTION();
	}


	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color/*= 0.f*/)
	{
		HZ_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color/*= 0.f*/)
	{
		HZ_PROFILE_FUNCTION();

		float textureIndex = 0.0f;
		float tilingFactor = 1.f;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f,0.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;


		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = {position.x + size.x, position.y, position.z};
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f,0.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y , position.z };
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f,1.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = { position.x , position.y + size.y , position.z };
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f,1.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.quadIndexCount += 6;

		/*s_Data.WhitBlock->Bind();
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.0f);
		s_Data.TextureShader->SetMat4("u_Transform", glm::mat4(1.0f));//我们需要为我们引擎的shader指定统一
		s_Data.QuadVA->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVA);
		*/

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor )
	{
		HZ_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, texture, tilingFactor,tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.f,1.f,1.f,1.f };

		float textureIndex = 0.f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) //if the texture  exist  in slots
		{
			
			if (*texture.get() == *s_Data.TextureSlots[i].get()) //get : get raw ptr?
			{
				textureIndex = (float) i;
				break;
			}
			

		}
		if (textureIndex == 0.f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex; // or a new slot
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}


		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f,0.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor= tilingFactor;
		
		s_Data.QuadVertexBufferPtr++;


		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, position.z };
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f,0.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y , position.z };
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f,1.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = { position.x , position.y + size.y , position.z };
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f,1.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.quadIndexCount += 6;
#if 0
		glm::mat4 transformMat =
			glm::translate(glm::mat4(1.f), position)
			*
			glm::scale(glm::mat4(1.0f), { size.x, size.y,1.0f });



		texture->Bind();

		s_Data.TextureShader->Bind();

		s_Data.TextureShader->SetFloat4("u_color", tintColor);//我们需要为我们引擎的shader指定统一
		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor); //cherno是一个float ... 算了就这样	 
		s_Data.TextureShader->SetMat4("u_Transform", transformMat);
		
#endif



		
		

	}

	void Renderer2D::DrawLine()
	{
		HZ_PROFILE_FUNCTION();


	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({position.x, position.y,0}, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();


		glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
			*
			glm::rotate(glm::mat4(1.f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) //换算成 弧度
			*
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.f));

		float textureIndex = 0.0f;
		float tilingFactor = 1.0f;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f,0.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;


		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f,0.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f,1.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f,1.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.quadIndexCount += 6;

// 
// 		glm::mat4 transformMat =
// 			glm::translate(glm::mat4(1.f), position)
// 			* glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0.f, 0.f, 1.f))
// 			* glm::scale(glm::mat4(1.0f), { size.x, size.y,1.0f });
// 		s_Data.WhitBlock->Bind();
// 
// 		s_Data.TextureShader->Bind();
// 		
// 		s_Data.TextureShader->SetFloat4("u_color", color);//我们需要为我们引擎的shader指定统一
// 		s_Data.TextureShader->SetFloat("u_TilingFactor", 1.f); //cherno是一个float ... 算了就这样	 
// 		s_Data.TextureShader->SetMat4("u_Transform", transformMat);



	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{

		DrawRotatedQuad({ position.x, position.y,0 }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();



		constexpr glm::vec4 color = { 1.f,1.f,1.f,1.f };

		float textureIndex = 0.f;
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) //if the texture  exist  in slots
		{

			if (*texture.get() == *s_Data.TextureSlots[i].get()) //get : get raw ptr?
			{
				textureIndex = (float)i;
				break;
			}


		}
		if (textureIndex == 0.f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex; // or a new slot
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}


		glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
			*
			glm::rotate(glm::mat4(1.f), glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f)) //it become radians ,so it was degree before
			*
			glm::scale(glm::mat4(1.0f),glm::vec3(size,0.f));


		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f,0.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

		s_Data.QuadVertexBufferPtr++;


		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f,0.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f,1.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f,1.f };//左下角定义	
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
		s_Data.QuadVertexBufferPtr++;

		s_Data.quadIndexCount += 6;



// 
// 		glm::mat4 transformMat =
// 			glm::translate(glm::mat4(1.f), position)
// 			*glm::rotate(glm::mat4(1.f),rotation,glm::vec3(0.f,0.f,1.f))
// 			*glm::scale(glm::mat4(1.0f), { size.x, size.y,1.0f });
// 
// 
// 
// 		texture->Bind();
// 
// 		s_Data.TextureShader->Bind();
// 
// 		s_Data.TextureShader->SetFloat4("u_color", tintColor);//我们需要为我们引擎的shader指定统一
// 		s_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor); //cherno是一个float ... 算了就这样	 
// 		s_Data.TextureShader->SetMat4("u_Transform", transformMat);
// 
// 		s_Data.QuadVA->Bind();
// 		RenderCommand::DrawIndexed(s_Data.QuadVA);

	}

}