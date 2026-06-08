#ifndef ARI_PARSER_IMPLEMENTATION
#define ARI_PARSER_IMPLEMENTATION

#include "tokenizer.h"

class Expression{};

class Parser{
    private:
        Token consume();
        bool match(TokenType token);
};

#endif