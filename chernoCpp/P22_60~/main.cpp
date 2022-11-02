
#include <string>
#include <iostream>
#include <cmath>
#include <chrono>
#include <fstream>
#include <algorithm>
#include <thread>


struct ProfileResult
{
	std::string Name;
	long long Start, End;
	uint32_t threadID;
};

struct InstrumentationSession
{
	InstrumentationSession(const std::string& name) { m_name = name; }
	std::string m_name;
};


class Instrumentor
{
public:
	Instrumentor()
		:m_CurrentSession(nullptr),m_ProfileCount(0)
	{

	};

	void BeginSession(const std::string& name,const std::string& filePath  = "result.json")
	{
		m_OutputStream.open(filePath);
		WriteHeader();
		m_CurrentSession = new InstrumentationSession(name);
	}

	void EndSession()
	{
		WriteFooter();
		m_OutputStream.close();

		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
	};



	void WriteProfile(const ProfileResult& result) //写文件内容
	{
		if (m_ProfileCount++ > 0)
		{
			m_OutputStream << ",";
		};
		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";   //类型
		m_OutputStream << "\"dur\":"<<(result.End- result.Start)<<","; //tick
		m_OutputStream << "\"name\":\""<<name<<"\","; //funcname
		m_OutputStream << "\"ph\":\"X\","; 
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":"<<result.threadID<<",";//线程id
		m_OutputStream << "\"ts\":"<<result.Start; //开始tick
		m_OutputStream << "}";

		m_OutputStream.flush(); //刷新/输出到文件


	}

	void WriteHeader()//写文件头
	{
		m_OutputStream << "{\"otherData\":{},\"traceEvents\":[";
		m_OutputStream.flush();
	}
	void WriteFooter()//写文件尾
	{
		m_OutputStream << "]}";
		m_OutputStream.flush();

	}

	static Instrumentor& Get()
	{
		static Instrumentor* instance = new Instrumentor();
		return *instance;
	}


private:
	InstrumentationSession* m_CurrentSession;
	std::ofstream m_OutputStream;
	int m_ProfileCount;
};




class InstrumentationTimer //Instrumentation 插桩
{
public:
	InstrumentationTimer(const char* name)
		:m_Name(name), m_stopped(false)
	{
		m_startTimepoint = std::chrono::high_resolution_clock::now();
	}

	void stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();


		long long start = std::chrono::time_point_cast<std::chrono::microseconds> (m_startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds> (endTimepoint).time_since_epoch().count();

		m_stopped = true;
		uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().WriteProfile({m_Name,start,end,threadID});

	}
	~InstrumentationTimer()
	{
		if (!m_stopped)
		{
			stop();
		}
	}



private:
	const char* m_Name;
	bool m_stopped;

	std::chrono::time_point < std::chrono::steady_clock> m_startTimepoint;
};
#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer time##__LINE__( name)
 #define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__);  //__FUNCTION__能读当前在的文件名(const char*) ,__FUNCSIG__读取函数签名(签名能体现重载和作用域名)

#else 
#definePROFILE_SCOPE(name)
#endif 
namespace benchFuncs
{ 
void func1(int value)
	
{
	PROFILE_FUNCTION()
	for (int i = 0; i < 1000; i++)
	{
		std::cout << "Hazel: " << i << std::endl;
	}
}
void func2()
{
	PROFILE_FUNCTION();

	for (int i = 0; i < 1000; i++)
	{
		std::cout << "Hazel: " << std::sqrt(i) << std::endl;
	}
}



void RunBenchMark()
{
	PROFILE_FUNCTION();

	std::cout << "Running Benchmarks...\n";

	
	std::thread a([]() {func1(2); });
	std::thread b([]() {func2(); });

	a.join();
	b.join();
}
}
int main()
{
	Instrumentor::Get().BeginSession("test");   //插桩

	std::cout << "Part60:基准测试可视化" << std::endl;
	//使用 chrome 浏览器自带的 tracing 做
	//我们需要将测试数据按照chrome标准导入到一个json文件中,chrome来可视化它
	benchFuncs::RunBenchMark();


	Instrumentor::Get().EndSession(); //拔出





}