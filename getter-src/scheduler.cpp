#include "scheduler.h"

class Server;

Scheduler::Scheduler(){
    process_agent = NULL;
}

Agent* Scheduler::fetch() {
    return getter.pop_agent();
}

void Scheduler::Scheduler() {

    if (!process_agent->init) {
        ModuleHelper::getter_module_instance(); 
        process_agent->init = true;
    } else {
        if (process_agent->processing_queue.empty()) {
            MyLockGuard lockguard(&process_agent->prepared_lock);
            process_agent->processing_queue.swap(processing_actor->prepared_queue);
        }

        auto msg = process_agent->processing_queue.front();
        process_agent->processing_queue.pop();

        // real dispatch
        ModuleHelper::getter_module_dispatch(mod,agent,msg); 

        {
            MyLockGuard lockguard(&process_agent->prepared_lock);
            --process_agent->msg_count;

            if (process_agent->msg_count > 0)

            else
                process_agent->schedule = false;
        }

        process_agent = NULL;
    }
}

void Scheduler::work_thread() {
    
    while(true) {
    
        Agent * process_agent = fetch();
        if (process_agent != NULL) {
            scheduling();
        else
            usleep(1000);
        }
    }
}

void Scheduler::start() {
    th = thread(&Scheduler::work_thread, this);
}

