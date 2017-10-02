#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

class util {
    public:
        static int getSecondsLevelTs();  
        static unsigned long long getMsSecondsLevelTs();  
        // static bool isSameDay(int, int);    
        static bool isSameHour(int, int);
};


#endif
