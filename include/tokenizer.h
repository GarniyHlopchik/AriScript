#ifndef ARI_TOKENIZER_IMPLEMENTATION
#define ARI_TOKENIZER_IMPLEMENTATION

#include <string>
#include <optional>
#include <cctype>
#include <variant>
#include <string>
#include <vector>
#include <optional>
#include <stack>
#include <queue>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include "def_literal.h"
enum class TokenType{
    // Single-character tokens and formatting.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COLON,
    COMMA, DOT, MINUS, PLUS, SHARP, MUL, DIV, INDENT, DEDENT, LINEBREAK,
    INC, DEC,

    // One or two character tokens. 17
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL, ARROW,
    MINUS_EQUAL, PLUS_EQUAL, MUL_EQUAL, DIV_EQUAL,

    // Literals. 28
    IDENTIFIER, STRING, INT, FLOAT, BOOL,

    // Keywords. 33
    AND, CLASS, ELSE, FUNC, FOR, IF, NIL, OR, NOT, IN,
    RETURN, SUPER, THIS, VAR, WHILE, IMPORT, EXPORT,
    //50
    FINISH
};

struct Token{
    TokenType type;
    std::string lexeme; //just factual symbols
    Literal literal; //null, double, string, bool, int parsed
    uint32_t line;    //if you make 4B line files I'll personally end you
    uint16_t column; //same with 65k character lines
    uint16_t length;  //65k for len cause long text strings might be > 255
    std::string to_string(){
        return lexeme + " " + literal_to_string();
    }
    std::string literal_to_string(){
        if (std::holds_alternative<double>(literal)) {
            return std::to_string(std::get<double>(literal));
        }
        else if(std::holds_alternative<int>(literal)){
            return std::to_string(std::get<int>(literal));
        }
        else if(std::holds_alternative<bool>(literal)){
            return std::get<bool>(literal) ? "true" : "false";
        }
        else if(std::holds_alternative<std::monostate>(literal)){
            return "nil";
        }
         else if (std::holds_alternative<std::string>(literal)) {
            return std::get<std::string>(literal);
        }
        return "";
    }
};
class Tokenizer{
    public:
        Tokenizer(std::string path, bool is_from_string);
        
        Token advance();
        ~Tokenizer();
    private:

        std::optional<Token> check_indents();
        std::optional<Token> check_singlechars();
        std::optional<Token> check_literals();
        std::string parse_keyword();
        Token make_token(std::string lexeme, TokenType type, uint8_t length);
        bool advance_character();
        std::optional<char> peek_character(int amount);

        uint32_t cursor_line = 0;
        uint16_t cursor_column = 0;

        bool is_from_string = false;
        std::ifstream file;
        std::string line = "";
        std::stringstream code_stringstream; //only used when code is from string
        std::stack<uint16_t> indent_stack;
        std::queue<Token> buffered_tokens;
        bool shutoff = false;
};

#endif