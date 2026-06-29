#ifndef ARI_LITERAL
#define ARI_LITERAL
using Literal = std::variant<std::monostate, double, std::string, bool, int>;
#endif