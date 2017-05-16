#ifndef alternating_data_automaton_h
#define alternating_data_automaton_h

#include "word.h"
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

    std::string _i;

    std::vector<std::string> _F;
    std::map<std::string, int> _F_index;

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

    void read_initial_state(std::ifstream & cur, std::string end, bool print = false)
    {
        std::string temp;
        while(cur >> temp)
        {
            if(temp == end)
                break;
            else
            {
                _i = temp;
                if(print)
                    std::cout << "% cath::declare : (declare-initial-state " << temp << ")" << std::endl;
            }
        }
    }

    void read_final_states(std::ifstream & cur, std::string end, bool print = false)
    {
        std::string temp;
        while(cur >> temp)
        {
            if(temp == end)
                break;
            else
            {
                _F.push_back(temp);
                _F_index[temp] = 1;
                if(print)
                    std::cout << "% cath::declare : (declare-final-state " << temp << ")" << std::endl;
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
                _SIGMA[temp] = ++nb_symbols;
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
                int pos = _SIGMA[temp] - 1;
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
        read_states(cur, "INITIAL", print);
        read_initial_state(cur, "FINAL", print);
        read_final_states(cur, "SYMBOLS", print);
        read_symbols(cur, "VARIABLES", print);
        read_variables(cur, "TRANSITIONS", print);
        read_transitions(cur, "END", print);
    }

    ADA(const ADA & a)
        :_Q(a._Q), _i(a._i), _F(a._F), _F_index(a._F_index), _Q_index(a._Q_index), _SIGMA(a._SIGMA), _X(a._X), _g(a._g)
    {}

    ~ADA()
    {}

    ADA & operator=(const ADA & a)
    {
        _Q = a._Q;
        _i = a._i;
        _F = a._F;
        _F_index = a._F_index;
        _Q_index = a._Q_index;
        _SIGMA = a._SIGMA;
        _X = a._X;
        _g = a._g;
    }

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
        return result.substitute(from, to).simplify();
    }

    bool is_always_false(const z3::expr & e) const
    {
        if(always_implies(e, parse("false")))
            return true;
        else
            return false;
    }

    bool is_sat(const z3::expr & e, bool print = false, z3::solver * p_solver = NULL) const
    {
        z3::expr temp = e;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < _Q.size(); i++)
        {
            int exist = _F_index.find(_Q[i])->second;
            if(exist != 1)
            {
               from.push_back(parse(_Q[i]));
               to.push_back(parse("false"));
            }
            else
            {
                from.push_back(parse(_Q[i]));
                to.push_back(parse("true"));
            }
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

    bool is_empty_concrete_mode(bool print = false) const
    {
        z3::expr init = parse(_i);
        std::vector<std::tuple<int, z3::expr> > NEXT;
        NEXT.push_back(std::make_tuple(0, init));
        std::vector<std::tuple<int, z3::expr> > PROCESSED;
        while(NEXT.size() > 0)
        {
            int CURRENT_STEP = std::get<0>(NEXT[NEXT.size() - 1]);
            z3::expr CURRENT = std::get<1>(NEXT[NEXT.size() - 1]);
            if(print)
                std::cout << "<" << CURRENT_STEP << "," << CURRENT << ">" << std::endl;
            if(is_sat(CURRENT, true))
                return false;
            PROCESSED.push_back(NEXT[NEXT.size() - 1]);
            NEXT.pop_back();
            for(int i = 0; i < _g.size(); i++)
            {
                z3::expr POST = concrete_post(CURRENT, _g[i], CURRENT_STEP);
                if(print)
                    std::cout << "POST(" << _g[i]._symbol << ") = " << "<" << CURRENT_STEP + 1
                                  << "," << POST << ">" << std::endl;
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
                    std::vector<std::tuple<int, z3::expr> >::iterator it = NEXT.begin();
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
                    NEXT.push_back(std::make_tuple(CURRENT_STEP + 1, POST));
                }
            }
            if(print)
                std::cout << std::endl;
        }
        return true;
    }

    z3::expr abstract_post(const z3::expr & before, const transition_group & tg, int step,
                           const std::vector<z3::expr> & interpolant) const
    {
        z3::expr cp = concrete_post(before, tg, step);
        z3::expr result = parse("true");
        for(int i = 0; i < interpolant.size(); i++)
        {
            //std::cout << "### " << cp << std::endl;
            //std::cout << "--- " << set_step(interpolant[i], 0, step + 1) << std::endl;
            if(always_implies(cp, set_step(interpolant[i], 0, step + 1)))
            {
                //std::cout << "+++ I = " << set_step(interpolant[i], 0, step + 1) << std::endl;
                result = result && set_step(interpolant[i], 0, step + 1);
            }
            //else
                //std::cout << "   NO" << std::endl;
        }
        return result;
    }

    bool is_covered(const z3::expr & e, int step, const node * pn) const
    {
        if(!pn->_valid)
            return false;
        z3::expr temp = set_step(e, step, pn->_step);
        //std::cout << "!!!" << temp << " -> " << pn->_e << std::endl;
        if(always_implies(temp, pn->_e))
        {
            //std::cout << "!!!" << temp << " -> " << pn->_e << std::endl;
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

    z3::expr time_stamp(const z3::expr & e, int step) const
    {
        z3::expr result = e;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < _Q.size(); i++)
        {
            std::string temp1 = _Q[i], temp2 = _Q[i] + "_" + itoa(step);
            from.push_back(context.bool_const(temp1.c_str()));
            to.push_back(context.bool_const(temp2.c_str()));
        }
        return result.substitute(from, to);
    }

    z3::expr remove_stamp(const z3::expr & e, int step) const
    {
        z3::expr result = e;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < _Q.size(); i++)
        {
            std::string temp1 = _Q[i] + "_" + itoa(step), temp2 = _Q[i];
            from.push_back(context.bool_const(temp1.c_str()));
            to.push_back(context.bool_const(temp2.c_str()));
        }
        return result.substitute(from, to);
    }

    bool is_empty_abstract_mode(bool print = false) const
    {
        z3::expr init = parse(_i);
        if(is_sat(init))
        {
            if(print)
            {
                std::cout << "Example is good." << std::endl;
                std::cout << "The automaton accepts: EMPTY WORD." << std::endl;
            }
            return false;
        }
        node history(init);
        std::vector<node *> NEXT;
        NEXT.push_back(&history);
        std::vector<z3::expr> INTERPOLANT;
        INTERPOLANT.push_back(parse("false"));
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
                std::cout << "<" << p_CURRENT->_step << "," << p_CURRENT->_e << ">" << std::endl;
            NEXT.pop_back();
            if(is_sat(p_CURRENT->_e, false, &ce_solver))
            {
                if(print)
                {
                    std::cout << "Example is good." << std::endl;
                    std::cout << std::endl << "Example found. But not enough."
                              << std::endl << std::endl;
                }
                std::vector<int> symbols;
                symbols.push_back(p_CURRENT->_symbol);
                bool bad_proved = false;
                int back_step = 0;
                for(node * back_track = p_CURRENT->_up;
                    back_track != NULL && bad_proved == false;
                    back_track = back_track->_up)
                {
                    back_step++;
                    std::vector<int> psi_step;
                    z3::expr CURRENT = back_track->_e;
                    int CURRENT_STEP = back_track->_step;
                    if(print)
                        std::cout << "BACK: <" << CURRENT_STEP << "," << CURRENT << ">" << std::endl;
                    for(int i = symbols.size() - 1; i >= 0; i--)
                    {
                        if(print)
                            std::cout << "<" << CURRENT_STEP << "," << CURRENT << ">" << std::endl;
                        CURRENT = concrete_post(CURRENT, _g[symbols[i]], CURRENT_STEP++).simplify();
                        psi_step.push_back(CURRENT_STEP);
                        //std::cout << "--- STEP ADD " << CURRENT_STEP << std::endl;
                        if(print)
                            std::cout << "POST(" << _g[symbols[i]]._symbol << ") = "
                                      << "<" << CURRENT_STEP << "," << CURRENT << ">" << std::endl;
                        if(is_always_false(CURRENT) ||
                                (i == 0 && !is_sat(CURRENT)))
                        {
                            bad_proved = true;
                            if(print)
                                std::cout << "Example is bad." << std::endl;
                            // compute interpolants
                            std::vector<z3::expr> e;
                            e.push_back(time_stamp(back_track->_e, 0));
                            //std::cout << "*** " << time_stamp(back_track->_e, 0) << std::endl;
                            std::vector<z3::expr> state_set = pick_states(back_track->_e);
                            int j = symbols.size() - 1;
                            int k = 0;
                            while(j >= i)
                            {
                                z3::expr right_0 = concrete_post(state_set[0], _g[symbols[j]], psi_step[k] - 1);
                                z3::expr right = right_0;
                                z3::expr temp = z3::implies(time_stamp(state_set[0], k), time_stamp(right, k + 1));
                                for(int m = 1; m < state_set.size(); m++)
                                {
                                    z3::expr right_m = concrete_post(state_set[m], _g[symbols[j]], psi_step[k] - 1);
                                    right = right || right_m;
                                    temp = temp && z3::implies(time_stamp(state_set[m], k), time_stamp(right_m, k + 1));
                                }
                                state_set = pick_states(right);
                                k++;
                                e.push_back(temp);
                                //std::cout << "*** " << temp << std::endl;
                                j--;
                            }
                            z3::expr final_one = parse("true");
                            for(int j = 0; j < state_set.size(); j++)
                            {
                                std::string xxx = state_set[j].decl().name().str();
                                if(_F_index.find(xxx)->second == 1)
                                    final_one = final_one && z3::implies(parse("true"), time_stamp(state_set[j], k));
                                else
                                    final_one = final_one && z3::implies(time_stamp(state_set[j], k), parse("false"));
                            }
                            e.push_back(final_one);
                            //std::cout << "*** " << final_one << std::endl;
                            z3::expr latest_interpolant = parse("true");
                            k = 0;
                            for(int j = 0; j < e.size() - 1; j++)
                            {
                                //std::cout << "j = " << j << " SO j < " << e.size() - 1 << std::endl;
                                z3::expr e1 = latest_interpolant && e[j];
                                z3::expr e2 = e[j + 1];
                                for(int m = j + 2; m < e.size(); m++)
                                    e2 = e2 && e[m];
                                //std::cout << "e1 = " << e1 << std::endl;
                                //std::cout << "e2 = " << e2 << std::endl;
                                z3::expr interpolant = NNF(compute_interpolant(e1, e2).simplify());
                                //std::cout << "INTERPOLANT = " << interpolant << std::endl;
                                z3::expr pure_interpolant = parse("true");
                                z3::expr with_stamp = parse("true");
                                if(j < e.size() - 2)
                                {
                                    with_stamp = interpolant;
                                    //std::cout << "*BEFORE [" << k << "] : " << with_stamp << std::endl;
                                    pure_interpolant = set_step(interpolant, psi_step[j] - 1, 0);
                                    pure_interpolant = remove_stamp(pure_interpolant, k);
                                    k++;
                                }
                                else
                                {
                                    with_stamp = interpolant;
                                    //std::cout << "BEFORE [" << k << "] : " << with_stamp << std::endl;
                                    pure_interpolant = set_step(interpolant, psi_step[j - 1] - 1, 0);
                                    pure_interpolant = remove_stamp(pure_interpolant, k);
                                    k++;
                                }
                                //std::cout << "PURE = " << pure_interpolant << std::endl;
                                latest_interpolant = interpolant;
                                bool already = false;
                                for(int m = 0; m < INTERPOLANT.size(); m++)
                                {
                                    if(always_implies(INTERPOLANT[m], pure_interpolant) && always_implies(pure_interpolant, INTERPOLANT[m]))
                                    {
                                        already = true;
                                        break;
                                    }
                                }
                                if(!already)
                                {
                                    INTERPOLANT.push_back(pure_interpolant);
                                    if(print)
                                        std::cout << "ADD INTERPOLANT: " << pure_interpolant << std::endl;
                                        //std::cout << "ADD INTERPOLANT: " << with_stamp << std::endl;
                                }
                            }
                            if(back_step >= BACK_STEP || back_track == &history)
                            {
                                //std::cout << back_step << std::endl;
                                //std::cout << BACK_STEP << std::endl;
                                back_track->all_set_invalid();
                                back_track->_valid = true;
                                NEXT.push_back(back_track);
                                break;
                            }
                            else
                                bad_proved = false;
                        }
                        else if(is_sat(CURRENT, false, &ce_solver) && back_track == &history)
                        {
                            if(print)
                            {
                                std::cout << std::endl << "Example is good." << std::endl;
                                z3::model final_model = ce_solver.get_model();
                                word result_word(symbols, _g, _X, final_model);
                                std::cout << std::endl << "The automaton accepts:" << std::endl << result_word << std::endl;
                                //std::cout << ce_solver.get_model() << std::endl;
                            }
                            return false;
                        }
                        else if(is_sat(CURRENT))
                        {
                            if(print)
                                std::cout << std::endl << "Example seems good. But not enough." << std::endl;
                        }
                    }
                    symbols.push_back(back_track->_symbol);
                    if(print)
                        std::cout << std::endl;
                }
            }
            else
            {
                for(int i = 0; i < _g.size(); i++)
                {
                    z3::expr POST = abstract_post(p_CURRENT->_e, _g[i], p_CURRENT->_step, INTERPOLANT).simplify();
                    if(print)
                        std::cout << "POST(" << _g[i]._symbol << ") = " << "<" << p_CURRENT->_step + 1
                                      << "," << POST << ">" << std::endl;
                    if(is_always_false(POST))
                        continue;
                    if(is_covered(POST, p_CURRENT->_step + 1, &history) == false)
                    {
                        p_CURRENT->_down[p_CURRENT->_nb_down] = new node(POST);
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

    bool is_empty(check_mode m = ABSTRACT, bool print = false) const
    {
        if(m == ABSTRACT)
            return is_empty_abstract_mode(print);
        else if(m == CONCRETE)
            return is_empty_concrete_mode(print);
    }

    ADA complement() const
    {
        ADA result = *this;
        z3::expr_vector from(context), to(context);
        std::vector<std::string> new_F;
        for(int i = 0; i < result._Q.size(); i++)
        {
            z3::expr temp = context.bool_const(result._Q[i].c_str());
            from.push_back(temp);
            to.push_back(!temp);
            if(result._F_index[result._Q[i]] != 1)
            {
                result._F_index[result._Q[i]] = 1;
                new_F.push_back(result._Q[i]);
            }
            else
                result._F_index[result._Q[i]] = 0;
        }
        result._F = new_F;
        for(int i = 0; i < result._g.size(); i++)
            for(int j = 0; j < result._g[i]._right.size(); j++)
                result._g[i]._right[j] = NNF(!result._g[i]._right[j].substitute(from, to));
        return result;
    }

    ADA intersect(const ADA & a) const
    {
        ADA result = *this;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < a._Q.size(); i++)
        {
            from.push_back(context.bool_const(a._Q[i].c_str()));
            to.push_back(context.bool_const((a._Q[i] + RENAME_SYMBOL).c_str()));
            result._Q.push_back(a._Q[i] + RENAME_SYMBOL);
            declare(a._Q[i] + RENAME_SYMBOL, _BOOL, false);
            result._Q_index[a._Q[i] + RENAME_SYMBOL] = result._Q.size() - 1;
        }
        result._i = "(and " + _i + " " + a._i + RENAME_SYMBOL + ")";
        for(int i = 0; i < a._F.size(); i++)
        {
            result._F.push_back(a._F[i] + RENAME_SYMBOL);
            result._F_index[a._F[i] + RENAME_SYMBOL] = 1;
        }
        for(int i = 0; i < a._g.size(); i++)
        {
            if(result._SIGMA[a._g[i]._symbol] == 0)
            {
                result._SIGMA[a._g[i]._symbol] = result._g.size() + 1;
                result._g.push_back(transition_group(a._g[i]._symbol, this->_Q.size()));
            }
            int pos = result._SIGMA[a._g[i]._symbol] - 1;
            for(int j = 0; j < a._g[i]._right.size(); j++)
            {
                z3::expr temp = a._g[i]._right[j];
                result._g[pos]._right.push_back(temp.substitute(from, to));
            }
        }
        for(int i = 0; i < result._g.size(); i++)
        {
            if(result._g[i]._right.size() != result._Q.size())
                for(int j = 0; j < a._Q.size(); j++)
                    result._g[i]._right.push_back(parse("false", false));
        }
        return result;
    }

    friend std::ostream & operator<<(std::ostream &, const ADA &);

};

std::ostream & operator<<(std::ostream & o, const ADA & a)
{
    o << "STATES" << std::endl;
    for(int i = 0; i < a._Q.size(); i++)
        o << a._Q[i] << " ";
    o << std::endl << std::endl;
    o << "INITIAL" << std::endl;
    o << a._i << std::endl << std::endl;
    o << "FINAL" << std::endl;
    for(int i = 0; i < a._F.size(); i++)
        o << a._F[i] << " ";
    o << std::endl << std::endl;
    o << "SYMBOLS" << std::endl;
    for(int i = 0; i < a._g.size(); i++)
        o << a._g[i]._symbol << " ";
    o << std::endl << std::endl;
    o << "VARIABLES" << std::endl;
    for(int i = 0; i < a._X.size(); i++)
        o << a._X[i] << " ";
    o << std::endl << std::endl;
    o << "TRANSITIONS" << std::endl;
    for(int i = 0; i < a._g.size(); i++)
    {
        for(int j = 0; j < a._g[i]._right.size(); j++)
        {
            if(a._g[i]._right[j].decl().name().str() != "false")
            {
                o << a._g[i]._symbol << " " << a._Q[j] << std::endl << a._g[i]._right[j] << std::endl;
            }
        }
    }
    return o;
}

}

#endif
