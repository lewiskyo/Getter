#include "global.h"

Server server;
TimerMng timerMng;
Logger logger;
ModuleHelper modHelper;

void init() {
    logger.init("./");
    modHelper.init("./service");
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
