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
        void workThread();
        void start();
        void scheduling();
        
    public:
        Agent * processAgent; 
        std::thread th;
};

#endif
