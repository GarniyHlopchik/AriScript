#include <unordered_map>
#include <cstdint>
#include <vector>
#include <stack>
#include "virtual_machine.h"
#include <string>
class AriInterpreter{
    public:
        AriInterpreter() = default;
        ~AriInterpreter() = default;
        void evaluate_bytecode(std::vector<uint8_t>& bytecode);
    private:
        void enable_panic(const std::string& msg);
        AriVirtualMachine vm;
        std::stack<uint16_t> stack;
};