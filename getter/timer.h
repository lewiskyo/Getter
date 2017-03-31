#pragma  once

#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <time.h>
#include <thread>
#include <unistd.h>
#include <cstdint>
#include <sys/time.h>
#include <list>
#include <memory>
#include "lock.h"
#include "util.h"


namespace getter {
	
	struct TimerInfo {
		uint32_t id;           //  分配的唯一id
		long expire_time;  //  超时时间(增量 ms)
		uint32_t src;          //  对应是哪个Actor注册的
		uint32_t interval;     //  循环间隔时间(ms)
		bool is_working;       //  标记是否还在工作
	};

	struct TimerComp {
		bool operator()(const std::shared_ptr<TimerInfo>& lhs, const std::shared_ptr<TimerInfo>& rhs)
		{
			return lhs.get()->expire_time < rhs.get()->expire_time;
		}
	};

	struct TimerMng: private noncopyable {

		TimerMng();

		int create_timer(long, uint32_t, uint32_t );
		void deleteTimer(uint32_t);
		
		void updateTimer();
		void executeTimer();

		void initial();
		void run();
		void stop();

		void workThread();

		static TimerMng& getTimerMng();

	private:
		uint32_t id_count;
		CasLock add_timer_lock;
		CasLock delete_timer_lock;

		std::map<uint32_t, std::shared_ptr<TimerInfo> > timer_map;
		std::priority_queue<std::shared_ptr<TimerInfo>, std::vector<std::shared_ptr<TimerInfo> >, TimerComp> timer_queue;

		// 新添加的定时器列表,在update_timer时再加入到优先队列
		std::list<std::shared_ptr<TimerInfo> > wait_add_timer_list;

		// 待删除的定时器列表, 再update_timer时再从优先队列和map中删除
		std::list<uint32_t> delete_timer_id_list;

		std::thread th;

		long now;          //  当前时间戳

		long getCurrentTime();
	};
}
