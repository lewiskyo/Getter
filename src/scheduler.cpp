#include "scheduler.h"
using namespace std;

class Server;
extern Server getter;

Scheduler::Scheduler(){
    process_agent = NULL;
}

Agent* Scheduler::fetch() {
    return getter.pop_agent();
}

void Scheduler::scheduling() {

    if (!process_agent->is_init) {
        process_agent->init();
        process_agent->is_init = true;
    } else {
        if (process_agent->processing_queue.empty()) {
            MyLockGuard lockguard(&process_agent->prepared_lock);
            process_agent->processing_queue.swap(process_agent->prepared_queue);
        }

        auto msg = process_agent->processing_queue.front();
        process_agent->processing_queue.pop();

        // real dispatch
        ModuleHelper::getter_module_instance_dispatch(process_agent->mod,process_agent,&msg); 

        {
            MyLockGuard lockguard(&process_agent->prepared_lock);
            --process_agent->msg_count;

            if (process_agent->msg_count > 0) {

            } else {
                process_agent->is_schedule = false;
            }

            process_agent = NULL;
        }
    }
}

void Scheduler::work_thread() {

    while(true) {

        Agent * process_agent = fetch();
        if (process_agent != NULL) {
            scheduling();
        } else {
            printf("nothing to do, sleep\n");
            usleep(1000000);
        }
    }
}

void Scheduler::start() {
    th = thread(&Scheduler::work_thread, this);
}
