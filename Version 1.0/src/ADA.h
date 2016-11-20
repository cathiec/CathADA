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
        int nbt = 0;
        std::vector<std::string> S;
        while(file >> temp)
        {
            if(temp == "TRANSITIONS")
                break;
            else
            {
                S.push_back(temp);
                mS[temp] = nbt;
                nbt++;
            }
        }
        g = new transition[nbt];
        for(int i = 0; i < nbt; i++)
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

    expression post(std::string before, std::string s) const
    {
        return post(expression(before), s);
    }

    expression post(const expression & before, std::string s) const
    {
        expression result = before;
        post_replace(result, s);
        return result;
    }

};

}

#endif
