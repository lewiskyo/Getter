#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <sys/time.h>
#include <string.h>

class util {
    public:
        static int getSecondsLevelTs();  // ��ȡ�뼶ʱ���
        static unsigned long long getMsSecondsLevelTs();  // ��ȡ���뼶ʱ���
        static bool isSameDay(int, int);    // �ж������뼶ʱ����Ƿ�Ϊͬһ��
        static bool isSameHour(int, int); // �ж������뼶ʱ����Ƿ�Ϊͬһ��Сʱ
};


#endif
