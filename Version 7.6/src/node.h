#ifndef node_h
#define node_h

#include "transition.h"

namespace cath{

class node
{

public:

    int _num;           // order

    int _step;

    z3::expr _lambda;   // conjunction of interpolants

    node * _father;
    int _symbol;
    z3::expr _theta;

    std::vector<z3::expr> _R;   // Booleans (states)

    std::vector<int> _down_symbol;
    std::vector<z3::expr> _down_theta;
    std::vector<node *> _down;

    std::vector<node *> _covering;  // the nodes that (cover) / (are implied by) the current node
    std::vector<node *> _covered;   // the nodes that (are covered by) / (imply) the current node

public:

    node()
        :_num(-1), _step(-1), _lambda(parse("true")), _father(NULL), _symbol(-1), _theta(parse("true"))
    {}

    node(const node & n)
        :_num(n._num), _step(n._step), _lambda(n._lambda), _father(n._father), _symbol(n._symbol),
         _theta(n._theta), _R(n._R), _down_symbol(n._down_symbol), _down_theta(n._down_theta),
         _down(n._down), _covering(n._covering), _covered(n._covered)
    {}

    ~node()
    {}

    node & operator=(const node & n)
    {
        _num = n._num;
        _step = n._step;
        _lambda = n._lambda;
        _father = n._father;
        _symbol = n._symbol;
        _theta = n._theta;
        _R = n._R;
        _down_symbol = n._down_symbol;
        _down_theta = n._down_theta;
        _down = n._down;
        _covering = n._covering;
        _covered = n._covered;
    }

    void remove_all_covered()
    {
        for(int i = 0; i < _covered.size(); i++)
        {
            node * temp = _covered[i];
            std::vector<node *>::iterator it;
            for(it = temp->_covering.begin(); it != temp->_covering.end(); it++)
            {
                if((*it)->_num == _num)
                {
                    temp->_covering.erase(it);
                    break;
                }
            }
        }
        _covered.clear();
        for(int i = 0; i < _down.size(); i++)
            _down[i]->remove_all_covered();
    }

    bool is_covered() const
    {
        for(const node * temp = this; temp != NULL; temp = temp->_father)
        {
            if(temp->_covering.size() > 0)
                return true;
        }
        return false;
    }

    bool is_ancestor_of(node * n) const
    {
        for(node * temp = n->_father; temp != NULL; temp = temp->_father)
        {
            if(_num == temp->_num)
                return true;
        }
        return false;
    }

};

}

#endif
