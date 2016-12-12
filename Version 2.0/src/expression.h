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

class expression
{

public:

    int step;
    z3::expr _z3;

public:

    expression()
        :step(0), _z3(context.bool_val(true))
    {}

    expression(const expression & e)
        :step(e.step), _z3(e._z3)
    {}

    expression(const z3::expr e)
        :step(1), _z3(e)
    {}

    expression(std::string e)
        :step(1), _z3(context.bool_val(true))
    {
        std::string smt2 = "(assert " + e + ")";
        _z3 = context.parse_string(smt2.c_str(), _DEFAULT_SORTS, _DECLS);
    }

    ~expression()
    {}

    expression & operator=(const expression & e)
    {
        step = e.step;
        _z3 = e._z3;
        return *this;
    }

    expression & operator=(const z3::expr & e)
    {
        step = 0;
        _z3 = e;
        return *this;
    }

    expression NNF() const
    {
        return cath::NNF(_z3);
    }

    expression MAIN() const
    {
        return cath::MAIN(_z3);
    }

    friend std::ostream & operator<<(std::ostream &, const expression &);

};

std::ostream & operator<<(std::ostream & o, const expression & e)
{
    o << e._z3;
    return o;
}

expression bool_val(bool b)
{
    return expression(context.bool_val(b));
}

expression bool_const(std::string b)
{
    return expression(context.bool_const(b.c_str()));
}

expression int_const(std::string i)
{
    return expression(context.int_const(i.c_str()));
}

}

#endif
