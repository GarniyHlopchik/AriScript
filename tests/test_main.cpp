#include <gtest/gtest.h>
#include "../include/tokenizer.h"
#include <variant>

TEST(TokenizerTests, DisplayToken) {
    Token token;
    token.type = TokenType::LEFT_BRACE; //index in enum: 2
    token.lexeme = "{";
    token.literal = std::monostate{};
    EXPECT_EQ(token.to_string(), "2 { nil");
    
}
