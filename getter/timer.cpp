#include "timer.h"

using namespace std;

namespace getter {

TimerMng::TimerMng() {
	id_count = 1;
}

TimerMng& TimerMng::getTimerMng() {
    static TimerMng timer_mng;
    return timer_mng;
}


int TimerMng::create_timer(long after_time, uint32_t src, uint32_t interval = 0) {

	shared_ptr<TimerInfo> timer(new TimerInfo());
	// struct TimerInfo timer;
	timer.get()->expire_time = util::getCurrentTime() + after_time;
	timer.get()->src = src;
	timer.get()->interval = interval;
	timer.get()->is_working = true;

	{
		MyLockGuard guard(&add_timer_lock);

		timer.get()->id = id_count;
		wait_add_timer_list.push_back(timer);

		++id_count;
	}

	return timer.get()->id;
}

void TimerMng::deleteTimer(uint32_t timer_id) {

	MyLockGuard guard(&delete_timer_lock);
	delete_timer_id_list.push_back(timer_id);
}



void TimerMng::updateTimer() {

	now = util::getCurrentTime();
	list<uint32_t> delete_tmp;
	list<shared_ptr<TimerInfo> > add_tmp;

	//先删除定时器 交换元素减少锁的竞争时间
	{
		MyLockGuard lockguard(&delete_timer_lock);
		delete_tmp.swap(delete_timer_id_list);
	}

	for(list<uint32_t>::iterator list_it = delete_tmp.begin(); list_it != delete_tmp.end(); ++list_it)
	{
		map<uint32_t, shared_ptr<TimerInfo> >::iterator it;

		it = timer_map.find(*list_it);

		if(it != timer_map.end())
		{
			it->second.get()->is_working = false;
			timer_map.erase(it);
		}
	}

	//再加入定时器 交换元素减少锁的竞争时间
	{
		MyLockGuard lockguard(&add_timer_lock);
		add_tmp.swap(wait_add_timer_list);
	}

	for(list<shared_ptr<TimerInfo> >::iterator list_it = add_tmp.begin(); list_it != add_tmp.end(); ++list_it)
	{
		timer_queue.push(*list_it);
		timer_map.insert(pair<uint32_t, shared_ptr<TimerInfo> >(list_it->get()->id, *list_it));
	}

}

void TimerMng::executeTimer() {

	while(!timer_queue.empty())
	{	
		// 未超时
		if(timer_queue.top().get()->expire_time > now)
			break;

		if (timer_queue.top().get()->is_working)
		{
			cout<< "execute_timer: " << timer_queue.top().get()->id << ", expired_time: " 
				<< timer_queue.top().get()->expire_time << endl;
		}

		timer_queue.pop();
	}
}

void TimerMng::workThread() {
	while(1)
	{
		updateTimer();
		executeTimer();
		usleep(1000);
	}
}

void TimerMng::initial() {

}

void TimerMng::run() {
	th = thread(&TimerMng::workThread, this);
}

void TimerMng::stop() {
	th.join();
}

}