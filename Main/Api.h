// 提供给Actor调用的接口函数

#ifndef API_H
#define API_H

#include <iostream>
#include "../Tools/Message.h"

namespace Api
{
	void send(Message*, int);

	int timer_create(long, uint32_t, uint32_t);

	void timer_delete(uint32_t);

	int new_actor(std::string);

}


#endif