#include "renderer.h"
#include <iostream>


void GlClearError()
{
	while (glGetError() != GL_NO_ERROR);//GL_NO_ERROR  = 0
}
bool GlLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[openGl errro]:(" << std::hex << (unsigned int)error << ")"
			<< " in file:" << file
			<< " function:" << function
			<< "at " << std::dec << line << " line" << std::endl;//find what kind error in glew.h
		return false;
	};
	return true;
}

void renderer::Drawer(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.Getcount(), GL_UNSIGNED_INT, nullptr));//errorgettest1 _ p10//glGetError();
}
void renderer::clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}