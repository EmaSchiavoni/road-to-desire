#pragma once
#include <string>

namespace rtd {

    template<typename T>
    struct Resource
    {
        bool is_loaded = false;
        unsigned int elements_using_it = 0;
        std::string path = "";
        T resource;
    };

}