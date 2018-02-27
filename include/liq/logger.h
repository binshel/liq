/**
 * @file logger.h
 * @brief 打印程序运行日志
 */

#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <string>


namespace liq {

    ///< @brief 文件 IO 的 buff 大小
#   define LOG_BUF_LEN 10240

    /**
     * 日志功能接口
     */
    class ILogger {
    public:
        /**
         * @brief 刷新缓存的日志
         */
        virtual void flush() = 0;
        /**
         * @brief 格式化打印日志
         * @param fmt 输出格式
         * @param ... 格式参数
         */
        virtual ILogger& format(const char *fmt, ...) = 0;
        /**
         * @brief 流方式打印日志
         */
        virtual ILogger& operator<< (bool val ) = 0;
        virtual ILogger& operator<< (short val ) = 0;
        virtual ILogger& operator<< (unsigned short val ) = 0;
        virtual ILogger& operator<< (int val ) = 0;
        virtual ILogger& operator<< (unsigned int val ) = 0;
        virtual ILogger& operator<< (long val ) = 0;
        virtual ILogger& operator<< (unsigned long val ) = 0;
        virtual ILogger& operator<< (float val ) = 0;
        virtual ILogger& operator<< (double val ) = 0;
        virtual ILogger& operator<< (long double val ) = 0;
        virtual ILogger& operator<< (const char *val) = 0;
        virtual ILogger& operator<< (const std::string& val) = 0;
        virtual ILogger& operator<< (const void* val) = 0;
    };

    /**
     * ILogger 的一个实现：忽略日志
     */
    class LoggerNone: public ILogger {
    public:
        virtual inline void flush() {}
        virtual inline ILogger& format(const char *fmt, ...) {return *this;}
        virtual inline ILogger& operator<< (bool val ) {return *this;}
        virtual inline ILogger& operator<< (short val ) {return *this;}
        virtual inline ILogger& operator<< (unsigned short val ) {return *this;}
        virtual inline ILogger& operator<< (int val ) {return *this;}
        virtual inline ILogger& operator<< (unsigned int val ) {return *this;}
        virtual inline ILogger& operator<< (long val ) {return *this;}
        virtual inline ILogger& operator<< (unsigned long val ) {return *this;}
        virtual inline ILogger& operator<< (float val ) {return *this;}
        virtual inline ILogger& operator<< (double val ) {return *this;}
        virtual inline ILogger& operator<< (long double val ) {return *this;}
        virtual inline ILogger& operator<< (const char *val) {return *this;}
        virtual inline ILogger& operator<< (const std::string& val) {return *this;}
        virtual inline ILogger& operator<< (const void* val ) {return *this;}
    };

    /**
     * ILogger 的一个实现：打印日志到文件
     */
    class LoggerFile: public ILogger {
    public:
        /**
         * @brief 初始化日志系统
         * @param node_name 节点名，用于确定输出文件的文件名
         * @param buf_size 日志文件的 buff 大小
         */
        int init(const std::string &node_name, size_t buf_size = LOG_BUF_LEN);

        inline void flush()
        {
            fflush(this->file);
        }

        virtual inline ILogger& format(const char *fmt, ...)
        {
            va_list args;
            va_start(args, fmt);
            vfprintf(file, fmt, args);
            va_end(args);
            return *this;
        }
        virtual inline ILogger& operator<< (const bool val )
        {
            fprintf(file, " %s", val ? "true" : "false");
            return *this;
        }
        virtual inline ILogger& operator<< (short val ) 
        {
            fprintf(file, " %d", val);
            return *this;
        }
        virtual inline ILogger& operator<< (unsigned short val ) 
        {
            fprintf(file, " %u", val);
            return *this;
        }
        virtual inline ILogger& operator<< (int val )
        {
            fprintf(file, " %d", val);
            return *this;
        }
        virtual inline ILogger& operator<< (unsigned int val ){
            fprintf(file, " %u", val);
            return *this;
        }
        virtual inline ILogger& operator<< (long val )
        {
            fprintf(file, " %ld", val);
            return *this;
        }
        virtual inline ILogger& operator<< (unsigned long val )
        {
            fprintf(file, " %lu", val);
            return *this;
        }
        virtual inline ILogger& operator<< (float val )
        {
            fprintf(file, " %f", val);
            return *this;
        }
        virtual inline ILogger& operator<< (double val )
        {
            fprintf(file, " %lf", val);
            return *this;
        }
        virtual inline ILogger& operator<< (long double val )
        {
            fprintf(file, " %Lf", val);
            return *this;
        }
        virtual inline ILogger& operator<< (const char *val)
        {
            fprintf(file, " %s", val);
            return *this;
        }
        virtual inline ILogger& operator<< (const std::string& val)
        {
            fprintf(file, " %s", val.c_str());
            return *this;
        }
        virtual inline ILogger& operator<< (const void* val )
        {
            fprintf(file, " %p", val);
            return *this;
        }
    private:
        FILE *file;
        char filepath[256];
    };

    /**
     * @brief 管理日志服务，根据日志级别给出不同的日志实现
     */
    class LoggerManager 
    {
    public:
        /// @brief 定义日志级别
        enum LogLevel
        {
            LOG_DEBUG = 0,
            LOG_INFO,
            LOG_WARN,
            LOG_ERROR,
            LOG_LEVEL_MAX
        };
        LoggerManager(const std::string &node_name, LogLevel level = LOG_DEBUG);
        ILogger &get(LogLevel level);
        void ontick();
    private:
        LogLevel level;     ///< 日志级别
        LoggerNone *none;   ///< 不记录日志
        LoggerFile *file;   ///< 用文件记录日志
    };

    
/// @brief 行结束符
#define EOL "\n"

#define LOGGER_FIX(level, prefix, suffix)  \
    LOGGER_DO(liq::Liq::instance().logger, level, prefix, suffix) 
#define LOGGER_DO(logger, level, prefix, suffix)  \
    logger->get(level).format(prefix "[%d @] %04d-%02d-%02d %02d:%02d:%02d %s:%d" suffix,    \
            level + 1,                                                          \
            liq::Liq::instance().tm.tm_year + 1900, liq::Liq::instance().tm.tm_mon + 1, liq::Liq::instance().tm.tm_mday,        \
            liq::Liq::instance().tm.tm_hour, liq::Liq::instance().tm.tm_min, liq::Liq::instance().tm.tm_sec,                    \
            __FILE__, __LINE__) 

    /// @brief 流形式初始 debug 级别日志
#define DEBUG   LOGGER_FIX(liq::LoggerManager::LOG_DEBUG, "", "")
    /// @brief 流形式初始 info 级别日志
#define INFO    LOGGER_FIX(liq::LoggerManager::LOG_INFO, "", "")
    /// @brief 流形式初始 warn 级别日志
#define WARN    LOGGER_FIX(liq::LoggerManager::LOG_WARN, "", "")
    /// @brief 流形式初始 error 级别日志
#define ERROR   LOGGER_FIX(liq::LoggerManager::LOG_ERROR, "", "")

    /// @brief 格式化输出 debug 级别日志
#define DEBUGF(fmt, args...) \
    LOGGER_FIX(liq::LoggerManager::LOG_DEBUG, "", " ").format(fmt "\n", ##args)
    /// @brief 格式化输出 info 级别日志
#define INFOF(fmt, args...) \
    LOGGER_FIX(liq::LoggerManager::LOG_INFO, "", " ").format(fmt "\n", ##args)
    /// @brief 格式化输出 warn 级别日志
#define WARNF(fmt, args...) \
    LOGGER_FIX(liq::LoggerManager::LOG_WARN, "", " ").format(fmt "\n", ##args)
    /// @brief 格式化输出 error 级别日志
#define ERRORF(fmt, args...) \
    LOGGER_FIX(liq::LoggerManager::LOG_ERROR, "", " ").format(fmt "\n", ##args)
    
#define CATCH(statement) \
    try {  \
        statement; \
    } catch (const char *e) { \
        ERRORF("exception string: [%s]", e); \
    } catch (const std::exception &e) { \
        ERRORF("std exception: [%s]", e.what()); \
    }
}

