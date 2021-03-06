#ifndef ADA_h
#define ADA_h

#include "transition.h"
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

    void post_replace(expression & before, std::string s) const
    {
        if(before.type != STATE)
        {
            for(int i = 0; i < before.nb_suc; i++)
                post_replace(before.suc[i], s);
        }
        else
        {
            int current_step = before.step;
            std::map<std::string, int>::const_iterator it = mS.find(s);
            int pos_trans = it->second;
            it = mQ.find(before.str);
            int pos_state = it->second;
            before = g[pos_trans].post[pos_state];
            before.set_step(current_step + 1);
        }
    }

    void post_replace(expression & before, const transition & t) const
    {
        if(before.type != STATE)
        {
            for(int i = 0; i < before.nb_suc; i++)
                post_replace(before.suc[i], t);
        }
        else
        {
            int current_step = before.step;
            std::map<std::string, int>::const_iterator it = mQ.find(before.str);
            int pos_state = it->second;
            before = t.post[pos_state];
            before.set_step(current_step + 1);
        }
    }

    ///*
    expression post(const expression & before, std::string s) const
    {
        expression result = before;
        post_replace(result, s);
        result.step++;
        return result;
    }

    expression post(const expression & before, const transition & t) const
    {
        expression result = before;
        post_replace(result, t);
        result.step++;
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
        expression result = post(before, t).DNF();
        if(result.is_final_step())
            return result;
        if(result.type == OR)
        {
            for(int i = 0; i < result.nb_suc; i++)
            {
                std::vector<expression> temp1;
                std::vector<expression> temp2;
                for(int j = 0; j < result.suc[i].nb_suc; j++)
                {
                    if(result.suc[i].suc[j].type == STATE)
                        temp1.push_back(result.suc[i].suc[j]);
                    else
                        temp2.push_back(result.suc[i].suc[j]);
                }
                expression state;
                if(temp1.size() > 0)
                    state = temp1[0];
                int interpolant_step = result.step + 1;
                for(int j = 1; j < temp1.size(); j++)
                    state = state * temp1[j];
                expression psi = expression("$true");
                if(temp2.size() > 0)
                    psi = temp2[0];
                for(int j = 1; j < temp2.size(); j++)
                    psi = psi * temp2[j];
                expression new_psi = expression("$true");
                for(int j = 0; j < interpolant.size(); j++)
                {
                    expression right = interpolant[j];
                    right.set_step(interpolant_step);
                    if(result.always_implies(right))
                        new_psi = new_psi * right;
                }
                if(temp1.size() > 0)
                    result = state * new_psi;
                else
                    result = new_psi;
            }
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
            int interpolant_step = result.step + 1;
            for(int j = 1; j < temp1.size(); j++)
                state = state * temp1[j];
            expression psi = expression("$true");
            if(temp2.size() > 0)
                psi = temp2[0];
            for(int j = 1; j < temp2.size(); j++)
                psi = psi * temp2[j];
            expression new_psi = expression("$true");
            for(int j = 0; j < interpolant.size(); j++)
            {
                expression right = interpolant[j];
                right.set_step(interpolant_step);
                if(result.always_implies(right))
                    new_psi = new_psi * right;
            }
            if(temp1.size() > 0)
                result = state * new_psi;
            else
                result = new_psi;
        }
        else
        {
            int interpolant_step = result.step + 1;
            expression new_psi = expression("$true");
            for(int j = 0; j < interpolant.size(); j++)
            {
                expression right = interpolant[j];
                right.set_step(interpolant_step);
                if(result.always_implies(right))
                    new_psi = new_psi * right;
            }
            result = new_psi;
        }
        result.step = before.step + 1;
        return result;
    }

    bool is_empty(const expression & i, const std::vector<expression> & interpolant) const
    {
        std::vector<expression> NEXT;
        NEXT.push_back(i);
        std::vector<expression> PROCESSED;
        while(NEXT.size() > 0)
        {
            expression CURRENT = *(NEXT.end() - 1);
            std::cout << CURRENT.to_string() << std::endl;
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
                std::cout << "POST: " << POST.to_string() << std::endl;
                if(POST.type == BOOL && POST.str != "true")
                {
                    //std::cout << "JUMP" << std::endl;
                    continue;
                }
                //std::cout << g[i].symbol << "(" << CURRENT.to_string() << ") = " << POST.to_string() << std::endl;
                bool already = false;
                for(int j = 0; j < NEXT.size(); j++)
                {
                    expression temp = NEXT[j];
                    temp.set_step(POST.step);
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
                        temp.set_step(POST.step);
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
                        temp.set_step(POST.step);
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
                        temp.set_step(POST.step);
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

    bool is_empty2(const expression & i) const
    {
        std::vector<expression> NEXT;
        NEXT.push_back(i);
        std::vector<expression> PROCESSED;
        while(NEXT.size() > 0)
        {
            expression CURRENT = *(NEXT.end() - 1);
            //std::cout << CURRENT.to_string() << std::endl;
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
                expression POST = post(CURRENT, g[i]);
                //std::cout << "POST: " << POST.to_string() << std::endl;
                bool already = false;
                for(int j = 0; j < NEXT.size(); j++)
                    if(POST.always_implies(NEXT[j]))
                    {
                        already = true;
                        break;
                    }
                if(already == false)
                {
                    for(int j = 0; j < PROCESSED.size(); j++)
                        if(POST.always_implies(PROCESSED[j]))
                        {
                            already = true;
                            break;
                        }
                }
                if(already == false)
                {
                    std::vector<expression>::iterator it = NEXT.begin();
                    while(NEXT.size() != (it - NEXT.begin()))
                    {
                        if(it->always_implies(POST))
                        {
                            NEXT.erase(it);
                            continue;
                        }
                        it++;
                    }
                    it = PROCESSED.begin();
                    while(PROCESSED.size() != (it - PROCESSED.begin()))
                    {
                        if(it->always_implies(POST))
                        {
                            PROCESSED.erase(it);
                            continue;
                        }
                        it++;
                    }
                    //std::cout << "ADD: " << POST.to_string() << std::endl;
                    NEXT.push_back(POST);
                }
            }
        }
        return true;
    }

};

}

#endif
