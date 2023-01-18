/*
If you plan to use cave::String in a Hash Table such as cave's HashMap or
even std::unordered_map, you'll need to make sure it works with the std::hash
function. So just include this file into your code and it will do the trick.
*/

#ifndef CAVE_STD_STRING_HASH_H
#define CAVE_STD_STRING_HASH_H

#include <functional>
#include <string>

#include "STD/String.h"


namespace std {
    template<>
    struct hash<cave::String> {
        size_t operator()(const cave::String& s) const {
            return hash<std::string>()(s.c_str());
        }
    };
}

#endif // ! CAVE_STD_STRING_HASH_H