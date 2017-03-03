#ifndef DA_h
#define DA_h

#include <iostream>
#include <vector>
#include <tuple>

#include "context.h"

#define str std::string
#define vec std::vector
#define tup std::tuple

#define out std::cout
#define end std::endl

namespace cath {

class state
{

public:

    str _name;

    state(std::string name = "undefined")
        :_name(name)
    {}

    ~state()
    {}

};

class DA
{

public:

    state i;
    vec<state> F;
    vec<tup<str, str, str, str> > SIGMA;

    DA()
    {}

    ~DA()
    {}

    bool emptiness() const
    {
        cath::parse("(not (= L 0))", true);
    }

};

}

#endif
