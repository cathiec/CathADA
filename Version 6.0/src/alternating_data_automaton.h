#ifndef alternating_data_automaton_h
#define alternating_data_automaton_h

#include "tree_node.h"
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

    bool is_sat(const z3::expr & e, z3::solver * p_solver = NULL) const
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
            if(p_solver != NULL)
                *p_solver = s;
            return true;
        }
        else
            return false;
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

    std::vector<DAG_path> all_pathes(DAG_node * N, const std::vector<std::string> & path) const
    {
        std::vector<DAG_path> result;
        //std::cout << "CURRENT " << path.size() << " : " << N->_e << std::endl;
        std::string last_symbol = path[path.size() - 1];
        std::vector<std::string> copy_path = path;
        copy_path.pop_back();
        for(int i = 0; i < N->_f_symbol.size(); i++)
        {
            if(N->_f_symbol[i] == last_symbol)
            {
                //std::cout << "$ " << N->_father[i]->_e << std::endl;
                if(path.size() > 1)
                {
                    std::vector<DAG_path> part_result = all_pathes(N->_father[i], copy_path);
                    for(int j = 0; j < part_result.size(); j++)
                    {
                        part_result[j].add(N->_f_phi[i], N);
                        result.push_back(part_result[j]);
                    }
                }
                else
                {
                    DAG_path temp;
                    temp.add(N->_f_phi[i], N);
                    result.push_back(temp);
                }
            }
        }
        return result;
    }

    std::vector<DAG_path> all_pathes(tree_node * CURRENT) const
    {
        std::vector<DAG_path> result;
        for(int i = 0; i < CURRENT->_ele.size(); i++)
        {
            if(is_sat(CURRENT->_ele[i]->_e))
            {
                //std::cout << CURRENT->_ele[i]->_e << std::endl;
                std::vector<DAG_path> part_result = all_pathes(CURRENT->_ele[i], CURRENT->_path);
                for(int j = 0; j < part_result.size(); j++)
                {
                    //part_result[j].print();
                    result.push_back(part_result[j]);
                }
            }
        }
        return result;
    }

    bool recompute(tree_node * CURRENT, z3::solver * p_solver = NULL, bool print = false) const
    {
        if(CURRENT->_father == NULL)
        {
            std::cout << CURRENT->_ele[0]->_e << std::endl;
            return is_sat(CURRENT->_ele[0]->_e, p_solver);
        }
        std::vector<DAG_path> PATHES = all_pathes(CURRENT);
        for(int i = 0; i < PATHES.size(); i++)
        {
            z3::expr temp = parse("true");
            for(int j = 0; j < PATHES[i]._phi.size(); j++)
            {
                temp = temp && PATHES[i]._phi[j];
            }
            if(is_sat(temp, p_solver))
            {
                return true;
            }
            else
            {
                //std::cout << temp << std::endl;
                z3::expr latest_interpolant = parse("true");
                for(int j = 0; j < PATHES[i]._phi.size() - 1; j++)
                {
                    //std::cout << latest_interpolant << std::endl;
                    //std::cout << PATHES[i]._phi[j] << std::endl;
                    z3::expr e1 = latest_interpolant && PATHES[i]._phi[j];
                    z3::expr e2 = PATHES[i]._phi[j + 1];
                    std::cout << e1 << std::endl;
                    std::cout << e2 << std::endl;
                    for(int k = j + 2; k < PATHES[i]._phi.size(); k++)
                        e2 = e2 && PATHES[i]._phi[k];
                    std::cout << e1 << std::endl;
                    std::cout << e2 << std::endl;
                    latest_interpolant = compute_interpolant(e1, e2);
                    if(print)
                    {
                        z3::expr pure_interpolant = set_step(latest_interpolant, j + 1, 0);
                        if(always_implies(parse("true"), PATHES[i]._node[j]->_interpolants))
                        {
                            PATHES[i]._node[j]->_interpolants = pure_interpolant;
                        }
                        else
                            PATHES[i]._node[j]->_interpolants = PATHES[i]._node[j]->_interpolants || pure_interpolant;
                        std::cout << "Bind " << latest_interpolant << " with " << PATHES[i]._node[j]->_e << std::endl;
                        //std::cout << "\tPURE " << pure_interpolant << std::endl;
                    }
                }
                if(always_implies(parse("true"), PATHES[i]._node[PATHES[i]._phi.size() - 1]->_interpolants))
                {
                    PATHES[i]._node[PATHES[i]._phi.size() - 1]->_interpolants = parse("false");
                    std::cout << "Bind false with " << PATHES[i]._node[PATHES[i]._phi.size() - 1]->_e << std::endl;
                }
            }
        }
        return false;
    }

    bool is_empty(bool print = false) const
    {
        z3::expr init = parse(_i);
        tree_node tree_root;
        DAG_node DAG_root(init);
        tree_root._ele.push_back(&DAG_root);
        std::vector<tree_node *> NEXT;
        NEXT.push_back(&tree_root);
        z3::solver ce_solver(context);
        std::cout << std::endl;
        while(NEXT.size() > 0)
        {
            tree_node * CURRENT = NEXT[NEXT.size() - 1];
            if(CURRENT->_covered)
            {
                NEXT.pop_back();
                continue;
            }
            z3::expr e = CURRENT->z3();
            if(print)
            {
                std::cout << "-";
                for(int i = 0; i < CURRENT->_path.size(); i++)
                    std::cout << CURRENT->_path[i] << "-";
                std::cout << " , " << e << std::endl;
            }
            NEXT.pop_back();
            if(is_sat(e))
            {
                if(print)
                {
                    std::cout << "Example seems good." << std::endl;
                    std::cout << "Recompute the concrete post." << std::endl;
                }
                if(recompute(CURRENT, &ce_solver, print))
                {
                    if(print)
                    {
                        std::cout << "Example is surely good." << std::endl;
                        word result_word(CURRENT->_path, _g, _X, ce_solver.get_model());
                        std::cout << std::endl << "The automaton accepts:" << std::endl << result_word << std::endl;
                    }
                    return false;
                }
            }
            else
            {
                for(int i = 0; i < _g.size(); i++)
                {
                    //std::cout << "Symbol: " << _g[i]._symbol << std::endl;
                    tree_node * ONE_POST = new tree_node;
                    ONE_POST->_father = CURRENT;
                    ONE_POST->_path = CURRENT->_path;
                    ONE_POST->_path.push_back(_g[i]._symbol);
                    std::vector<DAG_node *> ALREADY;
                    for(int j = 0; j < CURRENT->_ele.size(); j++)
                    {
                        //std::cout << "\t# We are currently in " << CURRENT->_ele[j]->_e << std::endl;
                        z3::expr PART_POST = concrete_post(CURRENT->_ele[j]->_e, _g[i], CURRENT->_path.size());
                        //std::cout << "\t$ PART POST = " << PART_POST << std::endl;
                        if(always_implies(PART_POST, parse("false")))
                        {
                            continue;
                        }
                        std::vector<z3::expr> INTO_DNF = DNF_array(PART_POST);
                        for(int k = 0; k < INTO_DNF.size(); k++)
                        {
                            CURRENT->_ele[j]->_symbol.push_back(_g[i]._symbol);
                            z3::expr psi = PSI(INTO_DNF[k]);
                            CURRENT->_ele[j]->_phi.push_back(psi);
                            z3::expr main = MAIN(INTO_DNF[k]);
                            std::cout << "\t\tINTO DNF PART: " << INTO_DNF[k] << std::endl;
                            std::cout << "\t\tINTO DNF PART MAIN: " << main << std::endl;
                            std::cout << "\t\tINTO DNF PART PSI: " << PSI(INTO_DNF[k]) << std::endl;
                            int already = -1;
                            for(int m = 0; m < ALREADY.size(); m++)
                                if(equal(ALREADY[m]->_e, main))
                                {
                                    already = m;
                                    break;
                                }
                            if(already == -1)
                            {
                                DAG_node * temp = new DAG_node(main);
                                temp->_f_symbol.push_back(_g[i]._symbol);
                                temp->_f_phi.push_back(psi);
                                temp->_father.push_back(CURRENT->_ele[j]);
                                CURRENT->_ele[j]->_succ.push_back(temp);
                                ONE_POST->_ele.push_back(temp);
                                ALREADY.push_back(temp);
                            }
                            else
                            {
                                //std::cout << "\t\t" << ALREADY[already]->_e << " ALREADY" << std::endl;
                                ALREADY[already]->_f_symbol.push_back(_g[i]._symbol);
                                ALREADY[already]->_f_phi.push_back(psi);
                                ALREADY[already]->_father.push_back(CURRENT->_ele[j]);
                                CURRENT->_ele[j]->_succ.push_back(ALREADY[already]);
                            }
                        }
                    }
                    if(!always_implies(ONE_POST->z3(), parse("false")))
                    {
                        //std::cout << "POST(" << _g[i]._symbol << ") = " << ONE_POST->z3() << std::endl;
                        CURRENT->_down.push_back(ONE_POST);
                        NEXT.push_back(ONE_POST);
                    }
                }
            }
            if(print)
                std::cout << std::endl;
        }
        return true;
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
