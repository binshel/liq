#include <string.h>
#include <errno.h>

#include "liq/liq.h"
#include "liq/logger.h"

namespace liq {

    int LoggerFile::init(const std::string &node_name, size_t buff_size)
    {
        snprintf(filepath, sizeof(filepath), "%s/logs/%s.log", getenv("FRAME_HOME"), node_name.c_str());
        this->file = fopen(filepath, "ab");
        if (!this->file) {
            fprintf(stderr, "can't open logger file[%s]: %s\n", filepath, strerror(errno));
            exit(-1);
        }
        if (setvbuf(this->file, NULL, _IOLBF, buff_size) ==0) {
            return 0;
        } else {
            fclose(this->file);
            this->file = NULL;
            fprintf(stderr, "setvbuf error[%s]: %s\n", filepath, strerror(errno));
            exit(-1);
        }
    }

    LoggerManager::LoggerManager(const std::string &node_name, LoggerManager::LogLevel level)
        :level (level)
    {
        this->level = LOG_INFO;
        this->none = new LoggerNone();
        this->file = new LoggerFile();
        this->file->init(node_name);
        LOGGER_DO(this, liq::LoggerManager::LOG_INFO, "\n", " ").format("LoggerManager inited");
    }
    ILogger &LoggerManager::get(LoggerManager::LogLevel level) 
    {
        if (level >= this->level) return *file;
        else return  *none;
    }
    void LoggerManager::ontick()
    {
        this->file->flush();
    }
}

