#ifndef ARI_DEBUG_IMPLEMENTATION
#define ARI_DEBUG_IMPLEMENTATION

#include <vector>
#include <stdint.h>
#include <string>

void disassembleChunk(std::vector<uint8_t> chunk, const std::string& name);
int disassembleInstruction(std::vector<uint8_t> chunk, int offset);

#endif