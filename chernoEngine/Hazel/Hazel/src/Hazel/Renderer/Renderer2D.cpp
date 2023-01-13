#include "hzpch.h"
#include "Renderer2D.h"
#include "RenderCommand.h"

#include "Hazel/Renderer/UniformBuffer.h"

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

		//editor-only
		int EntityID;
	};
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
			
		float Thickness;
		float Fade;

		//editor-only
		int EntityID;
	};

	

	struct  Renderer2DData
	{
		static const uint32_t  MaxQuads = 10000;
		static const uint32_t  MaxVertics = MaxQuads * 4;
		static const uint32_t  MaxIndeics = MaxQuads * 6;
		static const uint32_t  MaxTextureSlots = 32; //TODO;renderer caps


		Ref<VertexArray> QuadVA;
		Ref<VertexBuffer> QuadVB;
		Ref<Texture2D> WhitBlock  ;
		Ref<Shader> QuadShader ;
		
		Ref<VertexArray> CircleVA;
		Ref<VertexBuffer> CircleVB;
		Ref<Shader> CircleShader ;

		uint32_t quadIndexCount = 0; 
		QuadVertex* QuadVertexBufferBase =nullptr; //begin of qiadvertexArray 
		QuadVertex* QuadVertexBufferPtr = nullptr;	//now of quadvertexArray	
		
		uint32_t circleIndexCount = 0; 
		CircleVertex* CircleVertexBufferBase =nullptr; 
		CircleVertex* CircleVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;
		// 0  is for white texture; and we will traverse the slots ,one slot set a texture coord

		glm::vec4 quadVertexPosition[4];


		Renderer2D::Statistic Stats;

		//uniformBuffer的绑定实际上是对spriv_glsl shader的绑定,所以应该和uniformbuffer在spriv_glsl的形式一致,即struct处理,GPUBinding的uniformbuffer和shader中需要的一致
		struct CameraData
		{
			glm::mat4 ViewProjection;
		};

		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;

	};

	static Renderer2DData s_Data ; //又成整个结构了..,,这样的话整个结构都是静态,其成员就不能直接初始化而是应该在外初始化
	//将指针作为静态变量而不是将整个结构作为静态,这样方便管理(能够回收内存),只占一个指针的内存



	void Renderer2D::Init()
	{

		HZ_PROFILE_FUNCTION();

		ResetStats();


		if (s_Data.quadIndexCount >Renderer2DData::MaxIndeics)
		{
			EndScene();
			ShutDown();
		}	


		s_Data.QuadVA = VertexArray::Create();
		s_Data.QuadVB = VertexBuffer::Create(s_Data.MaxVertics * sizeof(QuadVertex));
		s_Data.QuadVB->SetLayout({
			{Hazel::ShaderDataType::Float3 ,"a_Position"},
			{Hazel::ShaderDataType::Float4 ,"a_Color"},
			{Hazel::ShaderDataType::Float2 ,"a_UVCoord"},
			{Hazel::ShaderDataType::Float ,"a_TexIndex"},
			{Hazel::ShaderDataType::Float ,"a_TilingFactor"},
			{Hazel::ShaderDataType::Int ,"a_EntityID"}
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

		//Circles
		s_Data.CircleVA = VertexArray::Create();
		s_Data.CircleVB = VertexBuffer::Create(s_Data.MaxVertics * sizeof(CircleVertex));

		s_Data.CircleVB->SetLayout({
			{Hazel::ShaderDataType::Float3 ,"a_WorldPosition"},
			{Hazel::ShaderDataType::Float3 ,"a_LocalPosition"},
			{Hazel::ShaderDataType::Float4 ,"a_Color"},
			{Hazel::ShaderDataType::Float ,"a_Thickness"},
			{Hazel::ShaderDataType::Float,"a_Fade"},
			{Hazel::ShaderDataType::Int ,"a_EntityID"}
			});
		s_Data.CircleVA->AddVertexBuffer(s_Data.CircleVB);
		s_Data.CircleVA->SetIndexBuffer(SquraIB); //圆也是四个点,index不需要改变
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertics];


		s_Data.WhitBlock = Texture2D::Create(1, 1);
		uint32_t OnePixel = 0xffffffff; //因为是采样...采样不在乎你原图的..
		s_Data.WhitBlock->SetData(&OnePixel, 4);




		int samples[s_Data.MaxTextureSlots];
		for ( int i = 0; i < s_Data.MaxTextureSlots; i++)
			samples[i] = i;
		
		s_Data.CircleShader = Shader::Create("Assets/shader/Renderer2D_Circle.glsl");
		s_Data.QuadShader = Shader::Create("Assets/shader/Renderer2D_Quad.glsl");

		//s_Data.QuadShader->Bind();
		//s_Data.QuadShader->SetIntArray("u_Textures",samples,s_Data.MaxTextureSlots);

		s_Data.TextureSlots[0] = s_Data.WhitBlock;


		//rotate around about the center
		s_Data.quadVertexPosition[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.quadVertexPosition[1] = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
		s_Data.quadVertexPosition[2] = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		s_Data.quadVertexPosition[3] = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);
	
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DData::CameraData), 0);

	}
	
	void Renderer2D::StartBatch()
	{
		s_Data.quadIndexCount = 0;	
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		
		s_Data.circleIndexCount= 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.TextureSlotIndex = 1; //begin
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}



	void Renderer2D::BeginScene(const Camera& Camera,glm::mat4 viewMat)
	{
		HZ_PROFILE_FUNCTION();


		s_Data.CameraBuffer.ViewProjection = Camera.GetProjection() * glm::inverse(viewMat);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));
		StartBatch();
	}

	//abandened
	void Renderer2D::BeginScene(const OrthographicCamera& Camera)
	{
		HZ_PROFILE_FUNCTION();

		s_Data.QuadShader->Bind();
		s_Data.CameraBuffer.ViewProjection = Camera.GetViewProjectionMat();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	
	}

	void Renderer2D::BeginScene(const EditorCamera& Camera)
	{
		HZ_PROFILE_FUNCTION();

		s_Data.QuadShader->Bind();
		s_Data.CameraBuffer.ViewProjection = Camera.GetViewProjectionMatrix();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DData::CameraData));

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();



		Flush();

	}

	void Renderer2D::Flush()
	{
		HZ_PROFILE_FUNCTION();

		if (s_Data.quadIndexCount)
		{
			uint32_t datasize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;//是空指针的步长.
			s_Data.QuadVB->SetData(s_Data.QuadVertexBufferBase, datasize);

			s_Data.QuadShader->Bind();
			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}
			RenderCommand::DrawIndexed(s_Data.QuadVA, s_Data.quadIndexCount);
			s_Data.Stats.DrawCalls++;
		}	
		
		if (s_Data.circleIndexCount)
		{
			uint32_t datasize = (uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase;//是空指针的步长.
			s_Data.CircleVB->SetData(s_Data.CircleVertexBufferBase, datasize);

			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVA, s_Data.circleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

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

		glm::mat4 tansformMat = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), glm::vec3(size, 0.f));

		DrawQuad(tansformMat, color);

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor )
	{
		HZ_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, texture, tilingFactor,tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{
		HZ_PROFILE_FUNCTION();

		// 2560 1664
		glm::mat4 tansformMat = glm::translate(glm::mat4(1.f), position) * glm::scale(glm::mat4(1.f), glm::vec3(size, 0.f));
		DrawQuad(tansformMat,texture,tilingFactor,tintColor);
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
		glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
			*
			glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0.f, 0.f, 1.f)) //使用默认就是弧度 //换算成 弧度
			*
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.f));

		DrawQuad(transform, color);

	}


	void Renderer2D::DrawQuad(const glm::mat4& transformat, const glm::vec4& color,int entityID)
	{

		HZ_PROFILE_FUNCTION();

		

		float textureIndex = 0.0f;
		float tilingFactor = 1.f;

		if (s_Data.quadIndexCount >= s_Data.MaxIndeics)
			NextBatch();


		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoord[4] = {
			{0.f,0.f},
			{1.f,0.f},
			{1.f,1.f},
			{0.f,1.f} };

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Position = transformat * s_Data.quadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoord[i];//左下角定义	
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;
		s_Data.Stats.quadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transformat, const Ref<Texture2D>& texture, float tilingFactor /*= 1.f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/, int entityID)
	{
		HZ_PROFILE_FUNCTION();

		if (s_Data.quadIndexCount >= s_Data.MaxIndeics)
			NextBatch();

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


		glm::vec4 color = tintColor;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoord[4] = {
				{0.f,0.f},
				{1.f,0.f},
				{1.f,1.f},
				{0.f,1.f}
		};


		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Position = transformat * s_Data.quadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoord[i];//左下角定义	
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;

			s_Data.QuadVertexBufferPtr++;
		}


		s_Data.quadIndexCount += 6;
		s_Data.Stats.quadCount++;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transformat, const glm::vec4& color, float thickness /*=1.f*/, float fade/*= 0.005f*/, int entityID /*=-1*/)
	{
		HZ_PROFILE_FUNCTION();


	// 		if (s_Data.CircleIndexCount >= s_Data.MaxIndeics)
	// 			NextBatch();

		constexpr size_t CircleVertexCount = 4;

		for (uint32_t i = 0; i < CircleVertexCount; i++)
		{
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->WorldPosition = transformat * s_Data.quadVertexPosition[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.quadVertexPosition[i]* 2.0f; // for localUV 4 points
			s_Data.CircleVertexBufferPtr->Thickness= thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}
		s_Data.circleIndexCount += 6;
		s_Data.Stats.quadCount++;
	}

	void Renderer2D::DrawSprite(const glm::mat4& transformat, SpriteRendererComponent& src, int entitID /*=-1*/)
	{
		if (src.Texture)
			DrawQuad(transformat, src.Texture,src.TilingFactor, src.Color,entitID);
		else
			DrawQuad(transformat, src.Color, entitID);

	}



	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{

		DrawRotatedQuad({ position.x, position.y,0 }, size, rotation, texture, tilingFactor, tintColor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tilingFactor, const glm::vec4& tintColor)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
			*
			glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0.f, 0.f, 1.f)) //使用默认就是弧度 //换算成 弧度
			*
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.f));

		DrawQuad(transform, texture, tilingFactor, tintColor);
	}



	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistic));
	}

	Renderer2D::Statistic& Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}





#if 0

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor /*= 1.f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		DrawQuad(glm::vec3(position, 0), size, subTexture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor /*= 1.f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{

		Ref<Texture2D> texture = subTexture->GetTexture();

		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.f,1.f,1.f,1.f };
		const glm::vec2* textureCoord = subTexture->GetTextureCoord();


		glm::vec2 Setposition[4] =
		{
			{0.f,0.f},
			{size.x,0.f},
			{size.x,size.y},
			{0.f,size.y}
		};

		float textureIndex = 0.f;

		if (s_Data.quadIndexCount >= s_Data.MaxIndeics)
			NextBatch();

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

		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Position = { position.x + Setposition[i].x,position.y + Setposition[i].y,position.z };
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoord[i];//左下角定义	
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;

		s_Data.Stats.quadCount++;

	}


	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor /*= 1.f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		DrawRotatedQuad({ position.x,position.y,0.f }, size, rotation, subTexture);
	}


	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, float tilingFactor /*= 1.f*/, const glm::vec4& tintColor /*= glm::vec4(1.0f)*/)
	{
		const Ref<Texture2D> texture = subTexture->GetTexture();


		float textureIndex = 0.f;

		if (s_Data.quadIndexCount >= s_Data.MaxIndeics)
			NextBatch();

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


		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.f,1.f,1.f,1.f };
		const glm::vec2* textureCoord = subTexture->GetTextureCoord();


		glm::mat4 transform = glm::translate(glm::mat4(1.f), position)
			*
			glm::rotate(glm::mat4(1.f), rotation, glm::vec3(0.f, 0.f, 1.f)) //it become radians ,so it was degree before
			*
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.f));


		for (uint32_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.quadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoord[i];//左下角定义	
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;

			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.quadIndexCount += 6;

		s_Data.Stats.quadCount++;

	}
#endif

}