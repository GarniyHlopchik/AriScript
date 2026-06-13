#include <iostream>
#include <string>
#include "tokenizer.h"
#include "chunk.h"
#include "debug.h"
#include <optional>
int main(int argc, char* argv[]){
    /*  tokenizer usage example
    if(argc>1){
        Tokenizer tokenizer(argv[1]);
        Token token;
        while(token.type != TokenType::FINISH){
            token = tokenizer.advance();
            std::cout << "[Token]" << token.to_string() << std::endl;
        }
    }
    */
    Chunk chunk;
    chunk.code.push_back(static_cast<uint8_t>(OpCode::OP_RETURN));
    chunk.code.push_back(static_cast<uint8_t>(OpCode::OP_CONSTANT));
    chunk.code.push_back(0);
    chunk.constants.push_back(67);
    disassembleChunk(chunk, "return_test");
    return 0;

    
}