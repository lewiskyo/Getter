#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <thread>
#include <queue>

#include "lock.h"
#include "agent.h"
#include "server.h"

class Scheduler {
    public:
        Scheduler();
        Agent * fetch();
        void work_thread();
        void start();
        void scheduling();
        
    public:
        Agent * process_agent; // 处理中的Agent
        std::thread th;
};

#endif
