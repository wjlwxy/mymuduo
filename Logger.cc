#include "Logger.h"
#include <iostream>

// 单例模式

Logger &Logger::instance()
{
    static Logger logger;
    return logger;
}

// 设置日志的级别
void Logger::setLogLevel(LogLevel level)
{
    logLevel_ = level;
}

// 写日志
void Logger::log(std::string msg)
{
    switch (logLevel_)
    {
    case INFO:
        std::cout << "[INFO] ";
        break;
    case WARNING:
        std::cout << "[WARNING] ";
        break;
    case ERROR:
        std::cout << "[ERROR] ";
        break;
    case FATAL:
        std::cout << "[FATAL] ";
        break;
    case DEBUG:
        std::cout << "[DEBUG] ";
        break;
    default:
        break;
    }
    std::cout << Timestamp::now().toString() << " : " << msg << std::endl;
}

// 构造函数
Logger::Logger()
{
}

// 析构函数
Logger::~Logger()
{
}
