#ifndef alternating_data_automaton_h
#define alternating_data_automaton_h

#include "transition.h"
#include <fstream>

namespace cath{

class ADA
{

public:

    std::vector<std::string> Q;
    std::vector<std::string> X;

public:

    void read_states(std::ifstream & cur, std::string end)
    {
        std::string temp;
        while(cur >> temp)
        {
            if(temp == end)
                break;
            else
            {
                Q.push_back(temp);
                declare(temp, _BOOL);
            }
        }
    }

    ADA()
    {}

    ADA(std::string file_name)
    {
        std::ifstream cur(file_name);
        std::string temp;
        while(cur >> temp)
            if(temp == "STATES")
                break;
        read_states(cur, "SYMBOLS");
    }

    ~ADA()
    {

    }

};

}

#endif
