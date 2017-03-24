#include "Timer.h"
#include "Scheduler.h"


void initial()
{
	Timer::initial();

	Scheduler::initial();
}

void run()
{
	Timer::run();

	Scheduler::run();
}

void stop()
{
	Timer::stop();

	Scheduler::stop();
}

int main()
{
	initial();

	run();

	stop();
}