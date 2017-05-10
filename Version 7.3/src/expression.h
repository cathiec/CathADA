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

bool already_in(const z3::expr & e, const std::vector<z3::expr> & v)
{
    for(int i = 0; i < v.size(); i++)
    {
        if(always_implies(e, v[i]) && always_implies(v[i], e))
            return true;
    }
    return false;
}

std::vector<z3::expr> pick_states(const z3::expr & e)
{
    std::vector<z3::expr> result_array;
    if(e.is_bool())
    {
        if(e.is_const() && e.decl().name().str() != "false" && e.decl().name().str() != "true")
        {
            if(!already_in(e, result_array))
                result_array.push_back(e);
        }
        else
        {
            for(int i = 0; i < e.num_args(); i++)
            {
                std::vector<z3::expr> temp = pick_states(e.arg(i));
                for(int j = 0; j < temp.size(); j++)
                    if(!already_in(temp[j], result_array))
                        result_array.push_back(temp[j]);
            }
        }
    }
    else if(e.is_quantifier())
    {
        std::vector<z3::expr> temp = pick_states(e.body());
        for(int i = 0; i < temp.size(); i++)
            if(!already_in(temp[i], result_array))
                result_array.push_back(temp[i]);
    }
    return result_array;
}

z3::expr compute_interpolant(const z3::expr & e1, const z3::expr & e2)
{
    //std::cout << "@@@ " << e1 << std::endl;
    //std::cout << "@@@ " << e2 << std::endl;
    z3::expr pattern = z3::expr(context, Z3_mk_interpolant(context, e1));
    Z3_model md;
    pattern = (pattern && e2);
    Z3_ast_vector temp;
    Z3_lbool result = Z3_compute_interpolant(context, pattern, 0, &temp, &md);
    //std::cout << "### " << z3::expr(context, Z3_ast_vector_get(context, temp, 0)) << std::endl;
    if(result == Z3_L_FALSE)
        return z3::expr(context, Z3_ast_vector_get(context, temp, 0));
    return parse("true");
}

}

#endif
