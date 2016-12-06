#ifndef NODE_H
#define NODE_H

#include "transition.h"

#define MAX_DOWN 10

namespace cath{

class node
{

public:

    expression exp;
    node * up;
    int nb_down;
    node * down[MAX_DOWN];
    int symbol;

public:

    node(const expression & e)
        :exp(e), up(NULL), nb_down(0), symbol(-1)
    {
        for(int i = 0; i < MAX_DOWN; i++)
            down[i] = NULL;
    }

    ~node()
    {
        for(int i = 0; i < MAX_DOWN; i++)
            delete down[i];
    }

    bool find_cover(const expression & e) const
    {
        if(e.always_implies(exp))
            return true;
        else
        {
            for(int i = 0; i < nb_down; i++)
                if(down[i]->find_cover(e))
                    return true;
        }
        return false;
    }

};

}

#endif
