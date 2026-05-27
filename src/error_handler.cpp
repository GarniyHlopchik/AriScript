#include "error_handler.h"
#include <iostream>

void ErrorHandler::report_error(CompilerError err){
    std::cerr << "\033[31m" << "[" << err.type << " Error] " << "on line "<< +err.line_number+1
    << ": " << err.message << "\033[0m"<< std::endl;
    std::cerr << err.line << std::endl;
    std::cerr << std::string(err.column, ' ') << '^' << std::endl;
}