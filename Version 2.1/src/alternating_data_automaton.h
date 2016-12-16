#ifndef alternating_data_automaton_h
#define alternating_data_automaton_h

#include "node.h"
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

    bool is_sat(const z3::expr & e, bool print = false, z3::solver * p_solver = NULL) const
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
            if(p_solver != NULL)
                *p_solver = s;
            return true;
        }
        else
            return false;
    }

    bool is_empty_concrete_mode(const z3::expr & init, bool print = false) const
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
            if(final_check(CURRENT) && is_sat(CURRENT, true))
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
            if(print)
                std::cout << std::endl;
        }
        return true;
    }

    z3::expr get_psi(const z3::expr & before, const transition_group & tg, int step) const
    {
        std::vector<z3::expr> dnf_array = DNF_array(concrete_post(MAIN(before), tg, step));
        std::vector<z3::expr> psi_array;
        for(int i = 0; i < dnf_array.size(); i++)
        {
            if(dnf_array[i].decl().name().str() == "and")
            {
                std::vector<z3::expr> constraints_array;
                for(int j = 0; j < dnf_array[i].num_args(); j++)
                {
                    if(!dnf_array[i].arg(j).is_const())
                        constraints_array.push_back(dnf_array[i].arg(j));
                }
                if(constraints_array.size() == 0)
                {
                    return parse("true");
                }
                else
                {
                    z3::expr constraints = constraints_array[0];
                    for(int j = 1; j < constraints_array.size(); j++)
                        constraints = constraints && constraints_array[j];
                    psi_array.push_back(constraints);
                }
            }
            else if(dnf_array[i].is_const())
                return parse("true");
            else
                psi_array.push_back(dnf_array[i]);
        }
        z3::expr result = psi_array[0];
        for(int i = 1; i < psi_array.size(); i++)
            result = result || psi_array[i];
        return result;
    }

    z3::expr abstract_post(const z3::expr & before, const transition_group & tg, int step,
                           const std::vector<z3::expr> & interpolant) const
    {
        std::vector<z3::expr> dnf_array = DNF_array(concrete_post(before, tg, step));
        std::vector<z3::expr> result_array;
        for(int i = 0; i < dnf_array.size(); i++)
        {
            if(dnf_array[i].decl().name().str() == "and")
            {
                std::vector<z3::expr> states_array;
                std::vector<z3::expr> constraints_array;
                for(int j = 0; j < dnf_array[i].num_args(); j++)
                {
                    if(dnf_array[i].arg(j).is_const())
                        states_array.push_back(dnf_array[i].arg(j));
                    else
                        constraints_array.push_back(dnf_array[i].arg(j));
                }
                if(constraints_array.size() == 0)
                {
                    z3::expr states = states_array[0];
                    for(int j = 1; j < states_array.size(); j++)
                        states = states && states_array[j];
                    result_array.push_back(states);
                }
                else
                {
                    z3::expr constraints = constraints_array[0];
                    for(int j = 1; j < constraints_array.size(); j++)
                        constraints = constraints && constraints_array[j];
                    std::vector<z3::expr> new_constraints_array;
                    for(int j = 0; j < interpolant.size(); j++)
                    {
                        z3::expr new_constraint = set_step(interpolant[j], 0, step + 1);
                        if(always_implies(dnf_array[i], new_constraint))
                            new_constraints_array.push_back(new_constraint);
                    }
                    if(new_constraints_array.size() == 0)
                    {
                        if(states_array.size() == 0)
                            result_array.push_back(parse("true"));
                        else
                        {
                            z3::expr states = states_array[0];
                            for(int j = 1; j < states_array.size(); j++)
                                states = states && states_array[j];
                            result_array.push_back(states);
                        }
                    }
                    else
                    {
                        z3::expr new_constraints = new_constraints_array[0];
                        for(int j = 1; j < new_constraints_array.size(); j++)
                            new_constraints = new_constraints && new_constraints_array[j];
                        if(states_array.size() == 0)
                            result_array.push_back(new_constraints);
                        else
                        {
                            z3::expr states = states_array[0];
                            for(int j = 1; j < states_array.size(); j++)
                                states = states && states_array[j];
                            result_array.push_back(new_constraints && states);
                        }
                    }
                }
            }
            else if(dnf_array[i].is_const())
                result_array.push_back(dnf_array[i]);
            else
            {
                std::vector<z3::expr> new_constraints_array;
                for(int j = 0; j < interpolant.size(); j++)
                {
                    z3::expr new_constraint = set_step(interpolant[j], 0, step + 1);
                    if(always_implies(dnf_array[i], new_constraint))
                        new_constraints_array.push_back(new_constraint);
                }
                if(new_constraints_array.size() == 0)
                    result_array.push_back(parse("true"));
                else
                {
                    z3::expr new_constraints = new_constraints_array[0];
                    for(int j = 1; j < new_constraints_array.size(); j++)
                        new_constraints = new_constraints && new_constraints_array[j];
                    result_array.push_back(new_constraints);
                }
            }
        }
        z3::expr result = result_array[0];
        for(int i = 1; i < result_array.size(); i++)
            result = result || result_array[i];
        return result;
    }

    bool is_covered(const z3::expr & e, int step, const node * pn) const
    {
        if(!pn->_valid)
            return false;
        z3::expr temp = set_step(e, step, pn->_step);
        if(always_implies(temp, pn->_e))
        {
            //std::cout << "!!!" << e << " -> " << pn->_e << std::endl;
            return true;
        }
        else
        {
            for(int i = 0; i < pn->_nb_down; i++)
                if(is_covered(e, step, pn->_down[i]))
                    return true;
        }
        return false;
    }

    bool is_empty_abstract_mode(const z3::expr & init, bool print = false) const
    {
        node history(init, MAIN(init));
        std::vector<node *> NEXT;
        NEXT.push_back(&history);
        std::vector<z3::expr> INTERPOLANT;
        z3::solver ce_solver(context);
        while(NEXT.size() > 0)
        {
            node * p_CURRENT = NEXT[NEXT.size() - 1];
            if(!p_CURRENT->_valid)
            {
                NEXT.pop_back();
                continue;
            }
            if(print)
                std::cout << "<" << p_CURRENT->_step << "," << p_CURRENT->_e << ","
                          << p_CURRENT->_main << ">" << std::endl;
            NEXT.pop_back();
            if(is_sat(p_CURRENT->_e, print))
            {
                if(print)
                    std::cout << std::endl << "Example found. But not enough."
                              << std::endl << std::endl;
                std::vector<int> word;
                word.push_back(p_CURRENT->_symbol);
                bool bad_proved = false;
                for(node * back_track = p_CURRENT->_up;
                    back_track != NULL && bad_proved == false;
                    back_track = back_track->_up)
                {
                    std::vector<z3::expr> psi;
                    std::vector<int> psi_step;
                    z3::expr CURRENT = back_track->_e;
                    int CURRENT_STEP = back_track->_step;
                    z3::expr CURRENT_MAIN = back_track->_main;
                    if(print)
                        std::cout << "BACK: <" << CURRENT_STEP << "," << CURRENT << ","
                                  << CURRENT_MAIN << ">" << std::endl;
                    for(int i = word.size() - 1; i >= 0; i--)
                    {
                        if(print)
                            std::cout << "<" << CURRENT_STEP << "," << CURRENT << ","
                                      << CURRENT_MAIN << ">" << std::endl;
                        z3::expr current_psi = get_psi(CURRENT, _g[word[i]], CURRENT_STEP);
                        CURRENT = concrete_post(CURRENT, _g[word[i]], CURRENT_STEP++);
                        CURRENT_MAIN = main_post(CURRENT_MAIN, _g[word[i]]);
                        psi.push_back(current_psi);
                        psi_step.push_back(CURRENT_STEP);
                        if(print)
                            std::cout << "POST(" << _g[word[i]]._symbol << ") = "
                                      << "<" << CURRENT_STEP << "," << CURRENT << ","
                                      << CURRENT_MAIN << ">" << std::endl;
                        if(is_always_false(CURRENT)
                                || (final_check(CURRENT_MAIN) && !is_sat(CURRENT)))
                        {
                            bad_proved = true;
                            if(print)
                                std::cout << "Example is bad." << std::endl;
                            z3::expr latest_interpolant = parse("true");
                            for(int j = 0; j < psi.size() - 1; j++)
                            {
                                z3::expr e1 = latest_interpolant && psi[j];
                                z3::expr e2 = psi[j + 1];
                                for(int k = j + 2; k < psi.size(); k++)
                                    e2 = e2 && psi[k];
                                std::vector<z3::expr> interpolants_array = compute_interpolants_array(e1, e2);
                                latest_interpolant = interpolants_array[0];
                                for(int k = 1; k < interpolants_array.size(); k++)
                                    latest_interpolant = latest_interpolant || interpolants_array[k];
                                for(int k = 0; k < interpolants_array.size(); k++)
                                {
                                    if(interpolants_array[k].decl().name().str() != "true")
                                    {
                                        z3::expr pure_interpolant = set_step(interpolants_array[k], psi_step[j], 0);
                                        bool already = false;
                                        for(int l = 0; l < INTERPOLANT.size(); l++)
                                        {
                                            if(always_implies(INTERPOLANT[l], pure_interpolant))
                                            {
                                                //std::cout << pure_interpolant << " <- " << INTERPOLANT[l] << std::endl;
                                                already = true;
                                                break;
                                            }
                                        }
                                        if(!already)
                                        {
                                            INTERPOLANT.push_back(pure_interpolant);
                                            if(print)
                                                std::cout << "ADD INTERPOLANT: " << pure_interpolant << std::endl;
                                        }
                                    }
                                }
                            }
                            back_track->all_set_invalid();
                            back_track->_valid = true;
                            NEXT.push_back(back_track);
                            break;
                        }
                        else if(is_sat(CURRENT, false, &ce_solver) && back_track == &history)
                        {
                            if(print)
                                std::cout << "Example is good." << std::endl;
                            std::cout << ce_solver.get_model() << std::endl;
                            return false;
                        }
                        else if(is_sat(CURRENT))
                        {
                            if(print)
                                std::cout << "Example seems good. But not enough." << std::endl;
                        }
                    }
                    word.push_back(back_track->_symbol);
                    if(print)
                        std::cout << std::endl;
                }
            }
            else
            {
                for(int i = 0; i < _g.size(); i++)
                {
                    z3::expr POST_MAIN = main_post(p_CURRENT->_main, _g[i]);
                    z3::expr POST = parse("true");
                    if(final_check(POST_MAIN))
                        POST = concrete_post(p_CURRENT->_e, _g[i], p_CURRENT->_step);
                    else
                        POST = abstract_post(p_CURRENT->_e, _g[i], p_CURRENT->_step, INTERPOLANT);
                    if(print)
                        std::cout << "POST(" << _g[i]._symbol << ") = " << "<" << p_CURRENT->_step + 1
                                      << "," << POST << "," << POST_MAIN << ">" << std::endl;
                    if(is_always_false(POST_MAIN) && !is_sat(POST))
                        continue;
                    if(is_always_false(POST))
                        continue;
                    if(is_covered(POST, p_CURRENT->_step + 1, &history) == false)
                    {
                        p_CURRENT->_down[p_CURRENT->_nb_down] = new node(POST, POST_MAIN);
                        p_CURRENT->_down[p_CURRENT->_nb_down]->_step = p_CURRENT->_step + 1;
                        p_CURRENT->_down[p_CURRENT->_nb_down]->_up = p_CURRENT;
                        p_CURRENT->_down[p_CURRENT->_nb_down]->_symbol = i;
                        //std::cout << "ADD: " << POST << std::endl;
                        NEXT.push_back(p_CURRENT->_down[p_CURRENT->_nb_down]);
                        p_CURRENT->_nb_down = p_CURRENT->_nb_down + 1;
                    }
                }
            }
            if(print)
                std::cout << std::endl;
        }
        return true;
    }

    bool is_empty(const z3::expr & init, check_mode m = ABSTRACT, bool print = false) const
    {
        if(m == ABSTRACT)
            return is_empty_abstract_mode(init, print);
        else if(m == CONCRETE)
            return is_empty_concrete_mode(init, print);
    }

};

}

#endif
