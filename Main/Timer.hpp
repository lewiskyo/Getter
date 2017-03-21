#IFNDEF "TIMER_HPP"
#DEFINE "TIMER_HPP"

#include <iostream>
#include <cstdint>
#include <queue>
#include <vector>
#include <map>
#include "../Tools/Lock.hpp"


struct TimerInfo
{
	uint32_t id;           //  分配的唯一id
	uint32_t expire_time;  //  超时时间戳
	uint32_t src;          //  对应是哪个Actor注册的
	
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
	map<uint32_t, struct TimerInfo> timer_map;
	uint32_t id_count;
	CasLock lock;
	std::priority_queue<TimerInfo, std::vector<TimerInfo>, TimerComp> timer_queue;

	TimerManager()
	{
		id_count = 1;
	}
};



int timer_create(uint32_t expire_time, uint32_t src);
int timer_delete(uint32_t timer_id);


#ENDIF