#ifndef SERVER_H
#define SERVER_H

#include <queue>
#include <vector>

#include "lock.h"
#include "scheduler.h"
#include "agent.h"

class Scheduler;

class Server {
    private:
        CasLock prepared_lock;
        CasLock processing_lock;

        std::queue<Agent*> prepared_queue;
        std::queue<Agent*> processing_queue;
        std::vector<Scheduler*> schedulers;

    public:
        void add_agent(Agent* agent);
        Agent* pop_agent();

        void init();
        void run();
        void stop();
};


#endif
