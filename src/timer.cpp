#include "timer.h"

using namespace std;

TimerMng::TimerMng() {
    id = 1;
}

int TimerMng::createTimer(long afterTime, int src, int interval = 0) {

    shared_ptr<TimerInfo> timer(new TimerInfo());

    /* Struct TimerInfo timer */
    timer.get()->expireTime = 123 + afterTime;
    timer.get()->src = src;
    timer.get()->interval = interval;
    timer.get()->isWorking = true;

    {
        LockGuard guard(&addTimerLock);

        timer.get()->id = id;
        waitAddTimers.push_back(timer);

        ++id;
    }

    return timer.get()->id;
}

void TimerMng::deleteTimer(int timerId) {

    LockGuard guard(&deleteTimerLock);
    deleteTimerIds.push_back(timerId);
}


void TimerMng::updateTimer() {

    this->now =  123;
    list<int> deleteTmp;

    /* Delete timer first, swap for deduce lock compete */
    {
        LockGuard guard(&deleteTimerLock);
        deleteTmp.swap(deleteTimerIds);
    }

    for(list<int>::iterator list_it = deleteTmp.begin(); list_it != deleteTmp.end(); ++list_it)
    {
        map<int, shared_ptr<TimerInfo> >::iterator it;

        it = timerMap.find(*list_it);

        if(it != timerMap.end())
        {
            it->second.get()->isWorking = false;
            timerMap.erase(it);
        }
    }

    list<shared_ptr<TimerInfo> > addTmp;
    /* Add timer again, swap for reduce lock compete */
    {
        LockGuard lockguard(&addTimerLock);
        addTmp.swap(waitAddTimers);
    }

    for(list<shared_ptr<TimerInfo> >::iterator list_it = addTmp.begin(); list_it != addTmp.end(); ++list_it)
    {
        timerQueue.push(*list_it);
        timerMap.insert(pair<int, shared_ptr<TimerInfo> >(list_it->get()->id, *list_it));
    }

}

void TimerMng::executeTimer() {

    while(!timerQueue.empty())
    {	
        /* Find the first not over time, behind is not over time too, so break */
        if(timerQueue.top().get()->expireTime > this->now)
            break;

        if (timerQueue.top().get()->isWorking)
        {
            //cout<< "execute_timer: " << timer_queue.top().get()->id << ", expired_time: " 
            //    << timer_queue.top().get()->expire_time << endl;
            printf("execute timer: %d, expired_time: %ld\n", timerQueue.top().get()->id,
            timerQueue.top().get()->expireTime);
        }

        timerQueue.pop();
    }
}

void TimerMng::workThread() {
    while(1) {
        this->updateTimer();
        this->executeTimer();
        usleep(1000000);
        printf("timermng one loop start\n");
    }
}

void TimerMng::init() {

}

void TimerMng::run() {
    th = thread(&TimerMng::workThread, this);
}

void TimerMng::stop() {
    th.join();
}

