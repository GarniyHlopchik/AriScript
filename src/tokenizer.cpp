#include "tokenizer.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include "error_handler.h"
/*
Implementation of the tokenizer class here in ari.
The core idea that it tries not to consume unnecessary ram by tokenizing the entire file.
Instead it takes the file as a stream and works through it line by line via std::newline
This fact is also abstracted by a bunch of helpers. Most importantly, advance_character()
which returns true when getting next character was successful and false when it failed.
Under the hood it handles getting the next line, returning false if there isn't one (EOF),
as well as handling cursor_column and cursor_line. Another byproduct of the stream approach
is that we don't generate a full std::vector<Token> or anything like that, but instead just
spit out the next token when something (probably the parser) calls advance(). advance() will
always return a valid token until token with TokenType::FINISH is emitted. Everything beyond
that will probably keep returning FINISH, but this is not "reglamented" at all, let's call
"undefined behavior" :) Meaning, when parser gets the first FINISH, it needs to stop using
this tokenizer and just destruct it - it did its thing
*/

Tokenizer::Tokenizer(std::string path, bool is_from_string = false) : is_from_string(is_from_string){
    

    indent_stack.push(0); //this is so that there is always something in the stack to
    //compare to. So if some line gets indent of say 4, it can check 4>0 and push indent here
    
    //stupid code for load from string cause I haven't thought of it initially
    //if is_from_string is true, path is not really path, but a source code string
    //in that case 'file' (a filestream) is never initialized
    if(is_from_string){
        code_stringstream.str(path);
        
    }else{

        file.open(path, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Could not open the file in path: " << path << std::endl;
            return;
        }
    }
    //getting first line
    if(is_from_string){
        std::getline(code_stringstream,line);
    }
    else{
        std::getline(file,line);
    }
    if(!line.empty() && line.back() == '\r') //getting rid of \r's for simpler tokenization
    line.pop_back();
    line+='\n'; //but we're adding \n's "back" since getline strips them away
    //and we need \n's to parse linebreak tokens (act as ;)

    
}
Tokenizer::~Tokenizer(){
    if(!is_from_string){
        file.close();
    }
}

bool Tokenizer::advance_character() {
    cursor_column += 1;
    if(cursor_column >= line.length()) {
        if(is_from_string){
            if(!std::getline(code_stringstream, line)) {
                return false;
            }
        }else{
            if(!std::getline(file, line)) {
                return false;
            }
        }
        //\r and \n stuff is same as in constructor
        if(!line.empty() && line.back() == '\r')
        line.pop_back();

        line += '\n';

        cursor_line += 1;
        cursor_column = 0;
    }
    
    return true;
}
std::optional<char> Tokenizer::peek_character(int amount){
    //if line ends peeking fails. tokens generally shouldn't span multiple lines so it's fine
    //not doing so adds complexity and having to make line buffers so I just impose this here
    if(line.length() <= cursor_column + amount){ 
        return std::nullopt;
    }

    return line[cursor_column+amount];
}
std::optional<Token> Tokenizer::check_singlechars(){
    //only consumes stuff when it actually finds a token
    //by the way it parses 1 and 2 char tokens so name is misleading but oh well
    switch(line[cursor_column]){
        case '{':
            return make_token("{",TokenType::LEFT_BRACE, 1);
        case '}':
            return make_token("}",TokenType::RIGHT_BRACE, 1);
        case '(':
            return make_token("(",TokenType::LEFT_PAREN, 1);
        case ')':
            return make_token(")",TokenType::RIGHT_PAREN, 1);
        case ',':
            return make_token(",",TokenType::COMMA, 1);
        case '.':
            return make_token(".",TokenType::DOT, 1);
        case '\n':
            return make_token("\n",TokenType::LINEBREAK, 1);
        case ':':
            return make_token(":",TokenType::COLON, 1);
        case '-':
            if(peek_character(1) == '='){
                Token t =  make_token("-=", TokenType::MINUS_EQUAL, 2);
                advance_character();
                return t;
            }else if(peek_character(1) == '>'){
                Token t =  make_token("->", TokenType::ARROW, 2);
                advance_character();
                return t;
            }else if(peek_character(1) == '-'){
                Token t =  make_token("--", TokenType::DEC, 2);
                advance_character();
                return t;
            }
            else return make_token("-",TokenType::MINUS, 1);
        case '+':
            if(peek_character(1) == '='){
                Token t =  make_token("+=", TokenType::PLUS_EQUAL, 2);
                advance_character();
                return t;
            }else if(peek_character(1) == '+'){
                Token t =  make_token("++", TokenType::INC, 2);
                advance_character();
                return t;
            }
            else return make_token("+",TokenType::PLUS, 1);
        case '*':
            if(peek_character(1) == '='){
                Token t =  make_token("*=", TokenType::MUL_EQUAL, 2);
                advance_character();
                return t;
            }else return make_token("*",TokenType::MUL, 1);
        case '/':
            if(peek_character(1) == '='){
                Token t =  make_token("/=", TokenType::DIV_EQUAL, 2);
                advance_character();
                return t;
            }else return make_token("/",TokenType::DIV, 1);
        case '=':
            if(peek_character(1) == '='){
                Token t =  make_token("==", TokenType::EQUAL_EQUAL, 2);
                advance_character();
                return t;
            }else return make_token("=",TokenType::EQUAL, 1);
        case '>':
            if(peek_character(1) == '='){
                Token t =  make_token(">=", TokenType::GREATER_EQUAL, 2);
                advance_character();
                return t;
            }else return make_token(">",TokenType::GREATER, 1);
        case '<':
            if(peek_character(1) == '='){
                Token t =  make_token("<=", TokenType::LESS_EQUAL, 2);
                advance_character();
                return t;
            }else return make_token("<",TokenType::LESS, 1);
    }
    return std::nullopt;
}
std::optional<Token> Tokenizer::check_literals(){
    //check for string
    if(line[cursor_column] == '"'){
        std::string parsed = "";
        if(!advance_character()){
            ErrorHandler::report_error({.type = "TokenizerError", .line_number=cursor_line,
            .column = cursor_column, .line = line, .message = "String started but never ended"});
            shutoff = true;
            return std::nullopt;
        }
        parsed+=line[cursor_column];
        while(line[cursor_column] != '"'){
            if(!advance_character()){
                ErrorHandler::report_error({.type = "TokenizerError", .line_number=cursor_line,
                .column = cursor_column, .line = line, .message = "String started but never ended"});
                shutoff = true;
                return std::nullopt;
            }
            parsed+=line[cursor_column];
        }
        if(!advance_character()){
            shutoff = true;
        }
        return Token{.type=TokenType::STRING, .lexeme=parsed, .literal=parsed, .line = cursor_line,
        .column = cursor_column, .length = static_cast<uint16_t>(parsed.length())};
    }
    //check number
    if(std::isdigit(line[cursor_column])){
        std::string parsed = "";
        bool is_float = false;
        while(std::isdigit(line[cursor_column]) || line[cursor_column] == '.'){
            if(line[cursor_column] == '.'){
                is_float = true; 
            }
            parsed+=line[cursor_column];
            if(!advance_character()){
                shutoff = true;
                break;
            }
        }
        if(is_float){
            try {
                double f = std::stod(parsed);
                return Token{.type=TokenType::FLOAT, .lexeme=parsed, .literal=f, 
                    .line = cursor_line, .column = cursor_column, .length = static_cast<uint16_t>(parsed.length())};
            } catch (const std::invalid_argument& e) {
                ErrorHandler::report_error({.type = "TokenizerError", .line_number=cursor_line,
                .column = cursor_column, .line = line, .message = "Float literal is invalid"});
            } catch (const std::out_of_range& e) {
                ErrorHandler::report_error({.type = "TokenizerError", .line_number=cursor_line,
                .column = cursor_column, .line = line, .message = "Float literal is out of range"});
            }
        }else if(parsed != ""){
            try {
                int i = std::stoi(parsed);
                return Token{.type=TokenType::INT, .lexeme=parsed, .literal=i, 
                    .line = cursor_line, .column = cursor_column, .length = static_cast<uint16_t>(parsed.length())};
            } catch (const std::invalid_argument& e) {
                ErrorHandler::report_error({.type = "TokenizerError", .line_number=cursor_line,
                .column = cursor_column, .line = line, .message = "Int literal is invalid"});
            } catch (const std::out_of_range& e) {
                ErrorHandler::report_error({.type = "TokenizerError", .line_number=cursor_line,
                .column = cursor_column, .line = line, .message = "Int literal is out of range"});
            }
        }
        else{
        //check bool
            // std::string keyword = parse_keyword();
            // if(keyword == "true"){
            //     return Token{.type=TokenType::BOOL, .lexeme="true", .literal=true, 
            //         .line = cursor_line, .column = cursor_column, .length = 4};
            // }else if(keyword == "false"){
            //     return Token{.type=TokenType::BOOL, .lexeme="false", .literal=false, 
            //         .line = cursor_line, .column = cursor_column, .length = 5};
            // }

            
        }
    }else{
        //apparently this logic never worked and just would fallback to iden
        //moreover this would consume a word that might not be right
        //so this is a rewrite that doesn't consume until it's sure
        if(!std::isalpha(line[cursor_column])){
            return std::nullopt;
        }
        uint8_t i = 1;
        std::string parsed = "";
        parsed += line[cursor_column];
        while(std::isalnum(peek_character(i).value())){
            if(!peek_character(i)){
                shutoff = true;
                break;
            }
            parsed += peek_character(i).value();
            i+=1;
        }
        if(parsed == "true"){
            for(int a = 0; a < 4; a++){
                advance_character();                }
            return Token{.type=TokenType::BOOL, .lexeme="true", .literal=true, 
                 .line = cursor_line, .column = cursor_column, .length = 4};
        }else if(parsed == "false"){
            for(int a = 0; a < 5; a++){
                advance_character();
            }
            return Token{.type=TokenType::BOOL, .lexeme="false", .literal=false, 
                .line = cursor_line, .column = cursor_column, .length = 5};
        }
        return std::nullopt;
    }
    return std::nullopt;
}
Token Tokenizer::make_token(std::string lexeme, TokenType type, uint8_t length){
    Token t{.type = type, .lexeme = lexeme, 
        .literal = lexeme, .line = cursor_line, .column = cursor_column, .length = length};
    return t;
}
std::string Tokenizer::parse_keyword(){
    //keyword names now probably can have number though not as first symbol
    //and not tested
    
    if(!std::isalpha(line[cursor_column]) && !(line[cursor_column]=='_')){
        return "";
    }
    char observed = line[cursor_column];
    std::string parsed = "";
    parsed += observed;
    int i = 1;
    while(std::isalnum(observed) || observed == '_'){
        auto next = peek_character(i);
        if(!next) break;
        if(!(std::isalnum(next.value()) || next.value() == '_')) break;
        observed = *next;
        i+=1;
        parsed += observed;
        
    }
    for(int a = 0; a < i; a++){
        advance_character();
    }
    return parsed;
}
Token Tokenizer::advance(){
    //first thing is checking buffer queue. it's mostly for when there's multiple dedents
    if(!buffered_tokens.empty()){
        Token token = buffered_tokens.front();
        buffered_tokens.pop();
        return token;
    }
    //shutoff flag is set to true on EOF to prevent further checks
    if(shutoff){
        //check for dedents on EOF
        while(indent_stack.size() > 1){
            indent_stack.pop();
            buffered_tokens.push(make_token("DEDENT",TokenType::DEDENT,1));
        }
        buffered_tokens.push(make_token("FINISH",TokenType::FINISH, 1));
        auto result = buffered_tokens.front();
        buffered_tokens.pop();
        return result;
    }
    
    //parse indent if start of line
    if(cursor_column == 0 && line[cursor_column] != '\n'){ //if line is empty skip allat    
        uint8_t spaces = 0;
        uint8_t chars = 0;
        while(line[cursor_column] == ' ' || line[cursor_column] == '\t'){
            if(line[cursor_column] == ' '){
                spaces++;
                chars++;
                advance_character();
            }else{
                spaces+=4;
                chars++;
                advance_character();
            }
        }
        if(spaces > indent_stack.top()){
            indent_stack.push(spaces);
            return make_token("INDENT",TokenType::INDENT, chars);
        }
        else if(spaces < indent_stack.top()){
            while(spaces < indent_stack.top()){
                indent_stack.pop();
                buffered_tokens.push(make_token("DEDENT",TokenType::DEDENT,1));
            }
            Token token = buffered_tokens.front();
            buffered_tokens.pop();
            return token;
        }
    }
    //after parsing indents, further spaces are ignored
    while(line[cursor_column] == ' '){
        advance_character();
    }
    //hacked thing to skip until \n for comms
    if(line[cursor_column] == '#'){
        while(line[cursor_column] != '\n' && advance_character()){}
    }
    auto single_result = check_singlechars();
    if(single_result){
        if(advance_character()){
            return single_result.value();
        }
        else{
            shutoff = true;
            return single_result.value();
        }
    }

    //checking literals before keywords so that true and false don't parse as identifiers
    auto literal = check_literals();
    if(literal){
        return literal.value();
    }
    //keyword lexing of doom and despair
    std::string keyword = parse_keyword();
    if(keyword == "func"){
        return make_token("func",TokenType::FUNC,4);
    }else if(keyword == "return"){
        return make_token("return",TokenType::RETURN, 6);
    }else if(keyword == "class"){
        return make_token("class",TokenType::CLASS, 5);
    }else if(keyword == "and"){
        return make_token("and",TokenType::AND,3);
    }else if(keyword == "or"){
        return make_token("or",TokenType::OR, 2);
    }else if(keyword == "if"){
        return make_token("if",TokenType::IF,2);
    }else if(keyword == "else"){
        return make_token("else",TokenType::ELSE,4);
    }else if(keyword == "for"){
        return make_token("for",TokenType::FOR,3);
    }else if(keyword == "while"){
        return make_token("while",TokenType::WHILE,5);
    }else if(keyword == "import"){
        return make_token("import",TokenType::IMPORT,6);
    }else if(keyword == "export"){
        return make_token("export",TokenType::EXPORT,6);
    }else if(keyword == "not"){
        return make_token("not",TokenType::NOT,3);
    }else if(keyword == "in"){
        return make_token("in",TokenType::IN,2);
    }else if(keyword == "nil"){
        return make_token("nil",TokenType::NIL,3);
    }else if(keyword == "var"){
        return make_token("var",TokenType::VAR,3);
    }else if(keyword != ""){
        //if word but not word i know -> identifier
        //this makes types also identifiers since types are class names
        //also this specific thing is the reason why we check for literals before identifiers
        //cause it would fallback to this as true is a word but not valid keyword
        return make_token(keyword, TokenType::IDENTIFIER, keyword.length());
    }
    std::cout << "No keyword parsed" << std::endl;
    

    return make_token("FINISH",TokenType::FINISH, 1);
}