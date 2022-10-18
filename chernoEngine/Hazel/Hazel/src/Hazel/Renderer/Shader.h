#pragma once
#include "hzpch.h"


namespace Hazel
{
	class Shader
	{

	public:
		
		virtual ~Shader() {};

		static Shader* Create(const std::string& filepath);

		//Bind,Unbind just for debug mode
		virtual void Bind() const =0;
		virtual void UnBind()const=0;
	private:

	};
}


