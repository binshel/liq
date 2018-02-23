#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <string>


namespace liq {
#define LOG_BUF_LEN 10240
    class ILogger {
    public:
        virtual ILogger& format(const char *fmt, ...) = 0;
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
    class LoggerNone: public ILogger {
    public:
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
    class LoggerFile: public ILogger {
    public:
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

    class LoggerManager 
    {
    public:
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
        LogLevel level;
        LoggerNone *none;
        LoggerFile *file;
    };

    
#define EOL "\n"

#define LOGGER_FIX(level, prefix, suffix)  \
    LOGGER_DO(liq::Liq::instance().logger, level, prefix, suffix) 
#define LOGGER_DO(logger, level, prefix, suffix)  \
    logger->get(level).format(prefix "[%d @] %04d-%02d-%02d %02d:%02d:%02d %s:%d" suffix,    \
            level + 1,                                                          \
            liq::Liq::instance().tm.tm_year + 1900, liq::Liq::instance().tm.tm_mon + 1, liq::Liq::instance().tm.tm_mday,        \
            liq::Liq::instance().tm.tm_hour, liq::Liq::instance().tm.tm_min, liq::Liq::instance().tm.tm_sec,                    \
            __FILE__, __LINE__) 

#define DEBUG   LOGGER_FIX(liq::LoggerManager::LOG_DEBUG, "", "")
#define INFO    LOGGER_FIX(liq::LoggerManager::LOG_INFO, "", "")
#define WARN    LOGGER_FIX(liq::LoggerManager::LOG_WARN, "", "")
#define ERROR   LOGGER_FIX(liq::LoggerManager::LOG_ERROR, "", "")

#define DEBUGF(fmt, args...) \
    LOGGER_FIX(liq::LoggerManager::LOG_DEBUG, "", " ").format(fmt "\n", ##args)
#define INFOF(fmt, args...) \
    LOGGER_FIX(liq::LoggerManager::LOG_INFO, "", " ").format(fmt "\n", ##args)
#define WARNF(fmt, args...) \
    LOGGER_FIX(liq::LoggerManager::LOG_WARN, "", " ").format(fmt "\n", ##args)
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

