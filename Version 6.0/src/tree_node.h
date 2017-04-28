#ifndef tree_node_h
#define tree_node_h

#include "DAG.h"

namespace cath{

class tree_node
{

public:

    std::vector<std::string> _path;

    tree_node * _father;

    std::vector<DAG_node *> _ele;

    std::vector<tree_node *> _down;

    bool _covered;

public:

    tree_node()
        :_father(NULL), _covered(false)
    {}

    ~tree_node()
    {
        for(int i = 0; i < _down.size(); i++)
            delete _down[i];
    }

    void set_covered(bool print = false)
    {
        _covered = true;
        if(print)
        {
            std::cout << "$ The node -";
            for(int i = 0; i < _path.size(); i++)
                std::cout << _path[i] << "-";
            std::cout << " has been set covered." << std::endl;
        }
        for(int i = 0; i < _down.size(); i++)
            _down[i]->set_covered(print);
    }

    z3::expr z3() const
    {
        if(_ele.size() == 0)
            return parse("false");
        z3::expr e = (_ele[0]->_e && _ele[0]->_interpolants).simplify();
        for(int i = 1; i < _ele.size(); i++)
            e = e || (_ele[i]->_e && _ele[i]->_interpolants).simplify();
        return e;
    }

};

}

#endif
