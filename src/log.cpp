#include "log.h"

using namespace std;

Logger::Logger(string _logDir):logDir(_logDir) {
}

void Logger::init(){
    lastUpdateTime = time((time_t*)NULL);
}

void Logger::run() {
    th = thread(&Logger::workThread, this);
}

void Logger::stop() {
    th.join();
}

void Logger::executeLog() {
    {
        LockGuard guard(&lock);
        processingLogs.swap(preparedLogs);
    }

    if (processingLogs.size() == 0) {
        return;
    }

    updateLogName();
}

void Logger::updateLogName() {
    int now = time((time_t*)NULL);
    if ( curLogFilename.empty() && util::isSameHour(now, lastUpdateTime)) {
        return;
    }
    
    time_t t =  time(NULL);

    struct tm *tmpTIme = localtime(&t);

    char tmp[64];
    strftime(tmp, sizeof(tmp), "%04Y%02m%02d%H", tmpTIme);
    string newFilename(logDir + tmp);
    
    handle = open(curLogFilename.c_str(), O_APPEND);
    if (handle == -1) {
        fprintf(stdout, "open logfilename %s err", curLogFilename.c_str());
        exit(1);
    }
}

void Logger::log(string content) {
    LockGuard guard(&lock);
    preparedLogs.push_back(content);
}

void Logger::workThread() {
    while(1) {
        executeLog();
        usleep(1000000);
        log("one log loop process finish\n");
    }
}
