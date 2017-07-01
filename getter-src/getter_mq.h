#idndef GETTER_MQ_H
#define GETTER_MQ_H

#include <string>
#include <queue>

#include "lock.h" 

class MessageQueue{
public:
    // mq��Ӧ��actorHandler���
    int handle;
    // ˫����
    std::queue<std::string> wait_queue;  // ���������Ϣ
    std::queue<std::string> processing_queue; // ���ڴ������Ϣ

    // wait_queue����
    CasLock wait_queue_lock;
    // ����Ϣ����
    int msg_count; 

public:
    
};


class GlobalQueue{
public: 
    // ˫����
    std::queue<MessageQueue*> wait_queue;
    std::queue<MessageQueue*> processing_queue;
    
    // wait_queue����
    CasLock wait_queue_lock;
    // processing_queue����
    CasLock processing_queue_lock;
public:
    void gq_push(MessageQueue* mq);
    MessageQueue* gq_pop();
};

#endif
