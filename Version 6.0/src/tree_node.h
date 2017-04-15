#ifndef tree_node_h
#define tree_node_h

#include <vector>
#include <iostream>

namespace cath{

class tree_node
{

public:

    std::string _path;

    std::vector<tree_node *> _down;

    bool _covered;

public:

    tree_node()
        :_path(""), _covered(false)
    {}

    tree_node(const tree_node & tn)
        :_path(tn._path), _covered(tn._covered)
    {
        for(int i = 0; i < _down.size(); i++)
            _down.push_back(new tree_node(*(tn._down[i])));
    }

    ~tree_node()
    {
        for(int i = 0; i < _down.size(); i++)
            delete _down[i];
    }

    tree_node & operator=(const tree_node & tn)
    {
        _path = tn._path;
        _covered = tn._covered;
        for(int i = 0; i < _down.size(); i++)
            _down.push_back(new tree_node(*(tn._down[i])));
    }

    void set_covered(bool print = false)
    {
        _covered = true;
        if(print)
            std::cout << "$ The node [ " << _path << " ] has been set covered." << std::endl;
        for(int i = 0; i < _down.size(); i++)
            _down[i]->set_covered(print);
    }

};

}

#endif
