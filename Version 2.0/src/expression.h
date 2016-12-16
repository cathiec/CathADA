#ifndef expression_h
#define expression_h

#include <iostream>
#include <vector>
#include "context.h"

namespace cath{

z3::expr parse(std::string e, bool print = false)
{
    std::string smt2 = "(assert " + e + ")";
    if(print)
        std::cout << "\t-> z3::parse : " << smt2 << std::endl;
    return context.parse_string(smt2.c_str(), _DEFAULT_SORTS, _DECLS);
}

z3::expr NNF(const z3::expr & e)
{
    z3::goal g(context);
    g.add(e);
    z3::tactic t(context, "nnf");
    z3::apply_result r = t(g);
    return r[0].as_expr();
}

z3::expr DNF(const z3::expr & e)
{
    z3::goal g(context);
    g.add(NNF(e));
    z3::tactic t = z3::repeat(z3::tactic(context, "split-clause") | z3::tactic(context, "skip"));
    z3::apply_result r = t(g);
    z3::expr result = r[0].as_expr();
    for(int i = 1; i < r.size(); i++)
        result = result || r[i].as_expr();
    return result;
}

std::vector<z3::expr> DNF_array(const z3::expr & e)
{
    z3::goal g(context);
    g.add(NNF(e));
    z3::tactic t = z3::repeat(z3::tactic(context, "split-clause") | z3::tactic(context, "skip"));
    z3::apply_result r = t(g);
    std::vector<z3::expr> result;
    for(int i = 0; i < r.size(); i++)
        result.push_back(r[i].as_expr());
    return result;
}

z3::expr MAIN(const z3::expr & e)
{
    if(e.is_app())
    {
        if(e.is_bool())
        {
            if(e.decl().name().str() == "and")
            {
                std::vector<z3::expr> result_array;
                for(int i = 0; i < e.num_args(); i++)
                {
                    if(MAIN(e.arg(i)).decl().name().str() != "false")
                        result_array.push_back(MAIN(e.arg(i)));
                }
                if(result_array.size() == 0)
                    return context.bool_val(false);
                else
                {
                    z3::expr result = result_array[0];
                    for(int i = 1; i < result_array.size(); i++)
                        result = result && result_array[i];
                    return result;
                }
            }
            else if(e.decl().name().str() == "or")
            {
                std::vector<z3::expr> result_array;
                for(int i = 0; i < e.num_args(); i++)
                    result_array.push_back(MAIN(e.arg(i)));
                if(result_array.size() == 0)
                    return context.bool_val(false);
                else
                {
                    z3::expr result = result_array[0];
                    for(int i = 1; i < result_array.size(); i++)
                        result = result || result_array[i];
                    return result;
                }
            }
            else if(e.decl().name().str() == "not")
                return MAIN(e.arg(0));
            else if(e.is_const())
                return e;
            else
                return context.bool_val(false);
        }
    }
    else if(e.is_quantifier())
    {
        return MAIN(e.body());
    }
    else
    {
        return context.bool_val(false);
    }
}

bool always_implies(const z3::expr & e1, const z3::expr & e2)
{
    //std::cout << "CHECK: " << e1 << " -> " << e2 << std::endl;
    z3::solver s(context);
    s.add(!z3::implies(e1, e2));
    if(s.check() == z3::unsat)
    {
        //std::cout << e1 << " -> " << e2 << std::endl;
        return true;
    }
    else
        return false;
}

bool is_always_false(const z3::expr & e)
{
    return always_implies(e, parse("false"));
}

z3::expr compute_interpolant(const z3::expr & e1, const z3::expr & e2)
{
    z3::expr pattern = z3::expr(context, Z3_mk_interpolant(context, e1));
    Z3_model md;
    pattern = (pattern && e2);
    Z3_ast_vector temp;
    Z3_lbool result = Z3_compute_interpolant(context, pattern, 0, &temp, &md);
    if(result == Z3_L_FALSE)
        return z3::expr(context, Z3_ast_vector_get(context, temp, 0));
    return parse("true");
}

}

#endif
