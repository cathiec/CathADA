#ifndef INTERPOLANT_H
#define INTERPOLANT_H

#include "data.h"
#include "expr.h"

namespace cath {

z3::expr compute_interpolant(z3::expr e1, z3::expr e2)
{
    z3::expr pattern = z3::expr(z3_context, Z3_mk_interpolant(z3_context, e1));
    Z3_model md;
    pattern = (pattern && e2);
    Z3_ast_vector temp;
    Z3_lbool result = Z3_compute_interpolant(z3_context, pattern, 0, &temp, &md);
    if(result == Z3_L_FALSE)
    {
        return z3::expr(z3_context, Z3_ast_vector_get(z3_context, temp, 0));
    }
    return expression("$true").z3();
}

expression trans_into_interpolant(z3::expr e)
{
    std::string temp = Z3_ast_to_string(z3_context, e);
    if(temp == "true")
        return expression("$true");
    else if(temp == "false")
        return expression("$false");
    else
    {
        //std::cout << temp << std::endl;
        int stage = 0;
        for(int i = 0; i < temp.length(); i++)
        {
            if(temp[i] == '(' || temp[i] == ')')
            {
                temp[i] = ' ';
                continue;
            }
            if(temp[i] == ' ')
            {
                switch(stage)
                {
                case 0:
                    temp[i] = '(';
                    stage++;
                    break;
                case 1:
                    temp[i] = ',';
                    stage++;
                    break;
                }
            }
        }
        temp += ')';
        //std::cout << temp << std::endl;
        for(int i = 0; i < temp.length(); i++)
        {
            if(temp[i] >= 'a' && temp[i] <= 'z')
            {
                int j = i + 1;
                while(temp[j] != ',')
                {
                    if(temp[j] >= '0' && temp[j] <= '9')
                        temp[j] = ' ';
                    j++;
                }
            }
        }
        //std::cout << temp << std::endl;
        return expression(temp);
    }
}

}

#endif
