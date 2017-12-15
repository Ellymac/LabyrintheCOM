/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: expr.h 440 2017-10-20 16:34:42Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/expr.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_EXPR_H
#define FILE_EXPR_H
/*======================================================================*/

#include "vars.h"

/*======================================================================*/
/*= Data structure                                                     =*/

typedef struct _Texpr  Texpr;

typedef enum   _TexprKind {
    EXPKD_NONE=0,
    EXPKD_NEG ='N',
    EXPKD_VAR ='V', EXPKD_CST  ='C',
    EXPKD_PLUS='+', EXPKD_MINUS='-', EXPKD_TIME='*',
    EXPKD_DIV ='/', EXPKD_MOD  ='%'
} TexprKind;

struct _Texpr {
    TexprKind e_kd;
    Texpr*    e_lc; // left  child
    Texpr*    e_rc; // right child
    union {
        int   cst;
        char* var;  // freed
        struct {
            Texpr*    lc; // left  child
            Texpr*    rc; // right child
        } u;
        Texpr* child;
    } u;
#define e_lc      u.u.lc  // for binary operator kd (left child)
#define e_rc      u.u.rc  // for binary operator kd (right child)
#define e_child   u.child // for unary operator kd  (child)
#define e_cst     u.cst   // for kd=EXPKD_CST
#define e_var     u.var   // for kd=EXPKD_VAR
};

/*======================================================================*/
/*= Constructors/destructor                                            =*/

Texpr* expr_cst      (int cst);
Texpr* expr_varCloned(Cstr var);
Texpr* expr_varEated(char* var);
Texpr* expr_uniOp   (TexprKind kd, Texpr* child);
Texpr* expr_binOp   (TexprKind kd, Texpr* lc, Texpr* rc);

void   expr_free    (Texpr* expr);

/*======================================================================*/

// Returns:
//  0: success and evaluation into *val.
//  1: failure due to an undefined variable that is stored into *uv.
//  2: failure due to zero division or modulo.
int expr_eval(const Texpr* expr, const Tvars* vars, int* val, Cstr*uv);

/*======================================================================*/
#endif // FILE_EXPR_H
/*======================================================================*/
