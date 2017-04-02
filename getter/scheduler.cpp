#include "scheduler.h"

using namespace std;

namespace getter {

struct Scheduler;

SchedulerMng& SchedulerMng::getSchedulerMng() {
    static SchedulerMng scheduler_mng;
    return scheduler_mng;
}

void SchedulerMng::addActorIntoGmq(Actor* actor) {

	MyLockGuard lockguard(&push_lock);
	push_queue.push(actor);
}

int SchedulerMng::newActor(string name) {

	ActorStruct* actor_struct = NULL;
	{

		lock_guard<mutex> lck(actor_struct_manager.mtx);

		auto it = actor_struct_manager.actor_struct_map.find(name);

		if (it != actor_struct_manager.actor_struct_map.end())
		{
			actor_struct = it->second;
		}
		else
		{
			void* handle = util::loadSo(name);
			ActorStruct* new_as = new ActorStruct();
			new_as->_create = (void*(*)(string))dlsym(handle, "create");
			new_as->_init = (bool(*)())dlsym(handle, "init");
			new_as->_dispatch = (void(*)(void*, Message*))dlsym(handle, "dispatch");
			new_as->_destroy = (void(*)())dlsym(handle, "destroy");

			actor_struct_manager.actor_struct_map.insert(pair<string, ActorStruct*>(name, new_as));

			actor_struct = new_as;
		}
	}

	Actor* actor = new Actor();
	actor->actor_struct = actor_struct;

	{
		lock_guard<mutex> lck(actor_manager.mtx);
		actor->id = actor_manager.id_count++;

		actor_manager.actor_id_map.insert(pair<uint32_t, Actor*>(actor->id, actor));

	}

	// 加入调度 进行init
	addActorIntoGmq(actor);

	return actor->id;

}

void SchedulerMng::init() {

	int thread_count = 3;

	for (int i = 0; i < thread_count; ++i)
	{
		Scheduler* scheduler = new Scheduler();
		v_schedulers.push_back(scheduler);
	}

	// new some actor
	
	newActor("Example");
}

void SchedulerMng::run() {
	for(auto iter = v_schedulers.begin(); iter != v_schedulers.end(); ++iter)
	{
		(*iter)->startWorkThread();
		// (*iter)->th = thread(&Scheduler::workThread, *iter);
	}

	while(actor_manager.actor_id_map.size() > 0)
	{
		//do something check
		
		sleep(1);
	}

}

void SchedulerMng::stop() {

	for(auto iter = v_schedulers.begin(); iter != v_schedulers.end(); ++iter)
	{
		(*iter)->th.join();
	}
}



Scheduler::Scheduler(){
	processing_actor = NULL;
}

Actor* Scheduler::fetchActor() {

	SchedulerMng &scheduler_mng = SchedulerMng::getSchedulerMng();

	MyLockGuard lockguard(&scheduler_mng.pop_lock);	
	if (scheduler_mng.pop_queue.empty()) {

		MyLockGuard lockguard(&scheduler_mng.push_lock);
		scheduler_mng.pop_queue.swap(scheduler_mng.push_queue);
	}

	if (scheduler_mng.pop_queue.empty())
		return NULL;
	else {
		auto actor = scheduler_mng.pop_queue.front();
		scheduler_mng.pop_queue.pop();

		return actor;
	}
}




void Scheduler::schedulingActor() {

	SchedulerMng &scheduler_mng = SchedulerMng::getSchedulerMng();

	// 未初始化 先进行初始化
	if (!processing_actor->is_inited)
	{
		processing_actor->actor_struct->_init();
		processing_actor->is_inited = true;
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
		processing_actor->actor_struct->_dispatch(processing_actor, msg);

		{
			MyLockGuard lockguard(&processing_actor->push_queue_lock);
			--processing_actor->msg_count;

			if (processing_actor->msg_count > 0)
				scheduler_mng.addActorIntoGmq(processing_actor);
			else
				processing_actor->is_scheduling = false;
		}

		processing_actor = NULL;

	}
}


void Scheduler::workThread() {

	SchedulerMng &scheduler_mng = SchedulerMng::getSchedulerMng();

	while(true) {

		processing_actor = fetchActor();
		if(processing_actor != NULL) {
			schedulingActor();
		}
		else
			usleep(1000);
	}
}

void Scheduler::startWorkThread() {
	th = thread(&Scheduler::workThread, this);
}


void Scheduler::send(Message* msg, int des) {

	SchedulerMng &scheduler_mng = SchedulerMng::getSchedulerMng();

	auto it = scheduler_mng.actor_manager.actor_id_map.find(des);

	if(it != scheduler_mng.actor_manager.actor_id_map.end()) {
		auto actor = it->second;
		{
			MyLockGuard lockguard(&actor->push_queue_lock);
			actor->push_queue.push(msg);
			++actor->msg_count;

			if (!actor->is_scheduling) {
				actor->is_scheduling = true;
				scheduler_mng.addActorIntoGmq(actor);
			}
		}
	}
}

}
