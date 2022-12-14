#pragma once
#include <Hazel/Core/Base.h>

namespace Hazel
{
	class Texture
	{
	public:


		virtual uint32_t GetWidth() const= 0;
		virtual uint32_t GetHeight() const= 0;

		virtual uint32_t GetRendererID() const=0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot =0) const= 0;

		virtual bool operator==(const Texture& other ) const = 0; 
		

	};

	class Texture2D:public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& filePath);//不再使用裸指针,而是使用shared->强引用
		static Ref<Texture2D> Create(uint32_t width, uint32_t height );//不再使用裸指针,而是使用shared->强引用




	};



}
