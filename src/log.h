#ifndef LOG_H
#define LOG_G

#include <string>
#include <list>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <thread>

#include "lock.h"
#include "util.h"


class Logger {
    public:
        Logger(std::string _log_dir);

        void log(std::string);  // 增加日志接口
        void execute_log();         // 输出日志

        void init();
        void run();
        void stop();
        void work_thread();

    private:
        CasLock lock;       
        std::list<std::string> prepared_logs; // 待处理的日志列表
        std::list<std::string> processing_logs; // 正在处理的日志列表
        std::string log_dir;   // 日志放置路径
        int handle;            // 当前输出文件句柄
        int last_update_time;   // 上次日志输出时间
        std::thread th;
        std::string cur_log_filename; // 当前输出日志文件名
    private:
        void update_log_name();

};

#endif
