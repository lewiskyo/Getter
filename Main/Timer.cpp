#include "Timer.h"

struct TimerManager timer_mng;

using namespace std;

long get_current_time()
{
	struct timeval tv;

	gettimeofday(&tv,NULL);

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}


int timer_create(long after_time, uint32_t src, uint32_t interval = 0)
{
	shared_ptr<TimerInfo> timer(new TimerInfo());
	// struct TimerInfo timer;
	timer.get()->expire_time = get_current_time() + after_time;
	timer.get()->src = src;
	timer.get()->interval = interval;
	timer.get()->is_working = true;

	{
		LockGuard guard(&timer_mng.add_timer_lock);

		timer.get()->id = timer_mng.id_count;
		timer_mng.wait_add_timer_list.push_back(timer);

		++timer_mng.id_count;
	}

	return timer.get()->id;
}

void timer_delete(uint32_t timer_id)
{
	LockGuard guard(&timer_mng.delete_timer_lock);
	timer_mng.delete_timer_id_list.push_back(timer_id);
}



void update_timer()
{
	timer_mng.now = get_current_time();
	list<uint32_t> delete_tmp;
	list<shared_ptr<TimerInfo> > add_tmp;

	//先删除定时器 交换元素减少锁的竞争时间
	{
		LockGuard lockguard(&timer_mng.delete_timer_lock);
		delete_tmp.swap(timer_mng.delete_timer_id_list);
	}

	for(list<uint32_t>::iterator list_it = delete_tmp.begin(); list_it != delete_tmp.end(); ++list_it)
	{
		map<uint32_t, shared_ptr<TimerInfo> >::iterator it;

		it = timer_mng.timer_map.find(*list_it);

		if(it != timer_mng.timer_map.end())
		{
			it->second.get()->is_working = false;
			timer_mng.timer_map.erase(it);
		}
	}

	//再加入定时器 交换元素减少锁的竞争时间
	{
		LockGuard lockguard(&timer_mng.add_timer_lock);
		add_tmp.swap(timer_mng.wait_add_timer_list);
	}

	for(list<shared_ptr<TimerInfo> >::iterator list_it = add_tmp.begin(); list_it != add_tmp.end(); ++list_it)
	{
		timer_mng.timer_queue.push(*list_it);
		timer_mng.timer_map.insert(pair<uint32_t, shared_ptr<TimerInfo> >(list_it->get()->id, *list_it));
	}

}

void execute_timer()
{
	while(!timer_mng.timer_queue.empty())
	{	
		// 未超时
		if(timer_mng.timer_queue.top().get()->expire_time > timer_mng.now)
			break;

		if (timer_mng.timer_queue.top().get()->is_working)
		{
			cout<< "execute_timer: " << timer_mng.timer_queue.top().get()->id << ", expired_time: " 
				<< timer_mng.timer_queue.top().get()->expire_time << endl;
		}

		timer_mng.timer_queue.pop();
	}
}

void timer_thread()
{
	while(1)
	{
		update_timer();
		execute_timer();
		usleep(1000);
	}
}

void timer_init()
{

}

void timer_run()
{
	timer_mng.th = thread(timer_thread);
}