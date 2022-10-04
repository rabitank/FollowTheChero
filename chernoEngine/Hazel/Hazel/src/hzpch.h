#pragma once

//预编译头 添加到premake中
#include<iostream>
#include<memory>
#include<utility>
#include<algorithm>
#include<functional>

#include<sstream>	 
#include<string>
#include<vector>
#include<tuple>
#include<unordered_map>
#include<unordered_set>

#include "Hazel/Log.h"

#ifdef HZ_PLATFORM_WINDOWS
#include <Windows.h>
#else
#error Hazel only support windows!
#endif

