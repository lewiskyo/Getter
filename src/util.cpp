#include "util.h"

int util::getSecondsLevelTs(){
    return time((time_t*)NULL);
}

unsigned long long util::getMsSecondsLevelTs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

/*
bool util::isSameDay(int left, int right) {
    char left_str[100];
    strftime(left_str, sizeof(left_str), "%04Y%02m%02d", left);

    char right_str[100];
    strftime(right_str, sizeof(right_str), "%04Y%02m%02d", right);

    if (strcmp(left_str, right_str)) {
        return false;
    }

    return true;
} */

bool util::isSameHour(int left, int right) {
    int diff = left - right;
    diff = abs(diff);
    if ( diff < 3600)
        return true;

    return false;
}