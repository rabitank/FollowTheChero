#pragma once
#include "Hazel/Renderer/Texture.h"

namespace Hazel
{

	class OpenGlTexture:public Texture2D
	{
	public:
		virtual void Bind(uint32_t slot=0) const override;
		inline virtual uint32_t GetWidth() const override { return m_width; };
		inline virtual uint32_t GetHeight() const override { return m_heigth; };

		OpenGlTexture(const std::string& path) ;
		virtual ~OpenGlTexture() ;

	private:
		std::string m_filePath;
		uint32_t m_rendererID;
		int m_width;
		int m_heigth;
		int m_BPP;
		
	};
}

