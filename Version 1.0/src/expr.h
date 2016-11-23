#ifndef expr_h
#define expr_h

#include "data.h"
#include <iostream>

namespace cath {

int location_size(int n)
{
    int k = 1;
    while(k < n)
        k *= 2;
    return k;
}

bool is_2n(int n)
{
    switch(n)
    {
    case 1:
    case 2:
    case 4:
    case 8:
    case 16:
    case 32:
    case 64:
    case 128:
    case 256:
    case 512:
    case 1024:
    case 2048:
    case 4096: return true;
    default: return false;
    }
}

std::string itoa(int i)
{
    char temp[10];
    std::sprintf(temp, "%d", i);
    return std::string(temp);
}

typedef enum{NOT, FORALL, EXISTS, AND, OR, STATE, BOOL, PLUS, MINUS, TIMES, DIVIDE, EQUAL, NEQUAL, GREATER, GEQUAL, LESS, LEQUAL, INT, VAR, UNDEF} expr_type;

class expression
{

public:

    expr_type type;
    int nb_suc;
    expression * suc;
    std::string str;
    int step;

public:

    expression()
        :type(UNDEF), nb_suc(0), suc(NULL), step(0)
    {}

    expression(const expression & e)
        :type(e.type), nb_suc(e.nb_suc), str(e.str), step(e.step)
    {
        suc = new expression[location_size(nb_suc)];
        for(int i = 0; i < nb_suc; i++)
            suc[i] = e.suc[i];
    }

    expression(std::string s)
    {
        type = UNDEF;
        nb_suc = 0;
        suc = NULL;
        step = 0;
        str = "";
        std::string temp = "";
        char c;
        int i = 0;
        int inside = 0;
        while(i < s.length())
        {
            c = s[i];
            if(c == ' ')
                i++;
            else
                break;
        }
        switch(c)
        {
        case '@':
            type = FORALL;
            suc = new expression[5];
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '%':
            type = EXISTS;
            suc = new expression[5];
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '$':
            type = BOOL;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(c != ' ')
                    str += c;
                i++;
            }
            break;
        case '#':
            type = STATE;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(c != ' ')
                    str += c;
                i++;
            }
            break;
        case '|':
            type = OR;
            suc = new expression[50];
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '&':
            type = AND;
            suc = new expression[50];
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '=':
            type = EQUAL;
            suc = new expression[2];
            nb_suc = 0;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '!':
            if(s[i + 1] == '=')
                type = NEQUAL;
            else
                type = NOT;
            suc = new expression[2];
            nb_suc = 0;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '>':
            if(s[i + 1] == '=')
                type = GEQUAL;
            else
                type = GREATER;
            suc = new expression[2];
            nb_suc = 0;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '<':
            if(s[i + 1] == '=')
                type = LEQUAL;
            else
                type = LESS;
            suc = new expression[2];
            nb_suc = 0;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '+':
            type = PLUS;
            suc = new expression[50];
            nb_suc = 0;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '-':
            type = MINUS;
            suc = new expression[50];
            nb_suc = 0;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '*':
            type = TIMES;
            suc = new expression[50];
            nb_suc = 0;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        case '/':
            type = DIVIDE;
            suc = new expression[50];
            nb_suc = 0;
            i++;
            while(i < s.length())
            {
                c = s[i];
                if(inside == 1 && (c == ',' || c == ')'))
                {
                    suc[nb_suc++] = expression(temp);
                    temp = "";
                    i++;
                    continue;
                }
                if(inside >= 1)
                    temp += c;
                if(c == '(')
                    inside++;
                else if(c == ')')
                    inside--;
                i++;
            }
            break;
        default:
            if(c >= 48 && c <= 57)
                type = INT;
            else
                type = VAR;
            while(i < s.length())
            {
                c = s[i];
                if(c != ' ')
                    str += c;
                i++;
            }
            break;
        }
    }

    ~expression()
    {
        delete[] suc;
    }

    expression & operator=(const expression & e)
    {
        delete[] suc;
        type = e.type;
        nb_suc = e.nb_suc;
        str = e.str;
        step = e.step;
        suc = new expression[location_size(nb_suc)];
        for(int i = 0; i < nb_suc; i++)
            suc[i] = e.suc[i];
        return *this;
    }

    z3::expr z3() const
    {
        z3::expr result = z3_context.bool_val(true);
        std::string temp;
        switch(type)
        {
        case NOT:
            return !(suc[0].z3());
        case FORALL:
            if(nb_suc == 2)
                return z3::forall(suc[0].z3(), suc[1].z3());
            else if(nb_suc == 3)
                return z3::forall(suc[0].z3(), suc[1].z3(), suc[2].z3());
            else if(nb_suc == 4)
                return z3::forall(suc[0].z3(), suc[1].z3(), suc[2].z3(), suc[3].z3());
            else if(nb_suc == 5)
                return z3::forall(suc[0].z3(), suc[1].z3(), suc[2].z3(), suc[3].z3(), suc[4].z3());
        case EXISTS:
            if(nb_suc == 2)
                return z3::exists(suc[0].z3(), suc[1].z3());
            else if(nb_suc == 3)
                return z3::exists(suc[0].z3(), suc[1].z3(), suc[2].z3());
            else if(nb_suc == 4)
                return z3::exists(suc[0].z3(), suc[1].z3(), suc[2].z3(), suc[3].z3());
            else if(nb_suc == 5)
                return z3::exists(suc[0].z3(), suc[1].z3(), suc[2].z3(), suc[3].z3(), suc[4].z3());
        case AND:
            result = suc[0].z3();
            for(int i = 1; i < nb_suc; i++)
                result = result && suc[i].z3();
            return result;
        case OR:
            result = suc[0].z3();
            for(int i = 1; i < nb_suc; i++)
                result = result || suc[i].z3();
            return result;
        case STATE:
            return z3_context.bool_const(str.c_str());
        case BOOL:
            return z3_context.bool_val(str == "true" ? true : false);
        case PLUS:
            result = suc[0].z3();
            for(int i = 1; i < nb_suc; i++)
                result = result + suc[i].z3();
            return result;
        case MINUS:
            result = suc[0].z3();
            for(int i = 1; i < nb_suc; i++)
                result = result - suc[i].z3();
            return result;
        case TIMES:
            result = suc[0].z3();
            for(int i = 1; i < nb_suc; i++)
                result = result * suc[i].z3();
            return result;
        case DIVIDE:
            result = suc[0].z3();
            for(int i = 1; i < nb_suc; i++)
                result = result / suc[i].z3();
            return result;
        case EQUAL:
            return suc[0].z3() == suc[1].z3();
        case NEQUAL:
            return suc[0].z3() != suc[1].z3();
        case GREATER:
            return suc[0].z3() > suc[1].z3();
        case GEQUAL:
            return suc[0].z3() >= suc[1].z3();
        case LESS:
            return suc[0].z3() < suc[1].z3();
        case LEQUAL:
            return suc[0].z3() <= suc[1].z3();
        case INT:
            return z3_context.int_val(str.c_str());
        case VAR:
            temp = str;
            if(str[str.length() - 1] != 39)
                temp = temp + itoa(step - 1);
            else
            {
                temp.erase(str.length() - 1);
                temp = temp + itoa(step);
            }
            return z3_context.int_const(temp.c_str());
        default:
            std::cout << "ERROR: the expression of undefined type cannot be transformed into z3 expr." << std::endl;
            exit(1);
        }
    }

    void set_step(int s)
    {
        if(type == STATE || type == VAR || type == BOOL)
        {
            std::cout << step << " to " << s << std::endl;
            std::cout << "  " << to_string();
            step = s;
            std::cout << " to " << to_string() << std::endl;
        }
        else
        {
            for(int i = 0; i < nb_suc; i++)
                suc[i].set_step(s);
        }
    }

    std::string to_string() const
    {
        std::string result = "";
        switch(type)
        {
        case NOT:
            result += "not(";
            result += suc[0].to_string();
            result += ")";
            return result;
        case FORALL:
            result += "forall ";
            for(int i = 0; i < nb_suc - 1; i++)
            {
                result += suc[i].to_string();
                result += ',';
            }
            result[result.length() - 1] = ':';
            result += ' ';
            result += suc[nb_suc - 1].to_string();
            return result;
        case EXISTS:
            result += "exists ";
            for(int i = 0; i < nb_suc - 1; i++)
            {
                result += suc[i].to_string();
                result += ',';
            }
            result[result.length() - 1] = ':';
            result += ' ';
            result += suc[nb_suc - 1].to_string();
            return result;
        case AND:
            if(suc[0].type != STATE && suc[0].type != VAR && suc[0].type != BOOL && suc[0].type != INT)
                result += '(';
            result += suc[0].to_string();
            if(suc[0].type != STATE && suc[0].type != VAR && suc[0].type != BOOL && suc[0].type != INT)
                result += ')';
            for(int i = 1; i < nb_suc; i++)
            {
                result += " & ";
                if(suc[i].type != STATE && suc[i].type != VAR && suc[i].type != BOOL && suc[i].type != INT)
                    result += '(';
                result += suc[i].to_string();
                if(suc[i].type != STATE && suc[i].type != VAR && suc[i].type != BOOL && suc[i].type != INT)
                    result += ')';
            }
            return result;
        case OR:
            if(suc[0].type != STATE && suc[0].type != VAR && suc[0].type != BOOL && suc[0].type != INT && suc[0].type != AND)
                result += '(';
            result += suc[0].to_string();
            if(suc[0].type != STATE && suc[0].type != VAR && suc[0].type != BOOL && suc[0].type != INT && suc[0].type != AND)
                result += ')';
            for(int i = 1; i < nb_suc; i++)
            {
                result += " | ";
                if(suc[i].type != STATE && suc[i].type != VAR && suc[i].type != BOOL && suc[i].type != INT && suc[i].type != AND)
                    result += '(';
                result += suc[i].to_string();
                if(suc[i].type != STATE && suc[i].type != VAR && suc[i].type != BOOL && suc[i].type != INT && suc[i].type != AND)
                    result += ')';
            }
            return result;
        case STATE:
            return str;
        case BOOL:
            return str;
        case PLUS:
            result += suc[0].to_string();
            for(int i = 1; i < nb_suc; i++)
            {
                result += "+";
                result += suc[i].to_string();
            }
            return result;
        case MINUS:
            if(suc[0].type != VAR && suc[0].type != INT)
                result += '(';
            result += suc[0].to_string();
            if(suc[0].type != VAR && suc[0].type != INT)
                result += ')';
            for(int i = 1; i < nb_suc; i++)
            {
                result += "-";
                if(suc[i].type != VAR && suc[i].type != INT)
                    result += '(';
                result += suc[i].to_string();
                if(suc[i].type != VAR && suc[i].type != INT)
                    result += ')';
            }
            return result;
        case TIMES:
            if(suc[0].type != VAR && suc[0].type != INT)
                result += '(';
            result += suc[0].to_string();
            if(suc[0].type != VAR && suc[0].type != INT)
                result += ')';
            for(int i = 1; i < nb_suc; i++)
            {
                result += "*";
                if(suc[i].type != VAR && suc[i].type != INT)
                    result += '(';
                result += suc[i].to_string();
                if(suc[i].type != VAR && suc[i].type != INT)
                    result += ')';
            }
            return result;
        case DIVIDE:
            if(suc[0].type != VAR && suc[0].type != INT)
                result += '(';
            result += suc[0].to_string();
            if(suc[0].type != VAR && suc[0].type != INT)
                result += ')';
            for(int i = 1; i < nb_suc; i++)
            {
                result += "/";
                if(suc[i].type != VAR && suc[i].type != INT)
                    result += '(';
                result += suc[i].to_string();
                if(suc[i].type != VAR && suc[i].type != INT)
                    result += ')';
            }
            return result;
        case EQUAL:
            result += suc[0].to_string();
            result += "=";
            result += suc[1].to_string();
            return result;
        case NEQUAL:
            result += suc[0].to_string();
            result += "!=";
            result += suc[1].to_string();
            return result;
        case GREATER:
            result += suc[0].to_string();
            result += ">";
            result += suc[1].to_string();
            return result;
        case GEQUAL:
            result += suc[0].to_string();
            result += ">=";
            result += suc[1].to_string();
            return result;
        case LESS:
            result += suc[0].to_string();
            result += "<";
            result += suc[1].to_string();
            return result;
        case LEQUAL:
            result += suc[0].to_string();
            result += "<=";
            result += suc[1].to_string();
            return result;
        case INT:
            return str;
        case VAR:
            result = str;
            if(str[str.length() - 1] != 39)
                result = result + itoa(step - 1);
            else
            {
                result.erase(str.length() - 1);
                result = result + itoa(step);
            }
            return result;
        default:
            std::cout << "ERROR: the expression of undefined type cannot be printed." << std::endl;
            exit(1);
        }
    }

    void reverse_self()
    {
        switch(type)
        {
        case AND:
            type = OR;
            break;
        case OR:
            type = AND;
            break;
        case BOOL:
            if(str == "true")
                str = "false";
            else
                str = "true";
            break;
        case EQUAL:
            type = NEQUAL;
            break;
        case NEQUAL:
            type = EQUAL;
            break;
        case GREATER:
            type = LEQUAL;
            break;
        case GEQUAL:
            type = LESS;
            break;
        case LESS:
            type = GEQUAL;
            break;
        case LEQUAL:
            type = GREATER;
            break;
        default:
            break;
        }
        for(int i = 0; i < nb_suc; i++)
            suc[i].reverse_self();
    }

    expression reverse_copy() const
    {
        expression result = *this;
        result.reverse_self();
        return result;
    }

    bool is_final_step() const
    {
        bool result = (type != STATE);
        if(result == false)
            return false;
        for(int i = 0; i < nb_suc; i++)
        {
            result = result && suc[i].is_final_step();
            if(result == false)
                return false;
        }
        return result;
    }

    bool is_sat() const
    {
        z3::solver s(z3_context);
        s.add(this->z3());
        return (s.check() == z3::sat);
    }

    bool implies(const expression & e) const
    {
        z3::solver s(z3_context);
        z3::expr c = z3::implies(this->z3(), e.z3());
        s.add(!c);
        return (s.check() == z3::unsat);
    }

    expression operator*(const expression & e) const
    {
        //std::cout << "Compute: (" << to_string() << ") * (" << e.to_string() << ")" << std::endl;
        if(type == BOOL && str == "true")
            return e;
        if(e.type == BOOL && e.str == "true")
            return *this;
        if((type == BOOL && str != "true") || (e.type == BOOL && e.str != "true"))
            return expression("$false");
        expression result;
        if(type == AND)
        {
            if(e.type == AND)
            {
                result = *this;
                for(int i = 0; i < e.nb_suc; i++)
                    result = result * e.suc[i];
            }
            else if(e.type == OR)
            {
                result = (*this) * e.suc[0];
                for(int i = 1; i < e.nb_suc; i++)
                    result = result + ((*this) * e.suc[i]);
            }
            else
            {
                result = *this;
                if(is_2n(result.nb_suc))
                {
                    delete[] result.suc;
                    result.suc = new expression[result.nb_suc * 2];
                    for(int i = 0; i < nb_suc; i++)
                        result.suc[i] = suc[i];
                }
                result.suc[result.nb_suc] = e;
                result.nb_suc++;
            }
        }
        else if(type == OR)
        {
            if(e.type == OR)
            {
                result = expression("$false");
                for(int i = 0; i < nb_suc; i++)
                    for(int j = 0; j < e.nb_suc; j++)
                        result = result + suc[i] * e.suc[j];
            }
            else
            {
                result = suc[0] * e;
                for(int i = 1; i < nb_suc; i++)
                    result = result + suc[i] * e;
            }
        }
        else
        {
            if(e.type == AND)
            {
                result = e;
                if(is_2n(result.nb_suc))
                {
                    delete[] result.suc;
                    result.suc = new expression[result.nb_suc * 2];
                    for(int i = 0; i < nb_suc; i++)
                        result.suc[i] = suc[i];
                }
                result.suc[result.nb_suc] = *this;
                result.nb_suc++;
            }
            else if(e.type == OR)
            {
                result = (*this) * e.suc[0];
                for(int i = 1; i < e.nb_suc; i++)
                    result = result + ((*this) * e.suc[i]);
            }
            else
            {
                result.type = AND;
                result.nb_suc = 2;
                result.suc = new expression[2];
                result.suc[0] = *this;
                result.suc[1] = e;
            }
        }
        //std::cout << " *Finished: (" << to_string() << ") * (" << e.to_string() << ")" << std::endl;
        return result;
    }

    expression operator+(const expression & e) const
    {
        //std::cout << "Compute: (" << to_string() << ") + (" << e.to_string() << ")" << std::endl;
        if(type == BOOL && str != "true")
            return e;
        if(e.type == BOOL && e.str != "true")
            return *this;
        if((type == BOOL && str == "true") || (e.type == BOOL && e.str == "true"))
            return expression("$true");
        expression result;
        if(type == AND)
        {
            if(e.type == OR)
            {
                result = *this;
                for(int i = 0; i < e.nb_suc; i++)
                    result = result + e.suc[i];
            }
            else
            {
                result.type = OR;
                result.nb_suc = 2;
                result.suc = new expression[2];
                result.suc[0] = *this;
                result.suc[1] = e;
            }
        }
        else if(type == OR)
        {
            if(e.type == OR)
            {
                result = *this;
                for(int i = 0; i < e.nb_suc; i++)
                    result = result + e.suc[i];
            }
            else
            {
                result = *this;
                if(is_2n(result.nb_suc))
                {
                    delete[] result.suc;
                    result.suc = new expression[result.nb_suc * 2];
                    for(int i = 0; i < nb_suc; i++)
                        result.suc[i] = suc[i];
                }
                result.suc[result.nb_suc] = e;
                result.nb_suc++;
            }
        }
        else
        {
            if(e.type == OR)
            {
                result = e;
                if(is_2n(result.nb_suc))
                {
                    delete[] result.suc;
                    result.suc = new expression[result.nb_suc * 2];
                    for(int i = 0; i < nb_suc; i++)
                        result.suc[i] = suc[i];
                }
                result.suc[result.nb_suc] = *this;
                result.nb_suc++;
            }
            else
            {
                result.type = OR;
                result.nb_suc = 2;
                result.suc = new expression[2];
                result.suc[0] = *this;
                result.suc[1] = e;
            }
        }
        //std::cout << " *Finished: (" << to_string() << ") + (" << e.to_string() << ")" << std::endl;
        return result;
    }

    expression DNF() const
    {
        expression result;
        expression * temp;
        switch(type)
        {
        case OR:
            temp = new expression[nb_suc];
            for(int i = 0; i < nb_suc; i++)
                temp[i] = suc[i].DNF();
            result = temp[0];
            for(int i = 1; i < nb_suc; i++)
                result = result + temp[i];
            result.step = step;
            return result;
        case AND:
            temp = new expression[nb_suc];
            for(int i = 0; i < nb_suc; i++)
                temp[i] = suc[i].DNF();
            result = temp[0];
            for(int i = 1; i < nb_suc; i++)
                result = result * temp[i];
            result.step = step;
            return result;
        default:
            return *this;
        }
    }

    bool always_implies(const expression & e) const
    {
        z3::solver s(z3_context);
        //std::cout << to_string() << " -> " << e.to_string() << std::endl;
        //std::cout << step << " -> " << e.step << std::endl;
        z3::expr test = z3::implies(this->z3(), e.z3());
        s.add(!test);
        if(s.check() == z3::unsat)
        {
            //std::cout << to_string() << " -> " << e.to_string() << std::endl;
            return true;
        }
        else
            return false;
    }

};

}

#endif
