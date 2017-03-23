#include "Scheduler.h"

using namespace std;

struct Getter::SchedulerManager scheduler_manager;


Getter::Actor* Getter::fetch_actor()
{
	MyLockGuard lockguard(&scheduler_manager.pop_lock);	
	if (scheduler_manager.pop_queue.empty())
	{
		MyLockGuard lockguard(&scheduler_manager.push_lock);
		scheduler_manager.pop_queue.swap(scheduler_manager.push_queue);
	}

	if (scheduler_manager.pop_queue.empty())
		return NULL;
	else
	{
		auto actor = scheduler_manager.pop_queue.front();
		scheduler_manager.pop_queue.pop();

		return actor;
	}
}

void Getter::add_actor_into_queue(Actor* actor)
{
	MyLockGuard lockguard(&scheduler_manager.push_lock);
	scheduler_manager.push_queue.push(actor);
}


void Getter::scheduling_actor(Scheduler* scheduler)
{
	auto processing_actor = scheduler->processing_actor;

	// 未初始化 先进行初始化
	if (processing_actor->is_inited)
	{
		processing_actor->actor_struct->_init();	
	}
	// 进行实际调度
	else
	{
		if (processing_actor->push_queue.empty())
		{
			MyLockGuard lockguard(&processing_actor->push_queue_lock);
			processing_actor->pop_queue.swap(processing_actor->push_queue);
		}

		auto msg = processing_actor->pop_queue.front();
		processing_actor->pop_queue.pop();

		// 消息调度
		processing_actor->actor_struct->_dispatch(msg);

		{
			MyLockGuard lockguard(&processing_actor->push_queue_lock);
			--processing_actor->msg_count;

			if (processing_actor->msg_count > 0)
				add_actor_into_queue(processing_actor);
		}

		scheduler->processing_actor = NULL;

	}
}


void Getter::scheduler_thread(Scheduler* scheduler)
{
	while(true)
	{
		scheduler->processing_actor = fetch_actor();
		if(scheduler->processing_actor != NULL)
		{
			scheduling_actor(scheduler);
		}
		else
			usleep(1000);
	}
}



void Getter::initial()
{
	int thread_count = 3;

	for (int i = 0; i < thread_count; ++i)
	{
		Scheduler* scheduler = new Scheduler();
		scheduler_manager.v_schedulers.push_back(scheduler);
	}
}

void Getter::run()
{
	for(int i = 0; i < scheduler_manager.v_schedulers.size(); ++i)
	{
		scheduler_manager.v_schedulers[i]->th = thread(scheduler_thread, scheduler_manager.v_schedulers[i]);
	}

	while(scheduler_manager.actor_manager.actor_id_map.size() > 0)
	{
		//do something check
		
		sleep(1);
	}
}


void Getter::send(Message* msg, int des)
{
	auto it = scheduler_manager.actor_manager.actor_id_map.find(des);

	if(it != scheduler_manager.actor_manager.actor_id_map.end())
	{
		auto actor = it->second;
		{
			MyLockGuard lockguard(&actor->push_queue_lock);
			actor->push_queue.push(msg);
			++actor->msg_count;
		}
	}
}