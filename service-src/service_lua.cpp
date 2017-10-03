extern "C" {
    #include <lua.h>  
    #include <lualib.h>  
    #include <lauxlib.h>  
}

#include "../src/agent.h"
#include <string>

typedef struct luaService {
    lua_State *L;
} luaService;

/* catch error info function */
static int traceback(lua_State *L){
    const char *msg = lua_tostring(L, 1);
    if (msg)
        luaL_traceback(L,  L, msg, 1);
    else {
        lua_pushliteral(L, "(no error message)");
    }

    return 1;
}


static int launchCb(Agent *agent, int type, int session, int source, const void *msg, size_t sz){
    assert( type == 0 && session == 0);

}


extern "C" {

void *luaService_create(void){
    luaService * l = new  luaService;
    l->L = luaL_newstate();
    return l;
}

bool luaService_init(void* inst, Agent *agent, std::string param ){
    luaService *l = (luaService*) inst;
    
}

void luaService_dispatch(Agent* agent, std::string* msg){

}

void luaService_destroy(){

}

}