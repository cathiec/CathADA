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
            int pos_trans = it ->second;
            it = mQ.find(before.str);
            int pos_state = it ->second;
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

    expression post(std::string before, std::string s) const
    {
        return post(expression(before), s);
    }

    expression post(std::string before, const transition & t) const
    {
        return post(expression(before), t);
    }

    ///*
    expression post(const expression & before, std::string s) const
    {
        expression result = before;
        post_replace(result, s);
        return result;
    }

    expression post(const expression & before, const transition & t) const
    {
        expression result = before;
        post_replace(result, t);
        return result;
    }
    //*/

    /*
    expression post(const expression & before, std::string s) const
    {
        expression temp = before;
        if(temp.type == STATE)
        {
            int old_step = temp.step;
            post_replace(temp, s);
            expression result;
            result.type = EXISTS;
            result.nb_suc = X.size() + 1;
            result.suc = new expression[result.nb_suc];
            for(int i = 0; i < X.size(); i++)
            {
                result.suc[i] = expression(X[i]);
                result.suc[i].step = old_step + 1;
            }
            result.suc[X.size()] = temp;
            result.suc[X.size()].step = old_step + 1;
            result.step = old_step + 1;
            return result;
        }
        else if(temp.type == EXISTS)
        {
            int old_step = temp.step;
            post_replace(temp, s);
            expression result;
            result.type = EXISTS;
            result.nb_suc = temp.nb_suc + X.size();
            result.suc = new expression[result.nb_suc];
            for(int i = 0; i < X.size(); i++)
            {
                result.suc[i] = expression(X[i]);
                result.suc[i].step = old_step + 1;
            }
            for(int i = 0; i < temp.nb_suc; i++)
                result.suc[i + X.size()] = temp.suc[i];
            result.step = old_step + 1;
            return result;
        }
        else
        {
            std::cout << "ERROR: impossible to compute the post image." << std::endl;
            exit(1);
        }

    }

    expression post(const expression & before, const transition & t) const
    {
        expression temp = before;
        if(temp.type == STATE)
        {
            int old_step = temp.step;
            post_replace(temp, t);
            expression result;
            result.type = EXISTS;
            result.nb_suc = X.size() + 1;
            result.suc = new expression[result.nb_suc];
            for(int i = 0; i < X.size(); i++)
            {
                result.suc[i] = expression(X[i]);
                result.suc[i].step = old_step + 1;
            }
            result.suc[X.size()] = temp;
            result.suc[X.size()].step = old_step + 1;
            result.step = old_step + 1;
            return result;
        }
        else if(temp.type == EXISTS)
        {
            int old_step = temp.step;
            post_replace(temp, t);
            expression result;
            result.type = EXISTS;
            result.nb_suc = temp.nb_suc + X.size();
            result.suc = new expression[result.nb_suc];
            for(int i = 0; i < X.size(); i++)
            {
                result.suc[i] = expression(X[i]);
                result.suc[i].step = old_step + 1;
            }
            for(int i = 0; i < temp.nb_suc; i++)
                result.suc[i + X.size()] = temp.suc[i];
            result.step = old_step + 1;
            return result;
        }
        else
        {
            std::cout << "ERROR: impossible to compute the post image." << std::endl;
            exit(1);
        }
    }
    */

    bool is_empty(const expression & i) const
    {
        if(i.is_final_step() && i.is_sat())
            return false;
        std::vector<expression> NEXT;
        NEXT.push_back(i);
        std::vector<expression> PROCESSED;
        while(NEXT.size() > 0)
        {
            expression CURRENT = *(NEXT.end() - 1);
            std::cout << CURRENT.to_string() << std::endl;
            NEXT.pop_back();
            PROCESSED.push_back(CURRENT);
            for(int i = 0; i < nb_trans; i++)
            {
                expression POST = post(CURRENT, g[i]);
                std::cout << POST.to_string() << std::endl;

            }
        }
    }

};

}

#endif
