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
        CasLock preparedLock;
        CasLock processingLock;

        std::queue<Agent*> preparedQueue;
        std::queue<Agent*> processingQueue;
        std::vector<Scheduler*> schedulers;

    public:
        void addAgent(Agent* agent);
        Agent* popAgent();

        void init();
        void run();
        void stop();
};


#endif
