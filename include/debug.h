#ifndef ARI_DEBUG_IMPLEMENTATION
#define ARI_DEBUG_IMPLEMENTATION

#include <vector>
#include <stdint.h>
#include <string>
#include "chunk.h"

void disassembleChunk(Chunk& chunk, const std::string& name);
int disassembleInstruction(Chunk& chunk, int offset);

#endif