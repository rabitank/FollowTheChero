#include "hzpch.h"
#include "Glad/glad.h"

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