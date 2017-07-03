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
    int id;           //  分配的唯一id
    long expire_time;  //  超时时间(增量 ms)
    int src;          //  对应是哪个Actor注册的
    int interval;     //  循环间隔时间(ms)
    bool is_working;       //  标记是否还在工作
} TimerInfo;

struct TimerComp {
    bool operator()(const std::shared_ptr<TimerInfo>& lhs, const std::shared_ptr<TimerInfo>& rhs)
    {
        return lhs.get()->expire_time < rhs.get()->expire_time;
    }
};

class TimerMng {
    public:
        TimerMng();

        int create_timer(long, int, int);
        void delete_timer(int);

        void update_timer();
        void execute_timer();

        void init();
        void run();
        void stop();

        void work_thread();

    private:
        int id;
        CasLock add_timer_lock;
        CasLock delete_timer_lock;

        std::map<int, std::shared_ptr<TimerInfo> > timer_map;
        std::priority_queue<std::shared_ptr<TimerInfo>, std::vector<std::shared_ptr<TimerInfo> >, TimerComp> timer_queue;

        // 新添加的定时器列表,在update_timer时再加入到优先队列
        std::list<std::shared_ptr<TimerInfo> > wait_add_timers;

        // 待删除的定时器列表, 在update_timer时再从优先队列和map中删除
        std::list<int> delete_timer_ids;

        std::thread th;

        long now;          //  当前时间戳
};

#endif
