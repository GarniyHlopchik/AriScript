#include "debug.h"
#include "chunk.h"
#include <iostream>
static int simpleInstruction(const std::string& name, int offset) {
  std::cout << name << std::endl;
  return offset + 1;
}
void disassembleChunk(std::vector<uint8_t> chunk, const std::string& name) {
  std::cout << "== "<< name <<" ==\n";

  for (int offset = 0; offset < chunk.size();) {
    offset = disassembleInstruction(chunk, offset);
  }
}
int disassembleInstruction(std::vector<uint8_t> chunk, int offset) {
  std::cout << offset;

  OpCode instruction = static_cast<OpCode>(chunk[offset]);
  switch (instruction) {
    case OpCode::OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
    default:
      std::cout << "Unknown OpCode: "<<static_cast<int>(instruction) << std::endl;
      return offset + 1;
  }
}
