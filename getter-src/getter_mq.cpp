#include "getter_mq.h"

using namespace std;

void GlobalQueue::gq_push(MessageQueue* mq){
    MyLockGuard lockguard(&this->wait_queue_lock);
    wait_queue_lock.push(mq);
}

MessageQueue* GlobalQueue::gq_pop(){
    MyLockGuard processing_guard(&this->processing_queue_lock);
    if (this->processing_queue.empty()){
        MyLockGuard wait_guard(&this->wait_queue_lock);
        this->processing_queue.swap(this->wait_queue);
    }

    if (this->processing_queue.empty())
        return NULL;
    else{
        auto mq = this->processing_queue.front();
        this->processing_queue.pop();
        return mq;
    }
}
