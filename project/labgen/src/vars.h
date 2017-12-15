/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: vars.h 440 2017-10-20 16:34:42Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/vars.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_VARS_H
#define FILE_VARS_H
/*======================================================================*/

/*======================================================================*/
/*= Type predefinition                                                 =*/

typedef struct _Tvar  Tvar;
typedef struct _Tvars Tvars;

/*======================================================================*/
/*= Data Structure                                                     =*/

// An integer variable
struct _Tvar {
    char* name; // variable name
    int   val;  // the value of variable
};

// A hierachical  set of integer variables
struct _Tvars {
    void*  set;    // the root variable set
    Tvars* subset; // the subset
};

/*======================================================================*/
/*= Interface                                                          =*/

extern Tvar* var_new(char* n, int v);
extern void  var_free(Tvar* v);

extern Tvars* vars_new();
extern void   vars_free(Tvars* s);

// vars_chgOrAdd works only on the set (not in subset).
// cloned: vn does not be freed by vars handler
// eated : vn will be freed by vars handler
extern void vars_chgOrAddCloned(Tvars*s, Cstr vn, int v);
extern void vars_chgOrAddEated (Tvars*s, char*vn, int v);

// vars_get  searches in set and not in subset
// vars_getR searches in set and the in subsets.
extern Tvar* vars_get     (const Tvars*s, const char* vn);
extern Tvar* vars_getR    (const Tvars*s, const char* vn);

/*======================================================================*/
#endif // FILE_VARS_H
/*======================================================================*/
