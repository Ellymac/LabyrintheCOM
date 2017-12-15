/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: pdt.c 440 2017-10-20 16:34:42Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/pdt.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include <stdlib.h>

#include "top.h"
#include "pdt.h"

/*======================================================================*/
/*= constructor/destructor                                             =*/

extern Tpdt * pdt_new ()
{
    Tpdt* pdt = u_malloc0( sizeof(*pdt) );
    pdt->vars = vars_new();
    pdt->out  = pts_new();
    pdt->md   = pts_new();
    return pdt;
}

extern void   pdt_free(Tpdt *pdt)
{
    vars_free( pdt->vars );
    pts_free( pdt->in );
    pts_free( pdt->out );
    pts_free( pdt->md );
    free( pdt );
}

/*======================================================================*/
/*= variable handler                                                   =*/

extern void pdt_var_chgOrAddEated(Tpdt* pdt, char*vn, int v)
{ vars_chgOrAddEated(pdt->vars,vn,v); }

extern void pdt_var_chgOrAddCloned(Tpdt* pdt, Cstr vn, int v)
{ vars_chgOrAddCloned(pdt->vars,vn,v); }

// returns 0 on sucess and puts the value vn variable into *v.
// Otherwise (vn variable does not exist) it returns 1.
extern int  pdt_var_get(Tpdt* pdt, Cstr vn, int*v)
{
    Tvar* found = vars_get(pdt->vars,vn);
    if ( found!=0 ) {
        *v = found->val;
    } 
    return found==0;
}

/*======================================================================*/
/*= miscellaneous                                                      =*/

// if the the worm hole src --> dest exists the function return dest.
// Otherwise it returns the NULL pointer.
const Tpoint* pdt_wormhole_dest(const Tpdt*pdt, Tpoint src)
{
    int i;
    for(i=0 ; i<pdt->whnb ; i+=1) {
        if ( pt_cmp(&pdt->whin[i],&src)==0 )
            return &pdt->whout[i];
    }
    return NULL;
}

void    pdt_wormhole_add(Tpdt*pdt, Tpoint src, Tpoint dest)
{
    pdt->whin [pdt->whnb] = src;
    pdt->whout[pdt->whnb] = dest;
    pdt->whnb            += 1;
}

// Returns the md parameters of the pt square.
// If it not yet exists it is created and attached to the pt square.
// So a successive calls with the same pt return the same object.
Tsqmd*  pdt_magicdoor_getcreate(Tpdt*pdt,Tlds*ds,Tpoint pt)
{
    int i;
    for(i=0 ; i<pdt->md->nb ; i+=1) {
        Tpoint curr = pdt->md->t[i];
        if ( pt_cmp(&curr,&pt)==0 )
            return ds->squares[curr.x][curr.y].sq_mdp;
    }
    ds->squares[pt.x][pt.y].opt    = LDS_OptMD;
    ds->squares[pt.x][pt.y].sq_mdp = lds_sqmd_new(pt);
    pts_app_pt( pdt->md, pt);
    return ds->squares[pt.x][pt.y].sq_mdp;
}

/*======================================================================*/
