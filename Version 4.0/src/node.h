#ifndef node_h
#define node_h

#include "transition.h"

namespace cath{

class node
{

public:

    int _step;
    z3::expr _e;

    node * _up;

    int _nb_down;
    node * _down[MAX_SYMBOL];

    int _symbol;

    bool _valid;

public:

    node(const z3::expr & e)
        :_step(-1), _e(e), _up(NULL), _nb_down(0), _symbol(-1), _valid(true)
    {
        for(int i = 0; i < MAX_SYMBOL; i++)
            _down[i] = NULL;
    }

    node(const node & n)
        :_step(n._step), _e(n._e), _up(n._up), _nb_down(n._nb_down), _symbol(n._symbol), _valid(n._valid)
    {
        for(int i = 0; i < MAX_SYMBOL; i++)
            _down[i] = n._down[i];
    }

    ~node()
    {
        for(int i = 0; i < MAX_SYMBOL; i++)
            delete _down[i];
    }

    node & operator=(const node & n)
    {
        _step = n._step;
        _e = n._e;
        _up = n._up;
        _nb_down = n._nb_down;
        for(int i = 0; i < MAX_SYMBOL; i++)
            _down[i] = n._down[i];
        _symbol = n._symbol;
        _valid = n._valid;
    }

    void all_set_invalid()
    {
        _valid = false;
        for(int i = 0; i < _nb_down; i++)
            _down[i]->all_set_invalid();
    }

};

}

#endif
