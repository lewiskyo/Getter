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

        std::queue<std::string> prepared_queue; //待处理消息
        std::queue<std::string> processing_queue; //正在处理的消息

        // prepared_queue锁
        CasLock prepared_lock;

        bool schedule; // 是否正在调度中
        bool init;  // 是否已经执行初始化工作
        int msg_count; // prepared_queue.size() + processing_queue.size()
        int session_id;

    public:
        Agent(std::string name, std::string param);  
};


#endif
