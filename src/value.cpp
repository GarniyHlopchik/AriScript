#include "value.h"

void printValue(Value& val){
    if(std::holds_alternative<int64_t>(val)){
        std::cout << std::get<int64_t>(val);
    }
    if(std::holds_alternative<double>(val)){
        std::cout << std::get<double>(val);
    }
    if(std::holds_alternative<Reference*>(val)){
        std::cout << std::get<Reference*>(val);
    }

}