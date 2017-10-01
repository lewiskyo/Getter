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

        void log(std::string);  // ������־�ӿ�
        void execute_log();         // �����־

        void init();
        void run();
        void stop();
        void work_thread();

    private:
        CasLock lock;       
        std::list<std::string> prepared_logs; // ���������־�б�
        std::list<std::string> processing_logs; // ���ڴ������־�б�
        std::string log_dir;   // ��־����·��
        int handle;            // ��ǰ����ļ����
        int last_update_time;   // �ϴ���־���ʱ��
        std::thread th;
        std::string cur_log_filename; // ��ǰ�����־�ļ���
    private:
        void update_log_name();

};

#endif
