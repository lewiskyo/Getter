#ifndef TIMER_H
#define TIMER_H

#include <iostream>
#include <cstdint>
#include <queue>
#include <vector>
#include <map>
#include <time.h>
#include <thread>
#include <unistd.h>
#include <cstdint>
#include <sys/time.h>
#include <list>
// #include "../Tools/Lock.hpp"
#include "Lock.h"


struct TimerInfo
{
	uint32_t id;           //  分配的唯一id
	long expire_time;  //  超时时间(增量 ms)
	uint32_t src;          //  对应是哪个Actor注册的
	uint32_t interval;     //  循环间隔时间(ms)
	
};

struct TimerComp
{
	bool operator()(const TimerInfo& lhs, const TimerInfo& rhs)
	{
		return lhs.expire_time < rhs.expire_time;
	}
};

struct TimerManager
{
	uint32_t id_count;
	CasLock add_timer_lock;
	CasLock delete_timer_lock;

	//TODO value改用shared_ptr
	std::map<uint32_t, TimerInfo> timer_map;
	std::priority_queue<TimerInfo, std::vector<TimerInfo>, TimerComp> timer_queue;

	// 新添加的定时器列表,在update_timer时再加入到优先队列
	std::list<TimerInfo> wait_add_timer_list;

	// 待删除的定时器列表, 再update_timer时再从优先队列和map中删除
	std::list<uint32_t> delete_timer_id_list;

	std::thread th;

	long now;          //  当前时间戳

	TimerManager()
	{
		id_count = 1;
	}
};


// 注册定时器
int timer_create(uint32_t expire_time, uint32_t src, uint32_t interval = 0);
// 删除已经注册的定时器
void timer_delete(uint32_t timer_id);
// 获取当前时间(ms)
long get_current_time();
// 做相关定时器更新操作
void update_timer();
// 执行定时器超时检查操作
void execute_timer();
// 初始化函数
void timer_init();
// 运行函数
void timer_run();


#endif