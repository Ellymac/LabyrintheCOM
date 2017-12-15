/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: points.c 433 2017-10-18 20:37:37Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/points.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include <stdlib.h>

#include "top.h"
#include "points.h"

/*======================================================================*/

extern int      pt_cmp(const Tpoint*l, const Tpoint*r)
{
    int ret = l->x - r->x;
    return ret ? ret : l->y - r->y;
}

/*======================================================================*/
// constructors & destructor

extern Tpoints* pts_new    ()
{ Tpoints* ret = u_malloc0( sizeof(*ret) ); return ret; }

extern Tpoints* pts_new_pt (Tpoint pt)
{  Tpoints* ret = pts_new(); pts_app_pt(ret,pt); return ret; }

extern Tpoints* pts_new_xy (int x, int y)
{  Tpoints* ret = pts_new(); pts_app_xy(ret,x,y); return ret; }

extern Tpoints* pts_new_pts(const Tpoints* pts)
{  Tpoints* ret = pts_new(); pts_app_pts(ret,pts); return ret; }

extern void     pts_free   (Tpoints* s)
{ if (s) free( s ); }

/*======================================================================*/
// append an element or all the elements of a set

extern void     pts_app_pt (Tpoints* s, Tpoint pt)
{ 
    if ( s->nb==PTS_SIZE )
        u_noMoreMem();
    s->t[s->nb++] = pt;
}

extern void     pts_app_xy (Tpoints* s, int x, int y)
{ Tpoint pt; pt.x=x; pt.y=y; pts_app_pt(s,pt); }

extern void     pts_app_pts(Tpoints* s, const Tpoints* pts)
{ int i; for (i=0 ; i<pts->nb ; i+=1)  pts_app_pt(s,pts->t[i]); }

/*======================================================================*/

// t[i]==t[i+k] ==> t[i+k] is suppressed
extern void     pts_uniq(Tpoints* s)
{
    int i,j,k;
    // very bad algorithm, must be improved
    for (i=0 ; i<s->nb ; i++) {
        Tpoint pti = s->t[i];
        for (j=i+1 ; j<s->nb ; ) {
            if ( pt_cmp(&pti, &s->t[j])==0 ) {
                s->nb -= 1;
                for (k=j ; k<s->nb ; k++) {
                    s->t[k] = s->t[k+1];
                }
            } else
                j+=1;
        }
    }
}

/*======================================================================*/
// pt3s interface

extern int      pt3_cmp2(const Tpoint3*l, const Tpoint3*r);

extern Tpoint3s* pt3s_new     ()
{ Tpoint3s* ret = u_malloc0( sizeof(*ret) ); return ret; }

extern Tpoint3s* pt3s_new_p2z (Tpoint pt, int z)
{  Tpoint3s* ret = pt3s_new(); pt3s_app_p2z(ret,pt,z); return ret; }

extern Tpoint3s* pt3s_new_xyz (int x, int y, int z)
{  Tpoint3s* ret = pt3s_new(); pt3s_app_xyz(ret,x,y,z); return ret; }

extern void      pt3s_free    (Tpoint3s* s)
{ if (s) free( s ); }

// append an element or all the element of a set
extern void     pt3s_app_pt3 (Tpoint3s* s, Tpoint3 pt3)
{ 
    if ( s->nb==PTS_SIZE )
        u_noMoreMem();
    s->t[s->nb++] = pt3;
}

extern void     pt3s_app_p2z (Tpoint3s* s, Tpoint pt, int z)
{ Tpoint3 pt3; pt3.xy=pt; pt3.z=z; pt3s_app_pt3(s,pt3); }

extern void     pt3s_app_xyz (Tpoint3s* s, int x, int y, int z)
{ Tpoint3 pt3; pt3.xy.x=x; pt3.xy.y=y; pt3.z=z; pt3s_app_pt3(s,pt3); }

extern void     pt3s_app_pts (Tpoint3s* s, const Tpoint3s* pts);

/*======================================================================*/
