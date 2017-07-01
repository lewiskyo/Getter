#idndef GETTER_MQ_H
#define GETTER_MQ_H

#include <string>
#include <queue>

#include "lock.h" 

class MessageQueue{
public:
    // mq对应的actorHandler编号
    int handle;
    // 双队列
    std::queue<std::string> wait_queue;  // 待处理的消息
    std::queue<std::string> processing_queue; // 正在处理的消息

    // wait_queue的锁
    CasLock wait_queue_lock;
    // 总消息数量
    int msg_count; 

public:
    
};


class GlobalQueue{
public: 
    // 双队列
    std::queue<MessageQueue*> wait_queue;
    std::queue<MessageQueue*> processing_queue;
    
    // wait_queue的锁
    CasLock wait_queue_lock;
    // processing_queue的锁
    CasLock processing_queue_lock;
public:
    void gq_push(MessageQueue* mq);
    MessageQueue* gq_pop();
};

#endif
