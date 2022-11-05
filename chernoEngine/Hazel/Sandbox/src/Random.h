#pragma once

#include <random>

class Random
{
public:
	static void Init()
	{
		s_randomEngine.seed(std::random_device()());
	}
	static float Float()
	{
		return (float)s_Distribution(s_randomEngine) / (float)std::numeric_limits<uint32_t>::max();
	}
private:
	static std::mt19937 s_randomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

