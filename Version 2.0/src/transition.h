#ifndef transition_h
#define transition_h

#include "expression.h"

namespace cath{

class transition_group
{

public:

    std::string _symbol;

    int _nb_transitions;
    std::vector<z3::expr>  _left;
    std::vector<z3::expr> _right;

public:

    transition_group()
        :_symbol("undefined"), _nb_transitions(0)
    {}

    transition_group(std::string s)
        :_symbol(s), _nb_transitions(0)
    {}

    transition_group(const transition_group & tg)
        :_symbol(tg._symbol), _nb_transitions(tg._nb_transitions), _left(tg._left), _right(tg._right)
    {}

    ~transition_group()
    {}

    void add(std::string & left, std::string & right, bool print = false)
    {
        _left.push_back(parse(left, print));
        _right.push_back(parse(right, print));
        _nb_transitions++;
    }

};

}

#endif
