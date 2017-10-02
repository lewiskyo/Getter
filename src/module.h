#ifndef MODULE_H
#define MODULE_H

#include <string>
#include <vector>
#include <dlfcn.h>
#include <stdio.h>
#include <assert.h>

#include "lock.h"

class Agent;

typedef void * (dlCreate)(void);
typedef bool (*dlInit)(void* inst, Agent *agent, std::string param );
typedef void (*dlDispatch)(Agent* agent, std::string* msg); 
typedef void (*dlDestroy)();

typedef struct Module {
    std::string name;
    /* Point to the service class */
    void *module;

    /* Four common function */
    dlCreate create;
    dlInit init;
    dlDispatch dispatch;
    dlDestroy destroy;

} Module;

class ModuleHelper {
    private:
        CasLock lock;
        /* Save loaded modules */
        std::vector<Module> modules;
        /* Service path */
        std::string path;

    private:
        void * tryOpen(std::string name);
        Module* query(std::string name);
        void * getApi(Module* mod, std::string api_name);
        bool openSym(Module *mod);

    public:
        ModuleHelper(std::string _path);
        static Module *modQuery(std::string name);
        static void *modInstanceCreate(Module *mod);
        static bool modInstanceInit(Module* mod, void *inst, Agent *agent, std::string param);
        static void modInstanceDispatch(Module *mod, Agent* agent, std::string * msg);

};

#endif
