#pragma once
#include <glm/glm.hpp>
#include <Hazel/Core/core.h>
#include "Texture.h"

namespace Hazel
{
	class SubTexture2D//只是添加切割的uv坐标的
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture , const glm::vec2& min, const glm::vec2& max);
		inline const Ref<Texture2D>& GetTexture() const { return m_texture; };
		inline const glm::vec2* GetTextureCoord() const { return m_textureCoord; };

		
		static Ref<SubTexture2D> CreateFromCroods(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1.f,1.f }); //根据区块坐标创建
	private:
		Ref<Texture2D> m_texture;


		glm::vec2 m_textureCoord[4];


	};

}

