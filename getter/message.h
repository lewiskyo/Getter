#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <memory>
#include <unistd.h>

typedef struct Message
{
	int type;  // 消息类型
	uint32_t length;   //  消息长度
	std::shared_ptr<char> data;  // 消息实体

} Message;

#endif