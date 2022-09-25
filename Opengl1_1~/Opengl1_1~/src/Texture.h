#pragma once

#include "renderer.h"

class Texture
{
private:
	unsigned int m_rendererID;
	std::string m_filePath;
	unsigned char* m_localBuffer;
	int m_weight, m_height,m_BPP;
public:
	Texture(const std::string& filepath);
	~Texture();
	void Bind(unsigned int slot =0 )const;//slot 纹理插槽,现代电脑显卡上有32个纹理插槽
	void Unbind() const;

	inline int GetWeight() const { return m_weight; }
	inline int GetHeight() const { return m_height; }
};

