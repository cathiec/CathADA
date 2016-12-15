#ifndef context_h
#define context_h

#include <z3++.h>
#include "expression.h"

namespace cath{

z3::context context;
z3::func_decl_vector _DECLS(context);

static z3::sort_vector _DEFAULT_SORTS(context);
static z3::sort _INT = context.int_sort();
static z3::sort _BOOL = context.bool_sort();

void declare(std::string name, z3::sort sort, bool print = false)
{
    z3::func_decl temp = context.function(name.c_str(), 0, 0, sort);
    _DECLS.push_back(temp);
    if(print)
        std::cout << "\t-> z3::declare : " << temp << std::endl;
}

}

#endif
