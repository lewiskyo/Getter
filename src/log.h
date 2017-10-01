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

        void log(std::string);  
        void execute_log();     

        void init();
        void run();
        void stop();
        void work_thread();

    private:
        CasLock lock;       
        std::list<std::string> prepared_logs; 
        std::list<std::string> processing_logs; 
        std::string log_dir;   
        int handle;            
        int last_update_time;   
        std::thread th;
        std::string cur_log_filename; 
    private:
        void update_log_name();

};

#endif
