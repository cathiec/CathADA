#ifndef transition_h
#define transition_h

#include "expr.h"
#include <vector>

namespace cath {

class transition
{

public:

    std::string symbol;
    int nb_state;
    std::string * state;
    expression * post;

public:

    transition()
        :symbol("unknown"), nb_state(0), state(NULL), post(NULL)
    {}

    transition(std::string s, int n, const std::vector<std::string> & q)
        :symbol(s), nb_state(n)
    {
        state = new std::string[n];
        for(int i = 0; i < n; i++)
            state[i] = q[i];
        post = new expression[n];
        for(int i = 0; i < n; i++)
            post[i] = expression("$false");
    }

    transition(const transition & t)
        :symbol(t.symbol), nb_state(t.nb_state)
    {
        state = new std::string[nb_state];
        for(int i = 0; i < nb_state; i++)
            state[i] = t.state[i];
        post = new expression[nb_state];
        for(int i = 0; i < nb_state; i++)
            post[i] = t.post[i];
    }

    ~transition()
    {
        delete[] state;
        delete[] post;
    }

    transition & operator=(const transition & t)
    {
        symbol = t.symbol;
        nb_state = t.nb_state;
        state = new std::string[nb_state];
        for(int i = 0; i < nb_state; i++)
            state[i] = t.state[i];
        post = new expression[nb_state];
        for(int i = 0; i < nb_state; i++)
            post[i] = t.post[i];
    }

};

}

#endif
