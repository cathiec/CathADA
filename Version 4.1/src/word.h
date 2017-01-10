#ifndef word_h
#define word_h

#include "node.h"
#include "transition.h"

namespace cath{

class word
{

public:

    std::vector<std::string> _w1;
    std::vector<std::vector<std::string> > _w2;
    std::vector<std::vector<z3::expr> > _w3;

public:

    word()
    {}

    word(const word & w)
        :_w1(w._w1), _w2(w._w2), _w3(w._w3)
    {}

    word(const std::vector<int> & symbols,
         const std::vector<transition_group> & g,
         const std::vector<std::string> & X,
         const z3::model & m)
    {
        for(int i = 0; i < symbols.size(); i++)
        {
            _w1.push_back(g[i]._symbol);
            std::cout << "# " << g[i]._symbol << std::endl;
            std::vector<std::string> temp1;
            std::vector<z3::expr> temp2;
            for(int j = 0; j < X.size(); j++)
            {
                std::string var = X[j] + itoa(i + 1);
                for(int k = 0; k < m.num_consts(); k++)
                {
                    if(m.get_const_decl(k).name().str() == var)
                    {
                        temp1.push_back(X[j]);
                        temp2.push_back(m.get_const_interp(m.get_const_decl(k)));
                        std::cout << X[j] << " = " << m.get_const_interp(m.get_const_decl(k)) << std::endl;
                    }
                }
            }
            _w2.push_back(temp1);
            _w3.push_back(temp2);
            std::cout << "----------" << std::endl;
        }
    }

    ~word()
    {}

    word & operator=(const word & w)
    {
        _w1 = w._w1;
        _w2 = w._w2;
        _w3 = w._w3;
    }

    friend std::ostream & operator<<(std::ostream &, const word &);

};

std::ostream & operator<<(std::ostream & o, const word & w)
{
    for(int i = 0; i < w._w1.size(); i++)
    {
        o << '<' << w._w1[i] << ",{";
        for(int j = 0; j < w._w2[i].size(); j++)
        {
            o << w._w2[i][j] << " = " << w._w3[i][j] << ',';
        }
        if(w._w2[i].size() >= 1)
            o << '\b';
        o << "}> ";
    }
    return o;
}

}

#endif
