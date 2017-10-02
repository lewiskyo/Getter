#include "agent.h"
using namespace std;

Agent::Agent(string name, string param) {

    this->sessionId = 0;
    this->isSchedule = false;
    this->isInit = false;
    msgCount = 0;
    this->param = param;
    this->name = name;

}

bool Agent::init() {
    
    Module * mod = ModuleHelper::moduleQuery(this->name);

    if (mod == NULL)
        return false;

    this->mod = mod;

    void *inst = ModuleHelper::getter_module_instance_create(mod);
    if (inst == NULL)
        return false;

    this->instance = inst;

    // Todo add to globalqueue and wait schedule

    return true;
}
