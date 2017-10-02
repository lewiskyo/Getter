#ifndef LOG_H
#define LOG_H

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

        void log(std::string);  
        void executeLog();     

        void init(std::string _logDir);
        void run();
        void stop();
        void workThread();

    private:
        CasLock lock;       
        std::list<std::string> preparedLogs; 
        std::list<std::string> processingLogs; 
        std::string logDir;   
        int handle;            
        int lastUpdateTime;   
        std::thread th;
        std::string curLogFilename; 
    private:
        void updateLogName();

};

#endif
