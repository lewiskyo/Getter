#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <mutex>
#include <unistd.h>
#include <thread>
#include <stdlib.h>
#include <dlfcn.h>
#include "../Tools/Message.h"
// #include "Message.h"
#include "../Tools/Lock.h"
// #include "Lock.h"


namespace Scheduler
{
	// 使用 _create 来表示 void* (*)(string);
	typedef void* (*create)(std::string);   //  对应Actor构造函数
	typedef bool (*init) ();           //  首次进行任务调度时，需要执行,对创建actor时传来的参数做一些操作等.
	typedef void (*dispatch)(void* Actor, Message *msg);  //任务调度函数，接收到客户端消息, 其他Actor消息,定时器消息等等的消息分发处理函数.
	typedef void (*destroy)();               //释放Actor自身的资源.

	typedef struct ActorStruct
	{
		create _create;
		init _init;
		dispatch _dispatch;
		destroy _destroy;

	} ActorStruct;

	// 用于管理加载的Actor
	typedef struct ActorStructManager
	{
		std::mutex mtx;
		std::map<std::string, ActorStruct*> actor_struct_map;

	} ActorStructManager;

	typedef struct Actor
	{
		int id;
		std::string name;    //对应ActorStruct服务名字
		ActorStruct *actor_struct;
		bool is_scheduling;  // 是否正在调度
		bool is_inited; //  是否已经执行初始化工作

		//队列锁
		CasLock push_queue_lock;

		//双队列
		std::queue<Message*> push_queue;   //待处理消息
		std::queue<Message*> pop_queue;   //正在处理的消息

		int msg_count;

		Actor()
		{
			is_scheduling = false;
			is_inited = false;
			msg_count = 0;
		}

	} Actor;

	// 管理实际的Actor
	typedef struct ActorManager
	{
		std::map<uint32_t, Actor*> actor_id_map;
		int id_count;
		std::mutex mtx;

		ActorManager()
		{
			id_count = 1;
		}

	} ActorManager;

	// 调度单元 跑在对应工作线程上
	typedef struct Scheduler
	{
		Actor* processing_actor;   //指向当前正在处理的Actor.

		std::thread th;

		Scheduler()
		{
			processing_actor = NULL;
		}
		
	} Scheduler;


	// 全局调度器
	struct SchedulerManager
	{
		std::vector<Scheduler*> v_schedulers;

		// 对应双队列锁
		CasLock push_lock;
		CasLock pop_lock;

		std::queue<Actor*> push_queue;
		std::queue<Actor*> pop_queue;

		ActorManager actor_manager;
		ActorStructManager actor_struct_manager;

	};



	// 调度过程中涉及到的函数

	// 尝试从SchedulerManager的任务队列中获取Actor.
	Actor* fetch_actor();
	// 将Actor加入调度
	void add_actor_into_queue(Actor*);
	// 进行实际的调度
	void scheduling_actor(Scheduler*);
	// 工作线程函数
	void scheduler_thread(Scheduler*);
	// 发送消息到指定的Actor
	void send(Message*, int);

	void initial();

	void run();

	void stop();

	int new_actor(std::string);

	void* load_so(std::string);
}



#endif