#pragma once

#include "VertexBuffer.h"
class BufferLayout;
//这里va被renderer引用,而bufferlayout.h引用了renderer,这里循环引用了导致错误,
//为了规避引用bufferlayout.h使用类声明

class VertexArray
{
private: 
	unsigned int m_rendererID;

public:
	VertexArray();
	~VertexArray();
	void AddBuffer(const VertexBuffer& buffer,const BufferLayout& bl);
	void Bind()const;

	void Unbind() const;
};

