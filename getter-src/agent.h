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

        std::queue<std::string> prepared_queue; //��������Ϣ
        std::queue<std::string> processing_queue; //���ڴ������Ϣ

        // prepared_queue��
        CasLock prepared_lock;

        bool is_schedule; // �Ƿ����ڵ�����
        bool is_init;  // �Ƿ��Ѿ�ִ�г�ʼ������
        int msg_count; // prepared_queue.size() + processing_queue.size()
        int session_id;
        std::string param;

    public:
        Agent(std::string name, std::string param);  
        bool init();
};


#endif
