#ifndef alternating_data_automaton_h
#define alternating_data_automaton_h

#include "transition.h"
#include <fstream>
#include <map>
#include <tuple>

namespace cath{

typedef enum{CONCRETE, ABSTRACT} check_mode;

class ADA
{

public:

    std::vector<std::string> _Q;
    std::map<std::string, int> _Q_index;

    std::map<std::string, int> _SIGMA;

    std::vector<std::string> _X;

    std::vector<transition_group> _g;

public:

    void read_states(std::ifstream & cur, std::string end, bool print = false)
    {
        std::string temp;
        int nb_states = 0;
        while(cur >> temp)
        {
            if(temp == end)
                break;
            else
            {
                _Q.push_back(temp);
                _Q_index[temp] = nb_states++;
                if(print)
                    std::cout << "% cath::declare : (declare-state " << temp << ")" << std::endl;
                declare(temp, _BOOL, print);
            }
        }
    }

    void read_symbols(std::ifstream & cur, std::string end, bool print = false)
    {
        std::string temp;
        int nb_symbols = 0;
        while(cur >> temp)
        {
            if(temp == end)
                break;
            else
            {
                _SIGMA[temp] = nb_symbols++;
                if(print)
                    std::cout << "% cath::declare : (declare-symbol " << temp << ")" << std::endl;
                _g.push_back(transition_group(temp, _Q.size()));
            }
        }
    }

    void read_variables(std::ifstream & cur, std::string end, bool print = false)
    {
        std::string temp;
        while(cur >> temp)
        {
            if(temp == end)
                break;
            else
            {
                _X.push_back(temp);
                if(print)
                    std::cout << "% cath::declare : (declare-variable " << temp << ")" << std::endl;
                declare(temp + "0", _INT, print);
                declare(temp + "1", _INT, print);
            }
        }
    }

    void read_transitions(std::ifstream & cur, std::string end, bool print = false)
    {
        std::string temp;
        while(cur >> temp)
        {
            if(temp == end)
                break;
            else
            {
                int pos = _SIGMA[temp];
                cur >> temp;
                std::string left = temp;
                getline(cur, temp, '\n');
                getline(cur, temp, '\n');
                if(print)
                {
                    std::cout << "% cath::declare : (declare-transition " << _g[pos]._symbol << "(" << left << ")";
                    std::cout << " = " << temp << ")" << std::endl;
                }
                _g[pos].replace(_Q_index[left], temp, print);
            }
        }
    }

    ADA()
    {}

    ADA(std::string file_name, bool print = false)
    {
        if(print)
            std::cout << "% cath::read : (read-file " << file_name << ")" << std::endl;
        std::ifstream cur(file_name);
        if(!cur.is_open())
        {
            if(print)
                std::cout << "\t-> c++::error : (std::ifstream cannot open the file)" << std::endl;
            exit(1);
        }
        std::string temp;
        while(cur >> temp)
            if(temp == "STATES")
                break;
        read_states(cur, "SYMBOLS", print);
        read_symbols(cur, "VARIABLES", print);
        read_variables(cur, "TRANSITIONS", print);
        read_transitions(cur, "END", print);
    }

    ~ADA()
    {}

    z3::expr set_step(const z3::expr & e, int step_before, int step_after) const
    {
        z3::expr result = e;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < _X.size(); i++)
            for(int j = 0; j <= step_before; j++)
            {
                std::string temp1 = _X[i] + itoa(j), temp2 = _X[i] + itoa(j + step_after - step_before);
                from.push_back(context.int_const(temp1.c_str()));
                to.push_back(context.int_const(temp2.c_str()));
            }
        return result.substitute(from, to);
    }

    z3::expr concrete_post(const z3::expr & before, const transition_group & tg, int step) const
    {
        z3::expr result = before;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < _Q.size(); i++)
        {
            from.push_back(parse(_Q[i]));
            to.push_back(set_step(tg._right[i], 1, step + 1));
        }
        return result.substitute(from, to);
    }

    z3::expr main_post(const z3::expr & before, const transition_group & tg) const
    {
        z3::expr result = before;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < _Q.size(); i++)
        {
            from.push_back(parse(_Q[i]));
            to.push_back(MAIN(tg._right[i]));
        }
        return result.substitute(from, to);
    }

    bool is_sat(const z3::expr & e, bool print = false) const
    {
        z3::expr temp = e;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < _Q.size(); i++)
        {
            from.push_back(parse(_Q[i]));
            to.push_back(parse("false"));
        }
        temp = temp.substitute(from, to);
        z3::solver s(context);
        s.add(temp);
        if(s.check())
        {
            if(print)
            {
                std::cout << "SAT" << std::endl;
                std::cout << "example: " << s.get_model() << std::endl;
            }
            return true;
        }
        else
            return false;
    }

    bool is_empty(const z3::expr & init, check_mode m = CONCRETE, bool print = false) const
    {
        std::vector<std::tuple<int, z3::expr, z3::expr> > NEXT;
        NEXT.push_back(std::make_tuple(0, init, MAIN(init)));
        std::vector<std::tuple<int, z3::expr, z3::expr> > PROCESSED;
        while(NEXT.size() > 0)
        {
            int CURRENT_STEP = std::get<0>(NEXT[NEXT.size() - 1]);
            z3::expr CURRENT = std::get<1>(NEXT[NEXT.size() - 1]);
            z3::expr CURRENT_MAIN = std::get<2>(NEXT[NEXT.size() - 1]);
            if(print)
                std::cout << "<" << CURRENT_STEP << "," << CURRENT << "," << CURRENT_MAIN << ">" << std::endl;
            if(is_sat(CURRENT, print))
                return false;
            PROCESSED.push_back(NEXT[NEXT.size() - 1]);
            NEXT.pop_back();
            for(int i = 0; i < _g.size(); i++)
            {
                z3::expr POST_MAIN = main_post(CURRENT_MAIN, _g[i]);
                z3::expr POST = concrete_post(CURRENT, _g[i], CURRENT_STEP);
                if(print)
                    std::cout << "POST(" << _g[i]._symbol << ") = " << "<" << CURRENT_STEP + 1
                                  << "," << POST << "," << POST_MAIN << ">" << std::endl;
                if(is_always_false(POST_MAIN) && !is_sat(POST))
                    continue;
                if(is_always_false(POST))
                    continue;
                bool already = false;
                for(int j = 0; j < NEXT.size(); j++)
                {
                    z3::expr target = set_step(std::get<1>(NEXT[j]), std::get<0>(NEXT[j]), CURRENT_STEP + 1);
                    if(always_implies(POST, target))
                    {
                        already = true;
                        break;
                    }
                }
                if(already == false)
                {
                    for(int j = 0; j < PROCESSED.size(); j++)
                    {
                        z3::expr target = set_step(std::get<1>(PROCESSED[j]), std::get<0>(PROCESSED[j]), CURRENT_STEP + 1);
                        if(always_implies(POST, target))
                        {
                            already = true;
                            break;
                        }
                    }
                }
                if(already == false)
                {
                    std::vector<std::tuple<int, z3::expr, z3::expr> >::iterator it = NEXT.begin();
                    while(NEXT.size() != (it - NEXT.begin()))
                    {
                        z3::expr target = set_step(std::get<1>(*it), std::get<0>(*it), CURRENT_STEP + 1);
                        if(always_implies(target, POST))
                        {
                            NEXT.erase(it);
                            continue;
                        }
                        it++;
                    }
                    it = PROCESSED.begin();
                    while(PROCESSED.size() != (it - PROCESSED.begin()))
                    {
                        z3::expr target = set_step(std::get<1>(*it), std::get<0>(*it), CURRENT_STEP + 1);
                        if(always_implies(target, POST))
                        {
                            PROCESSED.erase(it);
                            continue;
                        }
                        it++;
                    }
                    NEXT.push_back(std::make_tuple(CURRENT_STEP + 1, POST, POST_MAIN));
                }
            }
            std::cout << std::endl;
        }
        return true;
    }

};

}

#endif
