#include "global.h"

extern Server server;
extern TimerMng timerMng;

void init() {
    timerMng.init();
    server.init();
}

void run() {
    timerMng.run();
    server.run();
}

int main(int argc, char *argv[]) {
    init();
    run();
}
