#ifndef node_h
#define node_h

#include "transition.h"

namespace cath{

class node
{

public:

    int _step;
    z3::expr _e;
    z3::expr _main;

    node * _up;

    int _nb_down;
    node * _down[MAX_SYMBOL];

    int _symbol;

    bool _valid;

public:

    node(const z3::expr & e, const z3::expr & m)
        :_step(0), _e(e), _main(m), _up(NULL), _nb_down(0), _symbol(-1), _valid(true)
    {
        for(int i = 0; i < MAX_SYMBOL; i++)
            _down[i] = NULL;
    }

    ~node()
    {
        for(int i = 0; i < MAX_SYMBOL; i++)
            delete _down[i];
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
