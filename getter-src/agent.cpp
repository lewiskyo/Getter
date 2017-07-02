#include "agent.h"

Agent::Agent(string name, string param) {
    Module * mod = ModuleHelper::getter_module_query(name);

    if (mod == NULL)
        reutrn NULL;

    void *inst = ModuleHelper::getter_module_instance_create(mod);
    if (inst == NULL)
        return NULL;

    this->mod = mod;
    this->instance = inst;
    this->session_id = 0;

    this->schedule = false;
    this->init = false;
    msg_count = 0;

    // todo º”»ÎGlobalQueue
}
