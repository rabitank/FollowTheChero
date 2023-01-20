#pragma once

#define LOG(x) std::cout<<x<<std::endl;

template<typename Vector> //这个Typename...? 是一种假设,静态编译会认出来然后提供联想 , 不影响你往里面填各种类型
class VectorIterator
{
public:
	using ValueType = typename Vector::ValueType;
	using PointerType = ValueType*;
	using ReferenceType  = ValueType&;
public:
	VectorIterator(PointerType ptr):m_ptr(ptr){};
	VectorIterator& operator++() //前缀
	{
		m_ptr++;
		return *this;
	};
	const VectorIterator operator++(int)//后缀
	{
		VectorIterator iterator = *this;
		++(*this);
		return iterator;
	};

	VectorIterator& operator--() //前缀
	{
		m_ptr--;
		return *this;
	};
	const VectorIterator operator--(int)//后缀
	{
		VectorIterator iterator = *this;
		--(*this);
		return iterator;
	};

	ReferenceType operator[](size_t index)
	{
		return *(m_ptr + index);
	}

	PointerType operator->()
	{
		return m_ptr;
	}
	
	ReferenceType operator*()
	{
		return *m_ptr;
	}

	bool operator== (const VectorIterator& other) const	
	{
		return m_ptr == other.m_ptr;
	}
	
	bool operator!= (const VectorIterator& other) const	
	{
		return !(*this==other);
	}

private:
	PointerType m_ptr;
};



template<typename T>
class Vector
{
public:
	using ValueType = T;
	using Iterator = VectorIterator<Vector<ValueType>>;

public:
	Vector()
	{
		//allocate 2 elements;
		ReAlloc(2);
	}
	~Vector()
	{
		Clear();

		//可能会和Clear,Pop重复 , delete[] 调用整个容器所有位置的析构, 这会导致原本Pop的位置的元素被重复析构.
		//vector3 中的int* m_memoryBlock被释放两次
	}
	void pushBack(const T& element) 
	{
		if (m_size >= m_capacity)
		{
			ReAlloc(m_capacity + m_capacity / 2);
		}

		m_data[m_size] = element;
		m_size++;

	};
	void pushBack(T&& element) 
	{
		if (m_size >= m_capacity)
		{
			ReAlloc(m_capacity + m_capacity / 2);
		}

		m_data[m_size] = std::move(element);

		m_size++;



	};

	

	template<typename... Args>
	T&  emplaceBack(Args&&... args) //原地构造,客户可能需要拿到构造的对象
	{

		if (m_size >= m_capacity)
		{
			ReAlloc(m_capacity + m_capacity / 2);
		}

		new(&m_data[m_size]) T(std::forward<Args>(args)...); //前向转发 , std::move的进阶版??对参数包使用,整个move  ... 是move后展开
		//这个new是placement new ,在指定地址新建对象(直接覆盖)...很nb的用法 
		// 消除了 m_data[m_size] =  T(std::forward<Args>(args)...) 的 move (这个应该是m_data已经都初始化然后 = 移动赋值,而不是拷贝构造)

		m_size++;

		return m_data[m_size];
	}

	void PopBack()
	{
		if (m_size > 0)
		{
			m_size--;
			m_data[m_size].~T();

		}
	}
	void Clear() //只会删除size以内的对象,它们都实际赋值过,仍然存在
	{
		for (size_t i = 0; i < m_size; i++)
		{
			m_data[i].~T();
		}
		m_size = 0;
	}

	Iterator begin()
	{
		return Iterator(m_data);
	}
	
	Iterator end()
	{
		return Iterator(m_data + m_size);//, sizeof(T));
	}


	T& operator[](int index)  //返回左值 , 可修改模式
	{
		if (!(index <m_size))
		{
			__debugbreak(); //msvc的debug模式中断
		}
		return m_data[index];
	}

	const T& operator[](int index) const //只读 , const类型的array引用也会用const版的
	{
		if (!(index < m_size))
		{
			__debugbreak(); //msvc的debug模式中断
		}

		return m_data[index];
	}

	T* Data() { return m_data; }
	const T* Data() const { return m_data; }

	size_t Size() const { return m_size; }


private:
	void ReAlloc(size_t newCapacity) 
	{
		//1. 分配新内存块
		//T* newBlock = new T[newCapacity]; 不应该进行构造

		T* newBlock = (T*)::operator new(newCapacity * sizeof(T));
		//对于std::String 等,这样的分配可能还优点问题,对基本类之外的有赋值方法的复杂类,它没有构造对象,std::move调用类移动赋值方法需要的成员不存在,调用会会失败
		//下面修复了

		if (newCapacity < m_size)
			m_size = newCapacity;

		//2. 复制/移动数据到新内存
		for (size_t i = 0; i < m_size; i++)
		{
			new (&newBlock[i]) T(std::move(m_data[i])); //在尺寸之内就地移动构造元素
		}

		//memcpy(newBlock,newBlock , ) 不用是因为选择了拷贝方式,需要类的拷贝构造函数
		//3. 删除

		for (size_t i = 0; i < m_size; i++)
		{
			m_data[i].~T();
		}

		::operator delete(m_data, m_capacity * sizeof(T));//避免调用析构函数以至于 和pop,Clear冲突,重复析构

		m_data = newBlock;
		m_capacity = newCapacity;
	};


private:
	T* m_data = nullptr;
	
	size_t m_size = 0;
	size_t m_capacity = 2; // 容量 分配并不线性
};


struct Vector3
{
	float x = 0.f, y = 0.f, z = 0.f;
	int* m_memoryBlock = nullptr; //for test

	Vector3()
	{
		m_memoryBlock = new int[5];
	};
	Vector3(float scale)
		: x(scale), y(scale), z(scale)
	{
		m_memoryBlock = new int[5];
	}
	Vector3(float X, float Y, float Z)
		:x(X), y(Y), z(Z)
	{
		m_memoryBlock = new int[5];
	}
	Vector3(const Vector3& other) 
	: x(other.x), y(other.y), z(other.z)
	{

		LOG("Copy Construct");
	}
	Vector3(Vector3&& other)
		:x(other.x), y(other.y), z(other.z)
	{
		m_memoryBlock = other.m_memoryBlock;
		other.m_memoryBlock = nullptr;

		LOG("Move Construct");
	}
	Vector3& operator=(const Vector3& other) 
	{
		x = other.x;
		y = other.y;
		z = other.z;
		LOG("Copy");

		return *this;
	}
	Vector3& operator=(Vector3&& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;

		m_memoryBlock = other.m_memoryBlock;
		other.m_memoryBlock = nullptr;

		LOG("Move");
		return *this;
	}

	~Vector3() { LOG("Destoied"); delete[] m_memoryBlock; };



};

template<typename T>
void PrintVector(const Vector<T>& vector)
{
	for (size_t i = 0; i < vector.Size(); i++)
	{
		LOG(vector[i]);
	}
	std::cout << "----------------------------" << std::endl;
}
template<>
void PrintVector(const Vector<Vector3>& vector)
{
	for (size_t i = 0; i < vector.Size(); i++)
	{
		LOG(vector[i].x << vector[i].y << vector[i].z);
	}
	std::cout << "----------------------------" << std::endl;
}
