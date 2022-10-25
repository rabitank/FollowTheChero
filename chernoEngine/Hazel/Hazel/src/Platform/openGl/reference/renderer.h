#pragma once

#include<GLFW\glfw3.h>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"






class renderer//可以写成静态类,但是cherno为了多线程写成可实例的
{
public:
	void Drawer(const VertexArray& va,const IndexBuffer& ib,const Shader& shader)const;
	void clear() const;
};

