/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: pdt.h 440 2017-10-20 16:34:42Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/pdt.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_PDT_H
#define FILE_PDT_H
/*======================================================================*/

#include "vars.h"
#include "points.h"
#include "lds.h"

#define PDT_WHSIZE 20

/*======================================================================*/
/*= Parser DaTa                                                        =*/

struct _Tpdt {
    Tvars*   vars;
    Tpoints* in;
    Tpoints* out;
    // worm holes
    int whnb;
    Tpoint whin [PDT_WHSIZE]; // worm hole inputs
    Tpoint whout[PDT_WHSIZE]; // worm hole outputs
    // magic holes
    Tpoints*md; // points that have a magic door.
                // associated data are stored in the lds structure
};

/*======================================================================*/
/*= constructor/destructor                                             =*/

extern Tpdt * pdt_new ();
extern void   pdt_free(Tpdt *ds);

/*======================================================================*/
/*= variable handler                                                   =*/

extern void pdt_var_chgOrAddEated (Tpdt* pdt, char*vn, int v);
extern void pdt_var_chgOrAddCloned(Tpdt* pdt, Cstr vn, int v);

// returns 0 on sucess and puts the value vn variable into *v.
// Otherwise (vn variable does not exist) it returns 1.
extern int  pdt_var_get(Tpdt* pdt, Cstr vn, int*v);

/*======================================================================*/
/*= miscellaneous                                                      =*/

// if the the worm hole src --> dest exists the function return dest.
// Otherwise it returns the NULL pointer.
const Tpoint* pdt_wormhole_dest(const Tpdt*pdt, Tpoint src);
void    pdt_wormhole_add(Tpdt*pdt, Tpoint src, Tpoint dest);

// Returns the md parameters of the pt square.
// If it not yet exists it is created and attached to the pt square.
// So a successive calls with the same pt return the same object.
Tsqmd*  pdt_magicdoor_getcreate(Tpdt*pdt,Tlds*ds,Tpoint pt);

/*======================================================================*/
#endif // FILE_PDT_H
/*======================================================================*/
