#pragma once
//正式开发一个实用类 log
#include <iostream>


class Log
{
public://你可以把不同类型但同级别的成员与方法分开写,进行分类便于区分,
//比如这个public是public的变量
	enum Loglevel
	{
		LogLevelError=0, LogLevelWarning, LogLevelInfo
	};
private:
	Loglevel m_Loglevel=LogLevelInfo;//m_表示这是一个私有变量,默认任何信息都会被打印
public:
	void setLevel(int level) {
		m_Loglevel = level;

	}
	void error(const char* mesg) {
		if(m_Loglevel>=LogLevelError)
			std::cout << "[ERROR]:" << mesg << std::endl;
	}
	void warn(const char* mesg) {
		if(m_Loglevel>=LogLevelWarning)
			std::cout <<"[WARNING]:"<< mesg << std::endl;
	}
	void info(const char* mesg) {
		if(m_Loglevel>=LogLevelInfo)
			std::cout << "[INFO]:" << mesg << std::endl;
	}

};

