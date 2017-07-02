#include "server.h"

void Server::add_agent(Agent* agent) {
    MyLockGuard lockguard(&this->prepared_lock);
    prepared_queue.push(agent);
}

Agent * Server::pop_agent() {
   MyLockGuard pro_lockguard(&this->processing_lock);
   if (this->processing_queue.empty()) {
        MyLockGuard pre_lockguard(&this->prepared_lock);
        processing_queue.swap(prepared_queue);
   } 

   if (this->processing_queue.empty()) {
       return NULL;
   } else {
       auto agent = processing_queue.front();
       processing_queue.pop();

       return agent;
   }
}

void Server::init() {

    int thread_count = 3;
    
    for (auto i = 0; i < thread_count; ++i) {
        auto scheduler = new Scheduler();
        schedulers.push_back(scheduler);
    }
    // todo boostrasp
}

void Server::run() {
    
    for (auto iter = schedulers.begin(); iter != schedulers.end(); ++iter) {
        (*iter)->th = thread((*iter)->work_thread,(*iter));
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
