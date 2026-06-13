#ifndef ARI_VALUE_IMPLEMENTATION
#define ARI_VALUE_IMPLEMENTATION

#include <stdint.h>
#include <variant>
#include <iostream>
struct Reference{
    void* data;
    std::string type;
};
typedef std::variant<int64_t, double, Reference*> Value;

void printValue(Value& val);

#endif