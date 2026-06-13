#include "debug.h"
#include "chunk.h"
#include <iostream>
static int simpleInstruction(const std::string& name, int offset) {
    std::cout << name << std::endl;
    return offset + 1;
}
static int constantInstruction(const std::string& name, Chunk& chunk, int offset){
    int const_index = chunk.code[offset+1];
    std::cout << name << " ";
    printValue(chunk.constants[const_index]);
    return offset + 2;
}
void disassembleChunk(Chunk& chunk, const std::string& name) {
    std::cout << "== "<< name <<" ==\n";

    for (int offset = 0; offset < chunk.code.size();) {
        offset = disassembleInstruction(chunk, offset);
    }
}
int disassembleInstruction(Chunk& chunk, int offset) {
    std::cout << offset << " ";

    OpCode instruction = static_cast<OpCode>(chunk.code[offset]);
    switch (instruction) {
        case OpCode::OP_RETURN:
            return simpleInstruction("OP_RETURN", offset);
        case OpCode::OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            std::cout << "Unknown OpCode: "<<static_cast<int>(instruction) << std::endl;
            return offset + 1;
    }
}
