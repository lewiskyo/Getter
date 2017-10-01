#ifndef AGENT_H
#define AGENT_H

#include <queue>
#include <string>

#include "module.h"
#include "lock.h"

class Agent {
    public:
        void *instance;
        Module* mod;
        std::string name;

        std::queue<std::string> prepared_queue; 
        std::queue<std::string> processing_queue; 

        // prepared_queue
        CasLock prepared_lock;

        bool is_schedule; 
        bool is_init;  
        int msg_count; 
        int session_id;
        std::string param;

    public:
        Agent(std::string name, std::string param);  
        bool init();
};


#endif
