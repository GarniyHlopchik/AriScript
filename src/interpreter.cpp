#include "interpreter.h"

void AriInterpreter::evaluate_bytecode(std::vector<uint8_t>& bytecode){
    uint32_t ip = 0;
    size_t code_size = bytecode.size();
    
}