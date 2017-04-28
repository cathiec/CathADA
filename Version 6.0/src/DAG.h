#ifndef DAG_h
#define DAG_h

#include "transition.h"

#include <vector>
#include <iostream>

namespace cath{

class DAG_node
{

public:

    z3::expr _e;

    z3::expr _interpolants;

    std::vector<std::string> _f_symbol;
    std::vector<z3::expr> _f_phi;
    std::vector<DAG_node *> _father;

    std::vector<std::string> _symbol;
    std::vector<z3::expr> _phi;
    std::vector<DAG_node *> _succ;

public:

    DAG_node(const z3::expr & e)
        :_e(e), _interpolants(parse("true"))
    {}

    ~DAG_node()
    {
        // delete the allocated nodes in tree_node not here
    }
};

class DAG_path
{

public:

    std::vector<z3::expr> _phi;

    std::vector<DAG_node *> _node;

public:

    DAG_path()
    {}

    ~DAG_path()
    {}

    void add(const z3::expr e, DAG_node * n)
    {
        _phi.push_back(e);
        _node.push_back(n);
    }

    void print() const
    {
        for(int i = 0; i < _phi.size(); i++)
        {
            std::cout << "[" << _phi[i] << "," << _node[i]->_e << "] ";
        }
        std::cout << std::endl;
    }
};

}

#endif
