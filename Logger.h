#pragma once
#include "noncopyable.h"
#include <string>
#include "Timestamp.h"

// 定义日志的级别  INFO, WARNING, ERROR, FATAL, DEBUG
enum LogLevel
{
    INFO,    // 普通信息
    WARNING, // 警告信息
    ERROR,   // 错误信息
    FATAL,   // 严重错误信息
    DEBUG    // 调试信息
};

// 日志类
class Logger : noncopyable
{
public:
    // 获取日志唯一的实例对象
    static Logger &instance();

    // 设置日志的级别
    void setLogLevel(LogLevel level);

    // 写日志
    void log(std::string msg);

private:
    // 构造函数
    Logger();

    // 析构函数
    ~Logger();

    int logLevel_; // 日志级别
};

// 定义宏调用  LOG_INFO("%s  %d", arg1, arg2)
#define LOG_INFO(logmsgformat, ...)                     \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::instance();            \
        logger.setLogLevel(INFO);                       \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(c);                                  \
    } while (0);

// 在这个宏定义中，__VA_ARGS__ 代表可变参数列表。##__VA_ARGS__ 的作用是将可变参数列表展开并替换到宏定义中。如果没有提供可变参数，##操作符会移除##前面的1个逗号
#define LOG_WARNING(logmsgformat, ...)                  \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::instance();            \
        logger.setLogLevel(WARNING);                    \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(c);                                  \
    } while (0);

#define LOG_ERROR(logmsgformat, ...)                    \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::instance();            \
        logger.setLogLevel(ERROR);                      \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(c);                                  \
    } while (0);

#define LOG_FATAL(logmsgformat, ...)                    \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::instance();            \
        logger.setLogLevel(FATAL);                      \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(c);                                  \
        exit(-1);                                       \
    } while (0);

#ifdef MUdUO_DEBUG
#define LOG_DEBUG(logmsgformat, ...)                    \
    do                                                  \
    {                                                   \
        Logger &logger = Logger::instance();            \
        logger.setLogLevel(DEBUG);                      \
        char c[1024] = {0};                             \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__); \
        logger.log(c);                                  \
    } while (0);
#else
#define LOG_DEBUG(logmsgformat, ...)
#endif
