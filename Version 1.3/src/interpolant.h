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

}

#endif
