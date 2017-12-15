/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: vars.c 440 2017-10-20 16:34:42Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/vars.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <search.h>

#include "top.h"
#include "vars.h"

/*======================================================================*/

extern Tvar* var_new(char* n, int v)
{
    Tvar* ret = u_malloc( sizeof(*ret) );
    ret->name = n; //strdup( n );
    ret->val  = v;
    return ret;
}

extern void  var_free(Tvar* v)
{
    free(v->name);
    free(v);
}


/*======================================================================*/

extern Tvars* vars_new()
{
    Tvars* ret = u_malloc0( sizeof(*ret) );
    return ret;
}

extern void   vars_free(Tvars* s)
{
    if ( s==0 ) return;
    vars_free( s->subset );
    tdestroy(s->set, (void(*)(void*))var_free);
    free( s );
}

/*======================================================================*/

static int varcmp(const void*l,const void*r)
{
    const Tvar* lv = l;
    const Tvar* rv = r;
    return strcmp(lv->name,rv->name);
}

extern void   vars_chgOrAddCloned(Tvars*s, const char* vn, int v)
{
    Tvar* found = vars_get(s,vn);
    if ( found ) {
        found->val = v;
    } else {
        found = var_new(u_strdup(vn),v);
        Tvar**ret = tsearch(found,&s->set,varcmp);
        if ( ret==0 )
            u_noMoreMem();
    }
}

extern void   vars_chgOrAddEated(Tvars*s, char* vn, int v)
{
    Tvar* found = vars_get(s,vn);
    if ( found ) {
        found->val = v;
        free( vn );
    } else {
        found = var_new(vn,v);
        Tvar**ret = tsearch(found,&s->set,varcmp);
        if ( ret==0 )
            u_noMoreMem();
    }
}

extern Tvar* vars_get(const Tvars*s, const char* vn)
{
    Tvar key; key.name=(char*)vn; 
    Tvar** found = tfind(&key,&s->set,varcmp);
    return found==0 ? 0 : *found;
}

extern Tvar* vars_getR(const Tvars*s, const char* vn)
{
    Tvar* found = vars_get(s,vn);
    if (found==0 && s->subset!=0)
        found = vars_getR(s->subset,vn);
    return found;
}

/*======================================================================*/
