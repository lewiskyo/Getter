#include "agent.h"
using namespace std;

Agent::Agent(string name, string param) {

    this->session_id = 0;
    this->is_schedule = false;
    this->is_init = false;
    msg_count = 0;
    this->param = param;
    this->name = name;

}

bool Agent::init() {
    
    Module * mod = ModuleHelper::getter_module_query(name);

    if (mod == NULL)
        return false;

    this->mod = mod;

    void *inst = ModuleHelper::getter_module_instance_create(mod);
    if (inst == NULL)
        return false;

    this->instance = inst;

    // todo join globalqueue

    return true;
}
