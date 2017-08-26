#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <string.h>

class util {
    static int util::getSecondsLevelTs();  // 获取秒级时间戳
    static unsigned long long util::getMsSecondsLevelTs();  // 获取毫秒级时间戳
    static bool util::isSameDay(int int);    // 判断两个秒级时间戳是否为同一天
    static bool util::isSameHour(int, int); // 判断两个秒级时间戳是否为同一个小时
};


#endif
