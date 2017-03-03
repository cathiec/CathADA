#ifndef context_h
#define context_h

#include <z3++.h>

namespace cath{

z3::context context;
z3::func_decl_vector _DECLS(context);

static z3::sort_vector _DEFAULT_SORTS(context);
static z3::sort _INT = context.int_sort();
static z3::sort _BOOL = context.bool_sort();

void declare(std::string name, z3::sort sort, bool print = false)
{
    bool already = false;
    for(int i = 0; i < _DECLS.size(); i++)
    {
        if(_DECLS[i].name().str() == name)
        {
            already = true;
            break;
        }
    }
    if(!already)
    {
        z3::func_decl temp = context.function(name.c_str(), 0, 0, sort);
        _DECLS.push_back(temp);
        if(print)
            std::cout << "\t-> z3::declare : " << temp << std::endl;
    }
}

std::string itoa(int i)
{
    char temp[10];
    std::sprintf(temp, "%d", i);
    return std::string(temp);
}

z3::expr parse(std::string e, bool print = false)
{
    std::string smt2 = "(assert " + e + ")";
    if(print)
        std::cout << "\t-> z3::parse : " << smt2 << std::endl;
    return context.parse_string(smt2.c_str(), _DEFAULT_SORTS, _DECLS);
}

}

#endif
