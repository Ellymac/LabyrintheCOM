/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: lds.h 433 2017-10-18 20:37:37Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/lds.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_LDS_H
#define FILE_LDS_H
/*======================================================================*/

#include "points.h"
#include "top.h"

/*======================================================================*/
/*= Tuning and Constants                                               =*/

#define LDS_SIZE 100

typedef enum _TsquareKind {
    LDS_FREE=0, LDS_WALL, LDS_IN, LDS_OUT
} TsquareKind;

typedef enum _TsquareOpt {
    LDS_OptNone=0, LDS_OptWH, LDS_OptMD
} TsquareOpt;

/*======================================================================*/
/*= Labyrinth Data Stucture                                            =*/

// parameters of a magic door square
typedef struct _TsqmdEle {
    int    chg;  // 1 if dest was updated
    Twr    wrd;  // the direction of this entry
    Tpoint dest; // where to go when dir iss selected
} TsqmdEle;
typedef struct _Tsqmd {
    TsqmdEle t[LG_WrNb]; // where to go for every wind rose direction
} Tsqmd;

// Labyrinth square
typedef struct _Tsquare {
    TsquareKind kind; // LDS_FREE, LDS_WALL LDS_IN
    TsquareOpt  opt;  /* LDS_OptNone (u not used)
                       * LDS_OptWH   (u.whdes is set)
                       * LDS_OptMD   (u.mdp is set) */
    union {
        Tpoint whdest; // destination  vhen worm hole
        Tsqmd* mdp;    // destinations when macgic doors.
    } u;
#define sq_opt u.whdest
#define sq_whd u.whdest
#define sq_mdp u.mdp
} Tsquare;

// Labyrinth square
struct _Tlds {
    int dx,dy; // labyrinth size (squares[dx][dy]),
               // so squares[dx][0] is out of to the labyrinth
               // and squares[dx-1][0] belongs to the labyrinth.
    Tsquare squares[LDS_SIZE][LDS_SIZE]; // the labyrinth
    Tpoint  in; // the labyrinth input
} ;

/*======================================================================*/
/*= constructor/destructor                                             =*/

extern Tlds * lds_new ();
extern Tlds * lds_new_lds(const Tlds* lds);
extern void   lds_free(Tlds *ds);

/*======================================================================*/
/*= setters                                                            =*/
extern void lds_size_set(Tlds* ds, int dx, int dy);
extern void lds_draw_xy (Tlds* ds, TdrawOpt dopt, int x, int y);
extern void lds_draw_pt (Tlds* ds, TdrawOpt dopt, Tpoint  pt);
extern void lds_draw_pts(Tlds* ds, TdrawOpt dopt, Tpoints*pts);

/*======================================================================*/
/*= check                                                              =*/
/*=                                                                    =*/
/*= These function return 0 if the check is true otherwise 1.          =*/

extern int lds_check_xy      (const Tlds*ds, int x, int y); // in lab.
extern int lds_check_pt      (const Tlds*ds, Tpoint pt); // in lab.
extern int lds_checkborder_pt(const Tlds*ds, Tpoint pt); // in lab. border
// Return in *err the first point of pts that is not in labyrinth
extern int lds_check_pts     (const Tlds*ds, Tpoints*pts, Tpoint*err);

/*======================================================================*/
/*= magic door                                                         =*/

extern Tsqmd* lds_sqmd_new     (Tpoint org);
extern Tsqmd* lds_sqmd_new_sqmd(const Tsqmd* md);
extern void   lds_sqmd_free    (Tsqmd* md);

// set the wrd entry of md to des and set chg to 1.
extern Tsqmd* lds_sqmd_update(Tsqmd*md, Twr wrd, Tpoint des);

/*======================================================================*/
/*= others                                                             =*/
extern void lds_dump(Tlds* ds, FILE* ostream);

/*======================================================================*/
#endif // FILE_LDS_H
/*======================================================================*/
