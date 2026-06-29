#ifndef ARI_AST_DEFINITION
#define ARI_AST_DEFINITION

/*it's based off a flat vector of a struct. the struct tries not to do ptrs or refs
at the same time children of a node will be placed directly after it
this way it's very memory-local = very gud
the ast also needs to be able to represent itself in the console for debug
i kinda hate macro stuff in cpp, so just writing things twice it is
*/

#include <string>
#include <memory>
#include "type.h"
#include "tokenizer.h"
class Expr {
    public:
        Type* type;
        virtual ~Expr() = default;
};


#include "ast_utils.h"

#endif