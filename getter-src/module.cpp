#include "module.h"
using namespace std;

ModuleHelper::ModuleHelper(string _path):path(_path) {
    id = 1;
    m.reserve(32);
}

void * ModuleHelper::try_open(string name) {

    string full_path = this->path + name;

    void *dl = dlopen(full_path, RTLD_NOW | RTLD_GLOBAL);
    if (dl == NULL) {
        fprintf(stderr, "try open %s failded : %s\n", name, dlerror());
    }

    return dl;
}

Module * ModuleHelper::query(string name) {
    int i;
    for (i=0; i<this->m.size(); ++i) {
        if (this->m[i].name == name)
            return this->m[i];
    }

    return NULL;
}

void * ModuleHelper::get_api(Module *mod, string api_name) {
    string final_api_name = mod->name + api_name;
    return dlsym(mod->module, final_api_name.c_str())
}

bool ModuleHelper::open_sym(Module *mod) {
    mod->create = get_api(mod, "_create");
    mod->init = get_api(mod, "_init");
    mod->dispatch= get_api(mod, "_dispatch");
    mod->destroy= get_api(mod, "_destroy");

    return mod->init == NULL;
}

Module * ModuleHelper::getter_module_query(string name) {
    Module * result = mod_helper.query(name);
    if (result)
        return result;

    MyLockGuard lockguard(mod_helper.lock);

    result = mod_helper.query(name);

    if (result == NULL) {
        void * dl = mod_helper.try_open(name);
        if (dl) {
            Module new_mod;
            mod.name = name;
            mod.module = dl;

            if (!mod_helper.open_sym(new_mod)) {
                m.push_back(new_mod);
                result = &m.back();
            }
        }
    }

    return result;
}

void * ModuleHelper::getter_module_create(Module *mod) {
    if (m->create) {
        return m->create();
    } else {
        return NULL;
    }
}


void ModuleHelper::getter_module_dispatch(Module* mod, void *agent, string* msg) {
    m->dispatch(agent, msg);
}

void ModuleHelper::getter_module_init(Module * mod, void * inst, Agent * agent, string param) {
    m->init(inst, agent, param)
}
