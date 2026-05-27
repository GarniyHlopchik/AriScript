#ifndef ARI_ERR_HANDLER_IMPLEMENTATION
#define ARI_ERR_HANDLER_IMPLEMENTATION

#include <string>
#include <stdint.h>
struct CompilerError{
    std::string type;
    uint32_t line_number;
    uint16_t column;
    std::string line;
    std::string message;
};
class ErrorHandler{
    public:
        static void report_error(CompilerError err);
    private:
};

#endif