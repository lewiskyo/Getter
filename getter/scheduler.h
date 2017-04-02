#pragma  once

#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <mutex>
#include <unistd.h>
#include <thread>
#include <stdlib.h>
#include "message.h"
#include "lock.h"
#include "util.h"


namespace getter
{
	// 使用 _create 来表示 void* (*)(string);
	typedef void* (*create)(std::string);   //  对应Actor构造函数
	typedef bool (*init) ();           //  首次进行任务调度时，需要执行,对创建actor时传来的参数做一些操作等.
	typedef void (*dispatch)(void* Actor, Message *msg);  //任务调度函数，接收到客户端消息, 其他Actor消息,定时器消息等等的消息分发处理函数.
	typedef void (*destroy)();               //释放Actor自身的资源.

	typedef struct ActorStruct {
		create _create;
		init _init;
		dispatch _dispatch;
		destroy _destroy;

	} ActorStruct;

	// manage load actorso
	typedef struct ActorStructManager {
		std::mutex mtx;
		std::map<std::string, ActorStruct*> actor_struct_map;

	} ActorStructManager;

	typedef struct Actor {
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

	// manage real actors
	typedef struct ActorManager {
		std::map<uint32_t, Actor*> actor_id_map;
		int id_count;
		std::mutex mtx;

		ActorManager() {
			id_count = 1;
		}

	} ActorManager;

	// run on each thread
	typedef struct Scheduler {

		Scheduler();

		Actor* fetchActor();
		void schedulingActor();
		void workThread();
		void startWorkThread();
		void send(Message*, int);

		void init();
		void run();
		void stop();


	// private:
		Actor* processing_actor;   //指向当前正在处理的Actor.

		std::thread th;

		
	} Scheduler;


	// global message queue
	struct SchedulerMng: private noncopyable {	

		void run();
		void init();
		void stop();
		static SchedulerMng& getSchedulerMng();
		int newActor(std::string);

	// private:
		std::vector<Scheduler*> v_schedulers;

		// double queue lock
		CasLock push_lock;
		CasLock pop_lock;

		std::queue<Actor*> push_queue;
		std::queue<Actor*> pop_queue;

		ActorManager actor_manager;
		ActorStructManager actor_struct_manager;

		// push actor into queue
		void addActorIntoGmq(Actor*);

	};
}