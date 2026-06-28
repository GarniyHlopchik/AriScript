#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../include/tokenizer.h"
#include <variant>
#include <iostream>

using ::testing::DoubleEq;

TEST(TokenizerTests, DisplayToken) {
    Token token;
    token.type = TokenType::LEFT_BRACE; //index in enum: 2
    token.lexeme = "{";
    token.literal = std::monostate{};
    EXPECT_EQ(token.to_string(), "{ nil");
    
}
TEST(TokenizerTests, ArbitraryCode){
    const std::string code = 
    R"(import modulename
var varname: typename # comment
func funcname(arg: type):
    return 10.24
)";
    Tokenizer tokenizer(code, true);
    Token expected[22] = {
        {.type = TokenType::IMPORT, .lexeme = "import", .literal = "import"},
        {.type = TokenType::IDENTIFIER, .lexeme = "modulename", .literal = "modulename"},
        {.type = TokenType::LINEBREAK, .lexeme = "\n", .literal = "\n"},
        {.type = TokenType::VAR, .lexeme = "var", .literal = "var"},
        {.type = TokenType::IDENTIFIER, .lexeme = "varname", .literal = "varname"},
        {.type = TokenType::COLON, .lexeme = ":", .literal = ":"},
        {.type = TokenType::IDENTIFIER, .lexeme = "typename", .literal = "typename"},
        {.type = TokenType::LINEBREAK, .lexeme = "\n", .literal = "\n"},
        {.type = TokenType::FUNC, .lexeme = "func", .literal = "func"},
        {.type = TokenType::IDENTIFIER, .lexeme = "funcname", .literal = "funcname"},
        {.type = TokenType::LEFT_PAREN, .lexeme = "(", .literal = "("},
        {.type = TokenType::IDENTIFIER, .lexeme = "arg", .literal = "arg"},
        {.type = TokenType::COLON, .lexeme = ":", .literal = ":"},
        {.type = TokenType::IDENTIFIER, .lexeme = "type", .literal = "type"},
        {.type = TokenType::RIGHT_PAREN, .lexeme = ")", .literal = ")"},
        {.type = TokenType::COLON, .lexeme = ":", .literal = ":"},
        {.type = TokenType::LINEBREAK, .lexeme = "\n", .literal = "\n"},
        {.type = TokenType::INDENT, .lexeme = "INDENT", .literal = "INDENT"},
        {.type = TokenType::RETURN, .lexeme = "return", .literal = "return"},
        {.type = TokenType::FLOAT, .lexeme = "10.24", .literal = 10.24},
        {.type = TokenType::LINEBREAK, .lexeme = "\n", .literal = "\n"},
        {.type = TokenType::DEDENT, .lexeme = "DEDENT", .literal = "DEDENT"},
    };
    for(int i = 0; i < 22; i++){
        std::cout << "[ITERATION] " << i << std::endl;
        Token got = tokenizer.advance();
        std::cout << got.to_string() << std::endl;
        if(i == 0){
            EXPECT_NE(static_cast<int>(got.type),50) << "Emits finish immediately";
        }
        EXPECT_EQ(got.type, expected[i].type) << "on token " << i << " "<< got.to_string() << " != " << expected[i].to_string();
        EXPECT_EQ(got.lexeme, expected[i].lexeme) << "on token " << i << " "<< got.to_string() << " != " << expected[i].to_string();
        if(std::holds_alternative<double>(got.literal)){
            if(!std::holds_alternative<double>(expected[i].literal)){
                FAIL() << "literal holds double while expected isn't double";
            }
            EXPECT_THAT(std::get<double>(got.literal), DoubleEq(std::get<double>(expected[i].literal)));

        }
        else{
            EXPECT_EQ(got.literal, expected[i].literal) << "on token " << i << " "<< got.to_string() << " != " << expected[i].to_string();
        }
    }
}

TEST(TokenizerTests, literals){
    Tokenizer tokenizer("true false 0 12.3 67.89 ", true);
    std::cout << "if last failed [1]" << std::endl;
    Token t = tokenizer.advance();
    std::cout << t.literal.index() << std::endl;
    std::cout << static_cast<int>(t.type) << std::endl;
    EXPECT_EQ(std::get<bool>(t.literal), true) << "1";
    std::cout << "if last failed [2]" << std::endl;
    EXPECT_EQ(std::get<bool>(tokenizer.advance().literal), false) << "2";
    EXPECT_EQ(std::get<int>(tokenizer.advance().literal), 0) << "3";
    EXPECT_EQ(std::get<double>(tokenizer.advance().literal), 12.3) << "4";
    EXPECT_EQ(std::get<double>(tokenizer.advance().literal), 67.89) << "5";
}
TEST(TokenizerTests, IdentifierNaming){
    Tokenizer tokenizer("log2 _callback 67lovers67", true);
    Token log_token = tokenizer.advance(); 
    EXPECT_EQ(log_token.type, TokenType::IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(log_token.literal), log_token.lexeme);
    EXPECT_EQ(log_token.lexeme, "log2");

    Token callback_token = tokenizer.advance();
    EXPECT_EQ(callback_token.type, TokenType::IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(callback_token.literal), callback_token.lexeme);
    EXPECT_EQ(callback_token.lexeme, "_callback");

    Token num = tokenizer.advance();
    EXPECT_EQ(num.type, TokenType::INT);
    EXPECT_EQ(std::get<int>(num.literal),67);

    Token last = tokenizer.advance();
    EXPECT_EQ(last.type, TokenType::IDENTIFIER);
    EXPECT_EQ(std::get<std::string>(last.literal),last.lexeme);
    EXPECT_EQ(last.lexeme, "lovers67");
}