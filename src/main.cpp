#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "tokenizer.h"
#include "chunk.h"
#include "debug.h"
#include <optional>

std::string load_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return ""; // Or throw an error
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]){
    /*  tokenizer usage example */
    if(argc>1){
        Tokenizer tokenizer(load_file(argv[1]), true);
        Token token;
        while(token.type != TokenType::FINISH){
            token = tokenizer.advance();
            std::cout << "[Token]" << token.to_string() << std::endl;
        }
    }
    

    /*
    Chunk chunk;
    chunk.code.push_back(static_cast<uint8_t>(OpCode::OP_RETURN));
    chunk.code.push_back(static_cast<uint8_t>(OpCode::OP_CONSTANT));
    chunk.code.push_back(0);
    chunk.constants.push_back(67);
    disassembleChunk(chunk, "return_test");
    return 0;
    */
    
}