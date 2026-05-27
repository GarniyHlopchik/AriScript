#include <iostream>
#include <string>
#include "tokenizer.h"
#include <optional>
int main(int argc, char* argv[]){
    
    if(argc>1){
        Tokenizer tokenizer(argv[1]);
        Token token;
        while(token.type != TokenType::FINISH){
            token = tokenizer.advance();
            std::cout << "[Token]" << token.to_string() << std::endl;
        }
    }
    
}