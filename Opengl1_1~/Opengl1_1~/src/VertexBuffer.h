#pragma once
class VertexBuffer
{
private:
	unsigned int m_rendererID;
public:
	VertexBuffer(const void* Data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

};

