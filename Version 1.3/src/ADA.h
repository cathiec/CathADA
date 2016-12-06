#ifndef ADA_h
#define ADA_h

#include "node.h"
#include "interpolant.h"
#include <fstream>
#include <map>

namespace cath{

class ADA
{

public:

    int nb_trans;
    transition * g;
    std::map<std::string, int> mQ;
    std::map<std::string, int> mS;
    std::vector<std::string> X;

public:

    ADA()
        :nb_trans(0), g(NULL)
    {}

    ADA(std::string file_name)
    {
        std::ifstream file(file_name);
        std::string temp;
        while(file >> temp)
            if(temp == "STATES")
                break;
        int nbs = 0;
        std::vector<std::string> Q;
        while(file >> temp)
        {
            if(temp == "SYMBOLS")
                break;
            else
            {
                Q.push_back(temp);
                mQ[temp] = nbs;
                nbs++;
            }
        }
        std::vector<std::string> S;
        nb_trans = 0;
        while(file >> temp)
        {
            if(temp == "VARIABLES")
                break;
            else
            {
                S.push_back(temp);
                mS[temp] = nb_trans;
                nb_trans++;
            }
        }
        while(file >> temp)
        {
            if(temp == "TRANSITIONS")
                break;
            else
                X.push_back(temp);
        }
        g = new transition[nb_trans];
        for(int i = 0; i < nb_trans; i++)
            g[i] = transition(S[i], nbs, Q);
        while(getline(file, temp, '\n'))
        {
            if(temp.length() > 0)
            {
                int i = 0;
                char c;
                std::string s1 = "";
                while(i < temp.length())
                {
                    c = temp[i];
                    if(c == '(')
                        break;
                    s1 += c;
                    i++;
                }
                i++;
                std::string s2 = "";
                while(i < temp.length())
                {
                    c = temp[i];
                    if(c == ')')
                        break;
                    s2 += c;
                    i++;
                }
                i++;
                while(i < temp.length())
                {
                    c = temp[i];
                    if(c == ':')
                        break;
                    i++;
                }
                i++;
                std::string s3 = "";
                while(i < temp.length())
                {
                    c = temp[i];
                    s3 += c;
                    i++;
                }
                g[mS[s1]].post[mQ[s2]] = expression(s3);
            }
        }
        file.close();
    }

    ~ADA()
    {
        delete[] g;
    }

    int post_replace(expression & before, const transition & t) const
    {
        if(before.type != STATE)
        {
            int max = before.step;
            for(int i = 0; i < before.nb_suc; i++)
            {
                int j = post_replace(before.suc[i], t);
                if(j > max)
                    max = j;
            }
            before.step = max;
            return max;
        }
        else
        {
            std::map<std::string, int>::const_iterator it = mQ.find(before.str);
            int pos_state = it->second;
            int step_before = before.step;
            //std::cout << "before = " << step_before << std::endl;
            before = t.post[pos_state];
            before.set_step(step_before + 1);
            //std::cout << "after = " << before.step << std::endl;
            return before.step;
        }
    }

    ///*
    expression post(const expression & before, std::string s) const
    {
        std::map<std::string, int>::const_iterator it = mS.find(s);
        int pos_trans = it->second;
        return post(before, g[pos_trans]);
    }

    expression post(const expression & before, const transition & t) const
    {
        expression result = before;
        post_replace(result, t);
        return result;
    }
    //*/

    expression abstract_post(const expression & before, std::string s, const std::vector<expression> & interpolant) const
    {
        std::map<std::string, int>::const_iterator it = mS.find(s);
        int pos_trans = it->second;
        return abstract_post(before, g[pos_trans], interpolant);
    }

    expression abstract_post(const expression & before, const transition & t, const std::vector<expression> & interpolant) const
    {
        //std::cout << before.step << ": " << before.to_string() << std::endl;
        expression result = post(before, t).DNF();
        //std::cout << result.step << ": " << result.to_string() << std::endl;
        //std::cout << "CONCRETE(" << result.step << "): " << result.to_string() << std::endl;
        if(result.is_final_step())
        {
            result.step--;
            return result;
        }
        if(result.type == OR)
        {
            expression temp = expression("$false");
            for(int i = 0; i < result.nb_suc; i++)
            {
                std::vector<expression> temp1;
                std::vector<expression> temp2;
                if(result.suc[i].type == STATE)
                    temp1.push_back(result.suc[i]);
                else if(result.suc[i].type == AND || result.suc[i].type == OR)
                {
                    for(int j = 0; j < result.suc[i].nb_suc; j++)
                    {
                        if(result.suc[i].suc[j].type == STATE)
                            temp1.push_back(result.suc[i].suc[j]);
                        else
                            temp2.push_back(result.suc[i].suc[j]);
                    }
                }
                else
                    temp2.push_back(result.suc[i]);
                expression state;
                if(temp1.size() > 0)
                    state = temp1[0];
                for(int j = 1; j < temp1.size(); j++)
                    state = state * temp1[j];
                expression psi = expression("$true");
                if(temp2.size() > 0)
                    psi = temp2[0];
                for(int j = 1; j < temp2.size(); j++)
                    psi = psi * temp2[j];
                int interpolant_step = psi.biggest_x() + 1;
                expression new_psi = expression("$true");
                for(int j = 0; j < interpolant.size(); j++)
                {
                    expression right = interpolant[j];
                    right.set_step(interpolant_step);
                    if(psi.always_implies(right))
                    {
                        right.set_step(result.step + 1);
                        new_psi = new_psi * right;
                    }
                }
                if(temp1.size() > 0)
                    temp = temp + (state * new_psi);
                else
                    temp = temp + new_psi;
            }
            result = temp;
        }
        else if(result.type == AND)
        {
            std::vector<expression> temp1;
            std::vector<expression> temp2;
            for(int j = 0; j < result.nb_suc; j++)
            {
                if(result.suc[j].type == STATE)
                    temp1.push_back(result.suc[j]);
                else
                    temp2.push_back(result.suc[j]);
            }
            expression state;
            if(temp1.size() > 0)
                state = temp1[0];
            for(int j = 1; j < temp1.size(); j++)
                state = state * temp1[j];
            expression psi = expression("$true");
            if(temp2.size() > 0)
                psi = temp2[0];
            for(int j = 1; j < temp2.size(); j++)
                psi = psi * temp2[j];
            int interpolant_step = psi.biggest_x() + 1;
            expression new_psi = expression("$true");
            for(int j = 0; j < interpolant.size(); j++)
            {
                expression right = interpolant[j];
                right.set_step(interpolant_step);
                if(psi.always_implies(right))
                {
                    right.set_step(result.step + 1);
                    new_psi = new_psi * right;
                }
            }
            if(temp1.size() > 0)
                result = state * new_psi;
            else
                result = new_psi;
        }
        else if(result.type != STATE)
        {
            int interpolant_step = result.biggest_x() + 1;
            expression new_psi = expression("$true");
            for(int j = 0; j < interpolant.size(); j++)
            {
                expression right = interpolant[j];
                right.set_step(interpolant_step);
                if(result.always_implies(right))
                {
                    right.set_step(result.step + 1);
                    new_psi = new_psi * right;
                }
            }
            result = new_psi;
        }
        result.step = before.step + 1;
        //std::cout << "ABSTRACT(" << result.step << "): " << result.to_string() << std::endl;
        return result;
    }

    bool is_empty(const expression & init) const
    {
        node history(init);
        std::vector<node *> NEXT;
        NEXT.push_back(&history);
        std::vector<expression> INTERPOLANT;
        INTERPOLANT.push_back(expression("$false"));
        while(NEXT.size() > 0)
        {
            node * CURRENT = *(NEXT.end() - 1);
            std::cout << CURRENT->exp.to_string() << std::endl;
            NEXT.pop_back();
            if(CURRENT->exp.is_final_step())
            {
                z3::solver s(z3_context);
                s.add(CURRENT->exp.z3());
                if(s.check() == z3::sat)
                {
                    std::cout << "FIND: " << CURRENT->exp.to_string() << std::endl;
                    std::vector<int> word;
                    word.push_back(CURRENT->symbol);
                    bool found = false;
                    for(node * back_track = CURRENT->up; back_track != NULL && found == false; back_track = back_track->up)
                    {
                        std::cout << "BACK: " << std::endl;
                        expression CURRENT_EXP = back_track->exp;
                        for(int i = word.size() - 1; i >= 0; i--)
                        {
                            std::cout << CURRENT_EXP.to_string() << std::endl;
                            CURRENT_EXP = post(CURRENT_EXP, g[word[i]]);
                            std::cout << "CON POST with " << g[word[i]].symbol << ": " << CURRENT_EXP.to_string() << std::endl;
                            if(CURRENT_EXP.is_final_step())
                            {
                                z3::solver s2(z3_context);
                                s2.add(CURRENT_EXP.z3());
                                if(s2.check() != z3::sat)
                                {
                                    std::cout << "NOT REAL" << std::endl;
                                    found = true;

                                    ;

                                    break;
                                }
                                else if(back_track->exp.str == init.str)
                                {
                                    std::cout << "REAL" << std::endl;
                                    std::cout << s.get_model() << std::endl;
                                    return false;
                                }
                                else
                                    std::cout << "SAT BUT NOT ENOUGH" << std::endl;
                            }
                        }
                        word.push_back(back_track->symbol);
                    }
                }

            }
            for(int i = 0; i < nb_trans; i++)
            {
                expression POST = abstract_post(CURRENT->exp, g[i], INTERPOLANT);
                if(POST.type == BOOL && POST.str != "true")
                    continue;
                if(history.find_cover(POST) == false)
                {
                    CURRENT->down[CURRENT->nb_down] = new node(POST);
                    CURRENT->down[CURRENT->nb_down]->up = CURRENT;
                    CURRENT->down[CURRENT->nb_down]->symbol = i;
                    std::cout << "ABS POST with " << g[i].symbol << ": " << CURRENT->down[CURRENT->nb_down]->exp.to_string() << std::endl;
                    NEXT.push_back(CURRENT->down[CURRENT->nb_down]);
                    CURRENT->nb_down = CURRENT->nb_down + 1;
                }
            }
        }
        return true;
    }

    bool is_empty2(const expression & i, const std::vector<expression> & interpolant) const
    {
        std::vector<expression> NEXT;
        NEXT.push_back(i);
        std::vector<expression> PROCESSED;
        while(NEXT.size() > 0)
        {
            expression CURRENT = *(NEXT.end() - 1);
            std::cout << "(" << CURRENT.step << "): " << CURRENT.to_string() << std::endl;
            if(CURRENT.is_final_step())
            {
                z3::solver s(z3_context);
                s.add(CURRENT.z3());
                if(s.check() == z3::sat)
                {
                    std::cout << "FIND: " << CURRENT.to_string() << std::endl;
                    std::cout << s.get_model() << std::endl;
                    return false;
                }
            }
            NEXT.pop_back();
            PROCESSED.push_back(CURRENT);
            for(int i = 0; i < nb_trans; i++)
            {
                expression POST = abstract_post(CURRENT, g[i], interpolant);
                std::cout << "POST(" << POST.step << "): " << POST.to_string() << std::endl;
                if(POST.type == BOOL && POST.str != "true")
                    continue;
                bool already = false;
                for(int j = 0; j < NEXT.size(); j++)
                {
                    expression temp = NEXT[j];
                    temp.up_step(POST.step - temp.step);
                    if(POST.always_implies(temp))
                    {
                        already = true;
                        break;
                    }
                }
                if(already == false)
                {
                    for(int j = 0; j < PROCESSED.size(); j++)
                    {
                        expression temp = PROCESSED[j];
                        temp.up_step(POST.step - temp.step);
                        if(POST.always_implies(temp))
                        {
                            already = true;
                            break;
                        }
                    }
                }
                if(already == false)
                {
                    std::vector<expression>::iterator it = NEXT.begin();
                    while(NEXT.size() != (it - NEXT.begin()))
                    {
                        expression temp = *it;
                        temp.up_step(POST.step - temp.step);
                        if(temp.always_implies(POST))
                        {
                            NEXT.erase(it);
                            continue;
                        }
                        it++;
                    }
                    it = PROCESSED.begin();
                    while(PROCESSED.size() != (it - PROCESSED.begin()))
                    {
                        expression temp = *it;
                        temp.up_step(POST.step - temp.step);
                        if(temp.always_implies(POST))
                        {
                            PROCESSED.erase(it);
                            continue;
                        }
                        it++;
                    }
                    NEXT.push_back(POST);
                }
            }
        }
        return true;
    }

    bool is_empty3(const expression & i) const
    {
        std::vector<expression> NEXT;
        NEXT.push_back(i);
        std::vector<expression> PROCESSED;
        while(NEXT.size() > 0)
        {
            expression CURRENT = *(NEXT.end() - 1);
            std::cout << "(" << CURRENT.step << "): " << CURRENT.to_string() << std::endl;
            if(CURRENT.is_final_step())
            {
                z3::solver s(z3_context);
                s.add(CURRENT.z3());
                if(s.check() == z3::sat)
                {
                    std::cout << "FIND: " << CURRENT.to_string() << std::endl;
                    std::cout << s.get_model() << std::endl;
                    return false;
                }
            }
            NEXT.pop_back();
            PROCESSED.push_back(CURRENT);
            for(int i = 0; i < nb_trans; i++)
            {
                expression POST = post(CURRENT, g[i]).DNF();
                if(POST.is_final_step())
                    POST.step--;
                expression temp1 = POST;
                std::cout << "POST(" << POST.step << "): " << POST.to_string() << std::endl;
                bool already = false;
                for(int j = 0; j < NEXT.size(); j++)
                {
                    expression temp2 = NEXT[j];
                    if(temp1.step > temp2.step)
                        temp2.up_step(temp1.step - temp2.step);
                    else if(temp1.step < temp2.step)
                        temp1.up_step(temp2.step - temp1.step);
                    if(temp1.always_implies(temp2))
                    {
                        already = true;
                        break;
                    }
                }
                if(already == false)
                {
                    for(int j = 0; j < PROCESSED.size(); j++)
                    {
                        expression temp2 = PROCESSED[j];
                        if(temp1.step > temp2.step)
                            temp2.up_step(temp1.step - temp2.step);
                        else if(temp1.step < temp2.step)
                            temp1.up_step(temp2.step - temp1.step);
                        if(temp1.always_implies(temp2))
                        {
                            already = true;
                            break;
                        }
                    }
                }
                if(already == false)
                {
                    std::vector<expression>::iterator it = NEXT.begin();
                    while(NEXT.size() != (it - NEXT.begin()))
                    {
                        expression temp2 = *it;
                        if(temp1.step > temp2.step)
                            temp2.up_step(temp1.step - temp2.step);
                        else if(temp1.step < temp2.step)
                            temp1.up_step(temp2.step - temp1.step);
                        if(temp2.always_implies(temp1))
                        {
                            NEXT.erase(it);
                            continue;
                        }
                        it++;
                    }
                    it = PROCESSED.begin();
                    while(PROCESSED.size() != (it - PROCESSED.begin()))
                    {
                        expression temp2 = *it;
                        if(temp1.step > temp2.step)
                            temp2.up_step(temp1.step - temp2.step);
                        else if(temp1.step < temp2.step)
                            temp1.up_step(temp2.step - temp1.step);
                        if(temp2.always_implies(temp1))
                        {
                            PROCESSED.erase(it);
                            continue;
                        }
                        it++;
                    }
                    NEXT.push_back(POST);
                }
            }
        }
        return true;
    }

};

}

#endif
