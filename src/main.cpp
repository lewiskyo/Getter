#include "global.h"

extern Server getter;
extern TimerMng timermng;

void init() {
    timermng.init();
    getter.init();
}

void run() {
    timermng.run();
    getter.run();
}

int main(int argc, char *argv[]) {
    init();
    run();
}
