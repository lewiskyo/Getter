#include "server.h"
using namespace std;

void Server::addAgent(Agent* agent) {
    LockGuard guard(&this->preparedLock);
    preparedQueue.push(agent);
}

Agent * Server::popAgent() {
   LockGuard proGuard(&this->processingLock);
   if (this->processingQueue.empty()) {
        LockGuard preGuard(&this->preparedLock);
        processingQueue.swap(this->preparedQueue);
   } 

   if (this->preparedQueue.empty()) {
       return NULL;
   } else {
       auto agent = processingQueue.front();
       processingQueue.pop();

       return agent;
   }
}

void Server::init() {

    /* todo in config*/
    int threadCount = 3;
    
    for (auto i = 0; i < threadCount; ++i) {
        auto scheduler = new Scheduler();
        schedulers.push_back(scheduler);
    }

    // todo boot
}

void Server::run() {
    
    for (auto iter = schedulers.begin(); iter != schedulers.end(); ++iter) {
        (*iter)->start();
    }

    while(1) {
        sleep(1);
    }
}

void Server::stop() {

    for (auto iter = schedulers.begin(); iter != schedulers.end(); ++iter) {
        (*iter)->th.join();
    }
}
