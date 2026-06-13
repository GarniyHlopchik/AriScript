#ifndef ARI_CHUNK_IMPLEMENTATION
#define ARI_CHUNK_IMPLEMENTATION

#include <stdint.h>
#include <vector>
#include "value.h"

enum class OpCode: uint8_t {
    OP_RETURN,
    OP_CONSTANT
};
struct Chunk {
    std::vector<uint8_t> code;
    std::vector<Value> constants;
};

#endif