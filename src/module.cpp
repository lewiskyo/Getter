#include "module.h"
using namespace std;

ModuleHelper modHelper("./service");

ModuleHelper::ModuleHelper(string _path):path(_path) {
	modules.reserve(32);
}

void *ModuleHelper::tryOpen(string name) {

	string full_path = this->path + name;

	void *dl = dlopen(full_path.c_str(), RTLD_NOW | RTLD_GLOBAL);
	if (dl == NULL) {
		fprintf(stderr, "try open %s failded : %s\n", name.c_str(), dlerror());
	}

	return dl;
}

Module *ModuleHelper::query(string name) {
	int i;
	for ( i = 0; i < this->modules.size(); ++i ) {
		if ( this->modules[i].name == name )
			return &this->modules[i];
	}

	return NULL;
}

void *ModuleHelper::getApi(Module *mod, string api_name) {
	string final_api_name = mod->name + api_name;
	return dlsym(mod->module, final_api_name.c_str());
}

bool ModuleHelper::openSym(Module *mod) {
	mod->create = (void*(*)())get_api(mod, "_create");
	mod->init = (bool(*)(void*,Agent*,string))get_api(mod, "_init");
	mod->dispatch =(void(*)(Agent*, string*)) get_api(mod, "_dispatch");
	mod->destroy = (void(*)())get_api(mod, "_destroy");

	return mod->init == NULL;
}

Module * ModuleHelper::moduleQuery(string name) {
	Module *result = modHelper.query(name);
	if (result)
		return result;

	LockGuard guard(&modHelper.lock);

	result = modHelper.query(name);

	if (result == NULL) {
		void * dl = modHelper.tryOpen(name);
		if (dl) {
			Module newMod;
			newMod.name = name;
			newMod.module = dl;

			if (!modHelper.openSym(&newMod)) {
				modHelper.modules.push_back(newMod);
				result = &modHelper.modules.back();
			}
		}
	}

	return result;
}

void * ModuleHelper::modInstanceCreate(Module *mod) {
	if (mod->create) {
		return mod->create();
	} else {
		return NULL;
	}
}

bool ModuleHelper::modInstanceInit(Module * mod, void * inst, Agent * agent, string param) {
	return mod->init(inst, agent, param);
}


void ModuleHelper::modInstanceDispatch(Module* mod, Agent *agent, string* msg) {
	mod->dispatch(agent, msg);
}


