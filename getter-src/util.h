#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <string.h>

class util {
    static int util::getSecondsLevelTs();  // ��ȡ�뼶ʱ���
    static unsigned long long util::getMsSecondsLevelTs();  // ��ȡ���뼶ʱ���
    static bool util::isSameDay(int int);    // �ж������뼶ʱ����Ƿ�Ϊͬһ��
    static bool util::isSameHour(int, int); // �ж������뼶ʱ����Ƿ�Ϊͬһ��Сʱ
};


#endif
