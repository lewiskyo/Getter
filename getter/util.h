#pragma once
#include <functional>
#include <utility>
#include <stdlib.h>
#include <sys/time.h>
#include <dlfcn.h>
#include <string>

namespace getter {

struct noncopyable {
    noncopyable() {};
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};


struct util {
	static long getCurrentTime();
	static void* loadSo(std::string);
};

}