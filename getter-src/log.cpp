#include "log.h"

using namespace std;

Logger::Logger(string _log_dir):log_dir(_log_dir) {
}

void Logger::init(){
    last_update_time = time((time_t*)NULL);
}

void Logger::run() {
    th = thread(&Logger::work_thread, this);
}

void Logger::stop() {
    th.join();
}

void Logger::execute_log() {
    {
        MyLockGuard guard(&lock);
        processing_logs.swap(prepared_logs);
    }

    if (processing_logs.size() == 0) {
        return;
    }

    update_log_name();
    
}

void Logger::update_log_name() {
    int now = time((time_t*)NULL);
    if ( cur_log_filename.empty() and util::isSameHour(now, last_update_time)) {
        return;
    }
    
    // ���������־�ļ�
    char tmp[64];
    string new_filename(strftime(tmp,sizeof(tmp), "%04Y%02m%02d%H", now));
    cur_log_filename = log_dir + tmp;
    
    handle = open(cur_log_filename, O_APPEND);
    if (handle == -1) {
        fprintf(stdout, "open logfilename %s err", cur_log_filename);
        exit(1);
    }
}

void Logger::log(string content) {
    MyLockGuard guard(&lock);
    prepared_logs.push_back(content);
}

void Logger::work_thread() {
    while(1) {
        execute_log();
        usleep(1000000);
        log("one log loop process finish\n");
    }
}
