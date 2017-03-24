#include "Api.h"
#include "Scheduler.h"
#include "Timer.h"

using namespace std;

void Api::send(Message* msg, int des)
{
	Scheduler::send(msg, des);
}

int Api::timer_create(long after_time, uint32_t src, uint32_t interval = 0)
{
	return Timer::timer_create(after_time, src, interval);
}

void Api::timer_delete(uint32_t timer_id)
{	
	Timer::timer_delete(timer_id);
}

int Api::new_actor(string name)
{
	return Scheduler::new_actor(name);
}