#ifndef expression_h
#define expression_h

#include <iostream>
#include <vector>
#include "context.h"

namespace cath{

z3::expr NNF(const z3::expr e)
{
    z3::goal g(context);
    g.add(e);
    z3::tactic t(context, "nnf");
    z3::apply_result r = t(g);
    return r[0].as_expr();
}

z3::expr MAIN(const z3::expr & e)
{
    if(e.is_app())
    {
        if(e.is_bool())
        {
            if(e.decl().name().str() == "and")
            {
                z3::expr result = MAIN(e.arg(0));
                int num = e.num_args();
                for(int i = 1; i < num; i++)
                {
                    if(e.arg(i).decl().name().str() == "and"
                            || e.arg(i).decl().name().str() == "or"
                            || e.arg(i).decl().name().str() == "not")
                        result = result && MAIN(e.arg(i));
                    else if(e.arg(i).is_const())
                        result = result && e.arg(i);
                }
                return result.simplify();
            }
            else if(e.decl().name().str() == "or")
            {
                z3::expr result = MAIN(e.arg(0));
                int num = e.num_args();
                for(int i = 1; i < num; i++)
                {
                    if(e.arg(i).decl().name().str() == "and"
                            || e.arg(i).decl().name().str() == "or"
                            || e.arg(i).decl().name().str() == "not")
                        result = result || MAIN(e.arg(i));
                    else if(e.arg(i).is_const())
                        result = result || e.arg(i);
                }
                return result.simplify();
            }
            else if(e.decl().name().str() == "not")
                return MAIN(e.arg(0)).simplify();
            else if(e.is_const())
                return e;
            else
                return context.bool_val(false);
        }
    }
    else if(e.is_quantifier())
    {
        return MAIN(e.body()).simplify();
    }
    else
    {
        return context.bool_val(false);
    }
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
