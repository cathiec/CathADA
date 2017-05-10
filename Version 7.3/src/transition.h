#ifndef transition_h
#define transition_h

#include "expression.h"

namespace cath{

class transition_group
{

public:

    std::string _symbol;

    std::vector<z3::expr> _right;

public:

    transition_group()
        :_symbol("undefined")
    {}

    transition_group(std::string s, int nb_states = 0)
        :_symbol(s)
    {
        for(int i = 0; i < nb_states; i++)
        {
            _right.push_back(parse("false", false));
        }
    }

    transition_group(const transition_group & tg)
        :_symbol(tg._symbol), _right(tg._right)
    {}


    ~transition_group()
    {}

    transition_group & operator=(const transition_group & tg)
    {
        _symbol = tg._symbol;
        _right= tg._right;
    }

    void replace(int pos, std::string right, bool print = false)
    {
        _right[pos] = parse(right, print);
    }

};

}

#endif
