#include "global_types.h"
#include <unordered_map>
class AriVirtualMachine{
    public:
        AriVirtualMachine() = default;
        ~AriVirtualMachine();
        void register_value(uint16_t identifier, Variable var);
        void free_value(uint16_t identifier);
    private:
        //key is identifier passed by interpreter, 2 bytes
        std::unordered_map<uint16_t, Variable> registry;
};