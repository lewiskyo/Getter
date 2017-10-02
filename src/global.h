#ifndef GLOBAL_H
#define GLOBAL_H

#include "timer.h"
#include "server.h"
#include "log.h"
#include "module.h"

extern Server server;
extern TimerMng timerMng;
extern Logger logger;
extern ModuleHelper modHelper;

#endif
