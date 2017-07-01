#ifndef GETTER_CONTEXT_H
#define GETTER_CONTEXT_H

#include <map>
#include <vector>
#include <pthread>

#include "getter_mq.h"

class Context{
public:
   Module *mod; 
   MessageQueue mq; 
   // ¶ÔÓ¦µÄactor±àºÅ
   int handle;
};

class GlobalContext{
public:
    GlobalQueue gmq;
    std::map<int, Context*> mapctx;
    int alloc_id;
    std::vector<pthread_t> pids;
public:
    void init();
    void start();
};

#endif
