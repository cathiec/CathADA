#ifndef alternating_data_automaton_h
#define alternating_data_automaton_h

#include "word.h"
#include "node.h"
#include "timer.h"

#include <fstream>
#include <map>
#include <tuple>

namespace cath{

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

    z3::expr add_time_stamp(const z3::expr & e, int step) const
    {
        z3::expr result = e;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < _X.size(); i++)
        {
            for(int j = 0; j <= 1; j++)
            {
                std::string temp1 = _X[i] + itoa(j), temp2 = _X[i] + itoa(j + step - 1);
                from.push_back(context.int_const(temp1.c_str()));
                to.push_back(context.int_const(temp2.c_str()));
            }
            from.push_back(context.int_const(_X[i].c_str()));
            to.push_back(context.int_const((_X[i] + itoa(step)).c_str()));
        }
        for(int i = 0; i < _Q.size(); i++)
        {
            std::string temp1 = _Q[i], temp2 = _Q[i] + "_" + itoa(step);
            from.push_back(context.bool_const(temp1.c_str()));
            to.push_back(context.bool_const(temp2.c_str()));
        }
        return result.substitute(from, to);
    }

    z3::expr remove_time_stamp(const z3::expr & e, int step) const
    {
        z3::expr result = e;
        z3::expr_vector from(context), to(context);
        for(int i = 0; i < _X.size(); i++)
        {
            std::string temp1 = _X[i] + itoa(step), temp2 = _X[i];
            from.push_back(context.int_const(temp1.c_str()));
            to.push_back(context.int_const(temp2.c_str()));
        }
        for(int i = 0; i < _Q.size(); i++)
        {
            std::string temp1 = _Q[i] + "_" + itoa(step), temp2 = _Q[i];
            from.push_back(context.bool_const(temp1.c_str()));
            to.push_back(context.bool_const(temp2.c_str()));
        }
        return result.substitute(from, to);
    }

    z3::expr find_right(const z3::expr e, int a) const
    {
        int state_index = _Q_index.find(e.decl().name().str())->second;
        return _g[a]._right[state_index];
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

    bool close(node * p_x, const std::vector<node *> & N, bool print = false) const
    {
        for(int i = p_x->_num - 1; i >= 0; i--)
        //for(int i = p_x->_num + 1; i < N.size(); i++)
        {
            node * p_y = N[i];
            if(p_x->is_ancestor_of(p_y))
                continue;
            //std::cout << "? " << p_x->_lambda << " -> " << p_y->_lambda << std::endl;
            if(always_implies(p_x->_lambda, p_y->_lambda))
            {
                if(print)
                {
                    std::cout << "Coverage found: #" << p_x->_num << " is covered by #" << p_y->_num << std::endl;
                }
                p_x->remove_all_covered();
                p_x->_covering.push_back(p_y);
                p_y->_covered.push_back(p_x);
                return true;
            }
        }
        return false;
    }

    bool is_empty(bool print = false, int mode = CONTAINER_MODE) const
    {
        unsigned long max_time = 0;
        std::vector<z3::expr> ALL_STATES;
        for(int i = 0; i < _Q.size(); i++)
            ALL_STATES.push_back(parse(_Q[i]));

        if(print)
            std::cout << std::endl;
        z3::expr init = parse(_i);
        node * p_root = new node;
        p_root->_num = 0;
        p_root->_step = 0;
        p_root->_R = pick_states(init);
        p_root->_lambda = init;
        std::vector<node *> N;
        std::vector<node *> WorkList;
        WorkList.push_back(p_root);
        while(WorkList.size() > 0)
        {
            node * p_n;
            if(mode == 1)   // queue
            {
                p_n = WorkList[0];
                WorkList.erase(WorkList.begin());
            }
            else if(mode == 2)  // stack
            {
                p_n = WorkList[WorkList.size() - 1];
                WorkList.pop_back();
            }
            p_n->_num = N.size();
            N.push_back(p_n);
            if(print)
            {
                std::cout << "#" << p_n->_num << " [" << p_n->_step << "] "
                          << p_n->_lambda
                          << " : R = {";
                for(int j = 0; j < p_n->_R.size(); j++)
                    std::cout << p_n->_R[j] << ",";
                std::cout << "\b} : T = " << p_n->_theta << std::endl;
            }
            // final implication
            z3::expr final = parse("true");
            bool final_set = false;
            for(int i = 0; i < p_n->_R.size(); i++)
            {
                if(_F_index.find(p_n->_R[i].decl().name().str())->second != 1)
                {
                    if(!final_set)
                    {
                        final = z3::implies(add_time_stamp(p_n->_R[i], p_n->_step), parse("false"));
                        final_set = true;
                    }
                    else
                        final = final && z3::implies(add_time_stamp(p_n->_R[i], p_n->_step), parse("false"));
                }
            }
            int back_step = 0;
            // backtrack
            for(node * back = p_n; back != NULL; back = back->_father)
            {
                if(back_step < BACK_STEP && back != p_root)
                {
                    back_step++;
                    continue;
                }
                /*if(print)
                {
                    if(back != p_n)
                        std::cout << "BACK : ";
                    std::cout << "#" << back->_num << " [" << back->_step << "] "
                              << back->_lambda
                              << " : R = {";
                    for(int j = 0; j < back->_R.size(); j++)
                        std::cout << back->_R[j] << ",";
                    std::cout << "\b} : T = " << back->_theta << std::endl;
                }*/
                std::vector<z3::expr> e;
                std::vector<int> symbols;
                e.push_back(final);
                //std::cout << final << std::endl;
                std::vector<node *> temp_n;
                for(node * p = p_n; p != back->_father; p = p->_father)
                {
                    if(p != back)
                        symbols.push_back(p->_symbol);
                    if(p == back)
                    {
                        //std::cout << "BEFORE = " << p->_lambda << std::endl;
                        //std::cout << "AFTER = " << add_time_stamp(p->_lambda, p->_step) << std::endl;
                        e.push_back(add_time_stamp(p->_lambda, p->_step));
                    }
                    else
                        e.push_back(p->_theta);
                    temp_n.push_back(p);
                    //std::cout << "$$$ " << p->_theta << std::endl;
                }
                z3::expr pattern = z3::expr(context, Z3_mk_interpolant(context, true && e[e.size() - 1]));
                for(int i = e.size() - 2; i >= 0; i--)
                {
                    pattern = z3::expr(context, Z3_mk_interpolant(context, pattern && e[i]));
                }
                Z3_model md;
                Z3_ast_vector temp;
                cath::time_point start, end;
                start = cath::now();
                //std::cout << "--------------------" << std::endl;
                Z3_lbool result = Z3_compute_interpolant(context, pattern, 0, &temp, &md);
                end = cath::now();
                unsigned long time_usec = cath::time_diff_usec(start, end);
                if(time_usec > max_time)
                {
                    max_time = time_usec;
                    std::ofstream time_file("time.rst", std::ios::trunc);
                    std::ofstream pattern_file("pattern.rst", std::ios::trunc);
                    time_file << max_time << std::endl;
                    pattern_file << pattern << std::endl;
                    time_file.close();
                    pattern_file.close();
                }
                //printf("\t|\t-> Time used: %ld usec\n", time_usec);
                if(result != Z3_L_FALSE)
                {
                    if(back != p_root)
                    {
                        /*if(print)
                            std::cout << "Good but not enough." << std::endl;*/
                        continue;
                    }
                    word result_word(symbols, _g, _X, z3::model(context, md));
                    if(result_word._w1.size() == 0)
                        std::cout << std::endl << "The automaton accepts empty word." << std::endl << std::endl;
                    else
                        std::cout << std::endl << "The automaton accepts:" << std::endl << result_word << std::endl;
                    for(int i = 0; i < N.size(); i++)
                        delete N[i];
                    return false;
                }
                std::vector<node *> n;
                for(int i = temp_n.size() - 1; i >= 0; i--)
                    n.push_back(temp_n[i]);
                std::vector<z3::expr> I;
                int nb_interpolants = Z3_ast_vector_size(context, temp);
                for(int xxx = 0; xxx < nb_interpolants - 1; xxx++)
                {
                    z3::expr interpolant = z3::expr(context, Z3_ast_vector_get(context, temp, xxx));
                    I.push_back(interpolant);
                }
                /*for(int i = 0; i < I.size(); i++)
                    std::cout << "I[" << i << "] = " << I[i] << std::endl;*/
                // refinement
                bool b = false;
                int step_interpolant = n[0]->_step;
                for(int i = 0 ; i < n.size(); i++)
                {
                    z3::expr pure_interpolant = remove_time_stamp(I[i], step_interpolant++);
                    if(!always_implies(n[i]->_lambda, pure_interpolant))
                    {
                        n[i]->remove_all_covered();
                        if(print)
                            std::cout << "----- Bind " << pure_interpolant << " with #" << n[i]->_num << std::endl;
                        n[i]->_lambda = n[i]->_lambda && pure_interpolant;
                        if(!b)
                            b = close(n[i], N, print);
                    }
                }
                //std::cout << 222 << std::endl;
                break;
            }
            // expand n
            if(!p_n->is_covered())
            {
                for(int i = 0; i < _g.size(); i++)
                {
                    //std::cout << _g[i]._symbol << std::endl;
                    z3::expr new_theta = parse("true");
                    bool theta_set = false;
                    for(int j = 0; j < p_n->_R.size(); j++)
                    {
                        z3::expr temp = find_right(p_n->_R[j], i);
                        z3::expr theta = z3::implies(add_time_stamp(p_n->_R[j], p_n->_step), add_time_stamp(temp, p_n->_step + 1));
                        if(!theta_set)
                        {
                            new_theta = theta;
                            theta_set = true;
                        }
                        else
                            new_theta = new_theta && theta;
                    }
                    /*std::vector<z3::expr> new_R = pick_states(post_R);
                    if(new_R.size() == 0)
                        continue;*/
                    node * p_s = new node;
                    p_s->_step = p_n->_step + 1;
                    p_s->_father = p_n;
                    p_s->_symbol = i;
                    p_s->_theta = new_theta;
                    //p_s->_R = new_R;
                    p_s->_R = ALL_STATES;
                    p_n->_down.push_back(p_s);
                    p_n->_down_symbol.push_back(i);
                    p_n->_down_theta.push_back(new_theta);
                    if(print)
                    {
                        std::cout << '\t' << _g[i]._symbol << " -> "
                                  << " [" << p_s->_step << "] "
                                  << p_s->_lambda << " : R = {";
                        for(int j = 0; j < p_s->_R.size(); j++)
                            std::cout << p_s->_R[j] << ",";
                        std::cout << "\b} : T = " << p_s->_theta << std::endl;
                    }
                    WorkList.push_back(p_s);
                }
            }
            if(print)
                std::cout << std::endl;
        }
        for(int i = 0; i < N.size(); i++)
            delete N[i];
        return true;
    }

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
