#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>
#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>

#include "lock.h"

class Agent;

typedef void * (*getter_dl_create)(void);
typedef bool (*getter_dl_init)(void* inst, Agent *agent, std::string param );
typedef void (*getter_dl_dispatch)(Agent* agent, std::string* msg); 
typedef void (*getter_dl_destroy)();

typedef struct Module {
    std::string name;
    void *module;
    getter_dl_create create;
    getter_dl_init init;
    getter_dl_dispatch dispatch;
    getter_dl_destroy destroy;

} Module;

class ModuleHelper {
    private:
        CasLock lock;
        std::vector<Module> modules;
        std::string path;

    private:
        void * try_open(std::string name);
        Module* query(std::string name);
        void * get_api(Module* mod, std::string api_name);
        bool open_sym(Module *mod);

    public:
        ModuleHelper(std::string _path);
        static Module * getter_module_query(std::string name);
        static void * getter_module_instance_create(Module *mod);
        static bool getter_module_instance_init(Module* mod, void *inst, Agent *agent, std::string param);
        static void getter_module_instance_dispatch(Module *mod, Agent* agent, std::string * msg);

};

//ModuleHelper mod_helper("sss");

#endif
