#ifndef TIMER_H
#define TIMER_H

#include <queue>
#include <vector>
#include <map>
#include <time.h>
#include <thread>
#include <unistd.h>
#include <sys/time.h>
#include <list>
#include <memory>

#include "lock.h"


typedef struct TimerInfo {
    /*  Unique id */
    int id; 
    /* Over timestamp (ms) */
    long expireTime;  
    /*  record which actor register */
    int src;         
    /* If need cycle, record the cyscle time (ms) */
    int interval;
    /* Is still working flag */
    bool isWorking;
} TimerInfo;

struct TimerComp {
    bool operator()(const std::shared_ptr<TimerInfo>& lhs, const std::shared_ptr<TimerInfo>& rhs)
    {
        return lhs.get()->expireTime < rhs.get()->expireTime;
    }
};

class TimerMng {
    public:
        TimerMng();

        int createTimer(long, int, int);
        void deleteTimer(int);

        void updateTimer();
        void executeTimer();

        void init();
        void run();
        void stop();

        void workThread();

    private:
        int id;
        CasLock addTimerLock;
        CasLock deleteTimerLock;

        std::map<int, std::shared_ptr<TimerInfo> > timerMap;
        std::priority_queue<std::shared_ptr<TimerInfo>, std::vector<std::shared_ptr<TimerInfo> >, TimerComp> timerQueue;

        /* New  add timer list,  add on updateTImer function */
        std::list<std::shared_ptr<TimerInfo> > waitAddTimers;

        /* Waiting delete timer list, delete on updateTimer function. */
        std::list<int> deleteTimerIds;

        std::thread th;

        /*  Current timestamp */
        long now;          
};

#endif
