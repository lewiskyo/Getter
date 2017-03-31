#pragma once
#include <string>
#include <functional>
#include <utility>
#include <vector>
#include <string.h>
#include <stdlib.h>

namespace getter {

struct noncopyable {
    noncopyable() {};
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

}