#ifndef expr_h
#define expr_h

#include "data.h"
#include <iostream>

namespace cath {

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
        suc = new expression[nb_suc];
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
        type = e.type;
        nb_suc = e.nb_suc;
        str = e.str;
        step = e.step;
        suc = new expression[nb_suc];
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
            return z3_context.int_val(std::atoi(str.c_str()));
        case VAR:
            temp = str;
            if(str[str.length() - 1] != 39)
                temp = temp + itoa(step);
            else
            {
                temp.erase(str.length() - 1);
                temp = temp + itoa(step + 1);
            }
            return z3_context.int_const(temp.c_str());
        default:
            std::cout << "ERROR: the expression of undefined type cannot be transformed into z3 expr." << std::endl;
            exit(1);
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
                result = result + itoa(step);
            else
            {
                result.erase(str.length() - 1);
                result = result + itoa(step + 1);
            }
            return result;
        default:
            std::cout << "ERROR: the expression of undefined type cannot be transformed into z3 expr." << std::endl;
            exit(1);
        }
    }

};

}

#endif
