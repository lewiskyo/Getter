#include "scheduler.h"
using namespace std;

class Server;
extern Server server;

Scheduler::Scheduler(){
    processAgent = NULL;
}

Agent* Scheduler::fetch() {
    return server.popAgent();
}

void Scheduler::scheduling() {

    if (!processAgent->isInit) {
        processAgent->init();
        processAgent->isInit = true;
    } else {
        if (processAgent->processingQueue.empty()) {
            LockGuard lockguard(&processAgent->preparedLock);
            processAgent->processingQueue.swap(processAgent->processingQueue);
        }

        auto msg = processAgent->processingQueue.front();
        processAgent->processingQueue.pop();

        // real dispatch
        ModuleHelper::modInstanceDispatch(processAgent->mod, processAgent, &msg); 

        {
            LockGuard guard(&processAgent->preparedLock);
            --processAgent->msgCount;

            if (processAgent->msgCount > 0) {
                // todo add to gq
            } else {
                processAgent->isSchedule = false;
            }

            processAgent = NULL;
        }
    }
}

void Scheduler::workThread() {

    while(true) {

        Agent * processAgent = this->fetch();
        if (processAgent != NULL) {
           this-> scheduling();
        } else {
            printf("nothing to do, sleep\n");
            usleep(1000000);
        }
    }
}

void Scheduler::start() {
    th = thread(&Scheduler::workThread, this);
}
