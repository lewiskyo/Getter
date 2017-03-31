#include "util.h"

using namespace std;

namespace getter {

	long util::getCurrentTime() {
		struct timeval tv;
		gettimeofday(&tv,NULL);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
	}

	void* util::loadSo(string name) {

	    void *handle = NULL;

	    string so_path("./actorsolib/actor" + name + ".so");

	    //打开动态链接库
	    handle = dlopen(so_path.c_str(), RTLD_LAZY);
	    if (!handle) {
	    	fprintf(stderr, "%s\n", dlerror());
	    	exit(EXIT_FAILURE);
	    }

	    //清除之前存在的错误
	    dlerror();

	    return handle;
	}
}