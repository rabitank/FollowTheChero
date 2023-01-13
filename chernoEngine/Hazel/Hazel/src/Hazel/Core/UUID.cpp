#include "hzpch.h"
#include "UUID.h"

#include <random>

#include <unordered_map>

std::unordered_map<Hazel::UUID, std::string> m_Map;
static void Add2Map()
{
	m_Map[Hazel::UUID()] = "Hazel_UUID";
}


namespace Hazel
{

	static std::random_device s_RandomDevice; //随机设备 -> 提供高频真随机数 非确定性随机数生成器，使用硬件作为随机数来源，故其调用代价较高，一般用来产生随机数种子。
	static std::mt19937_64 s_Engine(s_RandomDevice()); //随机数引擎返回uint ,_64 返回指64位数据.用来生成伪随机数 其范围 2^19937 - 1
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution; //统一整数分布器? 分发uint64_t整数的一个hash? ->随机数分布控制

	//有人提议建立个 无序集合放UUID,用来新建UUID时检测是否重复了,cherno说没必要,概率很小
	UUID::UUID()
		:m_UUID(s_UniformDistribution(s_Engine))
	{
	}

	UUID::UUID(uint64_t id)
		:m_UUID(id)
	{
	}

}