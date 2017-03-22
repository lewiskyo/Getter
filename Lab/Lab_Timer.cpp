/*
author: Lewis
 
实现效率：

add_timer 已序堆中添加数据 O(log(N)).

execute_timer 最小已序堆中删除根节点,选取新的根节点 O(log(N)).

delete_timer 只需从map中找到对应timer,然后将is_working字段赋值为false即可,待
execute_timer时一并检查即可.

*/


#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <stdlib.h>
#include <algorithm>
#include <queue>

using namespace std;

#define THREAD_COUNT 4

typedef struct TimerInfo
{
    int id;
    long expired;     //超时时间戳(ms)
    bool is_working;  //用于判断该定时器是否有效
    bool is_repeat;  // 是否为循环定时器标识
    long interval; //循环定时器循环间隔

} TimerInfo;

// 排序函数
class comp
{
public:
    bool operator()(const shared_ptr<TimerInfo> &t1, const shared_ptr<TimerInfo> &t2)
    {
        if(t1.get()->expired > t2.get()->expired)
            return true;
        else
            return false;
    }
};


typedef struct TimerMng
{
    int id;
    
    priority_queue<shared_ptr<TimerInfo>, vector<shared_ptr<TimerInfo> >, comp> timer_queue;   // 优先队列
    map<int, shared_ptr<TimerInfo> > timer_map;   //id到定时器的映射 用于根据id取消定时器

    mutex mtx;

    struct timeval now;

    TimerMng()
    {
        id = 0;
    }

} TimerMng;

TimerMng t_mng;

void add_timer(bool is_repeat, long expired, long interval)
{
    TimerInfo *t_info = new TimerInfo();
    t_info->is_repeat = is_repeat;
    t_info->expired = expired;
    t_info->interval = interval;
    t_info->is_working = true;

    shared_ptr<TimerInfo> timer_info(t_info);

    lock_guard<mutex> guard(t_mng.mtx);
    int now_id = t_mng.id++;
    t_info->id = now_id;

    t_mng.timer_queue.push(timer_info);

    t_mng.timer_map.insert(pair<int, shared_ptr<TimerInfo> >(now_id, timer_info) );

}

void update_time()
{
    gettimeofday(&t_mng.now, NULL);
}

void execute_timer()
{
    long now_usec = t_mng.now.tv_sec * 1000 + t_mng.now.tv_usec / 1000;

    while(true) 
    {
        lock_guard<mutex> guard(t_mng.mtx);

        if (t_mng.timer_queue.empty())
            break;

        shared_ptr<TimerInfo> first = t_mng.timer_queue.top();

        if (first.get()->expired > now_usec )
            break;

        if (first.get()->is_working)
        {
            // do something here
            cout << "timer execute succ, now: " << now_usec <<  " id: " << first.get()->id << " " << "expired: " << first.get()->expired << endl;
        }

        map<int, shared_ptr<TimerInfo> >::iterator map_iter = t_mng.timer_map.find( first.get()->id);

        // 从map中移除
        t_mng.timer_map.erase(map_iter);

        // 从队列中移除
        t_mng.timer_queue.pop();

    }
}


void timer_thread()
{
    while(1)
    {
        update_time();
        execute_timer();
        // 1ms 1次循环
        usleep(1000);
    }
}

void worker_thread()
{
    srand((unsigned)time(0));
    while(1)
    {
        struct timeval now;
        gettimeofday(&now, NULL);

        int rand_sec = rand() % 5 + 1;
        int rand_usec = rand() % 900000 + 1;

        long expired = (now.tv_sec + rand_sec) * 1000 + ( rand_usec / 1000 );
        add_timer(false, expired, 0);

        sleep(rand() % 2 + 1);    
    }
}


int main()
{
    thread t1(timer_thread);

    vector<thread> v_thread;

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        v_thread.push_back(thread(worker_thread));
    }

    t1.join();

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        v_thread[i].join();
    }
}