#include "module.h"
using namespace std;

ModuleHelper mod_helper("sss");

ModuleHelper::ModuleHelper(string _path):path(_path) {
    modules.reserve(32);
}

void * ModuleHelper::try_open(string name) {

    string full_path = this->path + name;

    void *dl = dlopen(full_path.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (dl == NULL) {
        fprintf(stderr, "try open %s failded : %s\n", name.c_str(), dlerror());
    }

    return dl;
}

Module * ModuleHelper::query(string name) {
    int i;
    for (i=0; i<this->modules.size(); ++i) {
        if (this->modules[i].name == name)
            return &this->modules[i];
    }

    return NULL;
}

void * ModuleHelper::get_api(Module *mod, string api_name) {
    string final_api_name = mod->name + api_name;
    return dlsym(mod->module, final_api_name.c_str());
}

bool ModuleHelper::open_sym(Module *mod) {
    mod->create = (void*(*)())get_api(mod, "_create");
    mod->init = (bool(*)(void*,Agent*,string))get_api(mod, "_init");
    mod->dispatch =(void(*)(Agent*, string*)) get_api(mod, "_dispatch");
    mod->destroy = (void(*)())get_api(mod, "_destroy");

    return mod->init == NULL;
}

Module * ModuleHelper::getter_module_query(string name) {
    Module * result = mod_helper.query(name);
    if (result)
        return result;

    MyLockGuard lockguard(&mod_helper.lock);

    result = mod_helper.query(name);

    if (result == NULL) {
        void * dl = mod_helper.try_open(name);
        if (dl) {
            Module new_mod;
            new_mod.name = name;
            new_mod.module = dl;

            if (!mod_helper.open_sym(&new_mod)) {
                mod_helper.modules.push_back(new_mod);
                result = &mod_helper.modules.back();
            }
        }
    }

    return result;
}

void * ModuleHelper::getter_module_instance_create(Module *mod) {
    if (mod->create) {
        return mod->create();
    } else {
        return NULL;
    }
}


void ModuleHelper::getter_module_instance_dispatch(Module* mod, Agent *agent, string* msg) {
    mod->dispatch(agent, msg);
}

bool ModuleHelper::getter_module_instance_init(Module * mod, void * inst, Agent * agent, string param) {
    return mod->init(inst, agent, param);
}
