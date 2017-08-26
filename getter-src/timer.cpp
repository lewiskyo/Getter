#include "timer.h"

using namespace std;

TimerMng::TimerMng() {
    id = 1;
}

int TimerMng::create_timer(long after_time, int src, int interval = 0) {

    shared_ptr<TimerInfo> timer(new TimerInfo());
    // struct TimerInfo timer;
    timer.get()->expire_time = 123 + after_time;
    timer.get()->src = src;
    timer.get()->interval = interval;
    timer.get()->is_working = true;

    {
        MyLockGuard guard(&add_timer_lock);

        timer.get()->id = id;
        wait_add_timers.push_back(timer);

        ++id;
    }

    return timer.get()->id;
}

void TimerMng::delete_timer(int timer_id) {

    MyLockGuard guard(&delete_timer_lock);
    delete_timer_ids.push_back(timer_id);
}


void TimerMng::update_timer() {

    this->now =  123;
    list<int> delete_tmp;

    //先删除定时器 交换元素是为了减少锁的竞争时间
    {
        MyLockGuard lockguard(&delete_timer_lock);
        delete_tmp.swap(delete_timer_ids);
    }

    for(list<int>::iterator list_it = delete_tmp.begin(); list_it != delete_tmp.end(); ++list_it)
    {
        map<int, shared_ptr<TimerInfo> >::iterator it;

        it = timer_map.find(*list_it);

        if(it != timer_map.end())
        {
            it->second.get()->is_working = false;
            timer_map.erase(it);
        }
    }

    list<shared_ptr<TimerInfo> > add_tmp;
    //再加入定时器 交换元素减少锁的竞争时间
    {
        MyLockGuard lockguard(&add_timer_lock);
        add_tmp.swap(wait_add_timers);
    }

    for(list<shared_ptr<TimerInfo> >::iterator list_it = add_tmp.begin(); list_it != add_tmp.end(); ++list_it)
    {
        timer_queue.push(*list_it);
        timer_map.insert(pair<int, shared_ptr<TimerInfo> >(list_it->get()->id, *list_it));
    }

}

void TimerMng::execute_timer() {

    while(!timer_queue.empty())
    {	
        // 第一个未超时,后面的也是未超时
        if(timer_queue.top().get()->expire_time > this->now)
            break;

        if (timer_queue.top().get()->is_working)
        {
            //cout<< "execute_timer: " << timer_queue.top().get()->id << ", expired_time: " 
            //    << timer_queue.top().get()->expire_time << endl;
            printf("execute timer: %d, expired_time: %d\n", timer_queue.top().get()->id,
                    timer_queue.top().get()->expire_time);
        }

        timer_queue.pop();
    }
}

void TimerMng::work_thread() {
    while(1) {
        update_timer();
        execute_timer();
        usleep(1000000);
        printf("timermng one loop start\n");
    }
}

void TimerMng::init() {

}

void TimerMng::run() {
    th = thread(&TimerMng::work_thread, this);
}

void TimerMng::stop() {
    th.join();
}

