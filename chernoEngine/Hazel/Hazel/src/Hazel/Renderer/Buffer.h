#pragma once
#include "hzpch.h"


namespace Hazel
{
	//cherno不想一个类一个文件


	enum class ShaderDataType:uint8_t
	{
		None= 0,
		Float,Float2, Float3, Float4,
		Mat3,Mat4,
		Int, Int2, Int3, Int4,
		Bool

	};

	static uint32_t SizeofShaderDataType(ShaderDataType type) //static也没事,会只限定在这个cpp单元内
	{
		switch (type)
		{
		case Hazel::ShaderDataType::None: //break也许会被优化掉?
			return 0;
			break;
		case Hazel::ShaderDataType::Float:
			return sizeof(float) ;
			break;
		case Hazel::ShaderDataType::Float2:
			return sizeof(float)*2;
			break;
		case Hazel::ShaderDataType::Float3:
			return sizeof(float)*3;
			break;
		case Hazel::ShaderDataType::Float4:
			return sizeof(float)*4;
			break;
		case Hazel::ShaderDataType::Mat3:
			return sizeof(float)*9;
			break;
		case Hazel::ShaderDataType::Mat4:
			return sizeof(float)*16;
			break;
		case Hazel::ShaderDataType::Int:
			return sizeof(int);
			break;
		case Hazel::ShaderDataType::Int2:
			return sizeof(int)*2;
			break;
		case Hazel::ShaderDataType::Int3:
			return sizeof(int)*3;
			break;
		case Hazel::ShaderDataType::Int4:
			return sizeof(int)*4;
			break;
		case Hazel::ShaderDataType::Bool:
			return 1;
			break;
		default:
			break;
		}

		HZ_CORE_ASSERT(false, "Not Defined Type in ShaderDatatype ");
		return 0;
	}

	struct LayoutElement
	{

		LayoutElement()=default;
		ShaderDataType type;//GL里是
		std::string name;
		unsigned char ifNormalized;
		uint32_t size; //size
		uint32_t offset;//ptr

		LayoutElement(ShaderDataType Type, const std::string& Name, bool IfNor = false)
			:type(Type), name(Name), ifNormalized(IfNor), size(SizeofShaderDataType(Type)), offset(0)//offset由其他element决定(外部设置,这里简单初始化一下	
		{
		}

		inline uint32_t GetElementCount() const //是inline的..简单函数还定义在类内的就是inline的
		{
			switch (type)
			{
			case Hazel::ShaderDataType::None:
				return 0;
			case Hazel::ShaderDataType::Float:
				return 1;
			case Hazel::ShaderDataType::Float2:
				return 2;
			case Hazel::ShaderDataType::Float3:
				return 3;
			case Hazel::ShaderDataType::Float4:
				return 4;
			case Hazel::ShaderDataType::Mat3:
				return 3*3;
			case Hazel::ShaderDataType::Mat4:
				return 4*4;
			case Hazel::ShaderDataType::Int:
				return 1;
			case Hazel::ShaderDataType::Int2:
				return 2;
			case Hazel::ShaderDataType::Int3:
				return 3;
			case Hazel::ShaderDataType::Int4:
				return 4;
			case Hazel::ShaderDataType::Bool:
				return 1;
			default:
				break;
			}

			HZ_CORE_ASSERT(false, "Not Defined Type in ShaderDatatyp ");
			return 0;
		}

	};

	class BufferLayout
	{
	public:
		BufferLayout(const  std::initializer_list<LayoutElement>& elements)
			:m_elements(elements)
		{
			CalculateOffsetAndStride();//const & 是在修改外部变量(根本改不了,const v& 遍历也得是 const),这里还是得修改内部成员 
		}; //构造可以有,这只是一种数据结构

		BufferLayout() = default;

		~BufferLayout();
		inline const std::vector<LayoutElement>& GetLayoutElement()const { return m_elements; }
		inline const uint32_t GetStride()const { return m_stride; }

		std::vector<LayoutElement>::iterator begin() { return m_elements.begin(); } //提供迭代器接口,方便遍历
		std::vector<LayoutElement>::iterator end() { return m_elements.end(); }
		std::vector<LayoutElement>::const_iterator begin() const{ return m_elements.begin(); } //提供迭代器接口,方便遍历
		std::vector<LayoutElement>::const_iterator end() const { return m_elements.end(); }//专门为const& 和const函数提供的 const迭代器

	private:
		std::vector<LayoutElement> m_elements;
		uint32_t  m_stride; //顶点步长

		void CalculateOffsetAndStride()
		{
			uint32_t currentOffset = 0;

			for (LayoutElement& element : m_elements)
			{

				element.offset = currentOffset;
				currentOffset += element.size;

			}
			m_stride = currentOffset;
		};

	};

	class VertexBuffer
	{
	public:
		//VertexBuffer() {}; 不能有构造函数,必须得由子类实现实例创建,
		virtual ~VertexBuffer() =default; //同时这些函数必须得是protect或public才能对子类开放,子类自己的构造析构会需要调用他们
		//virtual ~VertexBuffer() =default; virtual ~VertexBuffer(){};类似,前者显式指明构建默认析构函数(有的情况下编译器不会为你构建默认函数),后面随便写了个空实现 
		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static  Ref<VertexBuffer> Create(float* vertics, uint32_t size);
		static  Ref<VertexBuffer> Create(uint32_t size);

		virtual void SetLayout(const BufferLayout& layout)=0;
		virtual void SetData(const void* data,uint32_t size)=0;


		virtual const BufferLayout& GetLayout()const  =0;

	};


	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() =default;

		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual uint32_t GetCount()const = 0;

		static  Ref<IndexBuffer> Create(uint32_t* indices, size_t size);

	};


}