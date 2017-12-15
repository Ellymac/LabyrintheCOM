/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: points.h 433 2017-10-18 20:37:37Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/points.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_POINTS_H
#define FILE_POINTS_H
/*======================================================================*/

#define PTS_SIZE 100 // maximum elements in set

/*======================================================================*/
/*= Type predefinition                                                 =*/

typedef struct _Tpoint   Tpoint;
typedef struct _Tpoints  Tpoints;
typedef struct _Tpoint3  Tpoint3;
typedef struct _Tpoint3s Tpoint3s;

/*======================================================================*/
/*= Data Structure                                                     =*/

// A two-dimensional point.
struct _Tpoint {
    int  x;
    int  y;
};

// A set of two-dimensional points.
struct _Tpoints {
    int    nb;          // number of elements in t
    Tpoint t[PTS_SIZE]; // the elements
};

// A three-dimensional point.
struct _Tpoint3 {
    Tpoint  xy;
    int  z;
};

// A set of three-dimensional points.
struct _Tpoint3s {
    int     nb;          // number of elements in t
    Tpoint3 t[PTS_SIZE]; // the elements
};

/*======================================================================*/
/*= Interface to Tpoint and Tpoints                                    =*/

extern int      pt_cmp(const Tpoint*l, const Tpoint*r);

extern Tpoints* pts_new    ();
extern Tpoints* pts_new_pt (Tpoint pt);
extern Tpoints* pts_new_xy (int x, int y);
extern Tpoints* pts_new_pts(const Tpoints* pts);
extern void     pts_free   (Tpoints* s);

// append an element or all the element of a set
extern void     pts_app_pt (Tpoints* s, Tpoint pt);
extern void     pts_app_xy (Tpoints* s, int x, int y);
extern void     pts_app_pts(Tpoints* s, const Tpoints* pts);

// suppress the point duplication (t[i]==t[i+k] ==> t[i+k] is suppressed)
extern void     pts_uniq(Tpoints* s);

/*======================================================================*/
/*= Interface to Tpoint3 and Tpoint3s                                  =*/

extern int      pt3_cmp2(const Tpoint3*l, const Tpoint3*r);

extern Tpoint3s* pt3s_new     ();
extern Tpoint3s* pt3s_new_p2z (Tpoint pt, int z);
extern Tpoint3s* pt3s_new_xyz (int x, int y, int z);
extern void      pt3s_free    (Tpoint3s* s);

// append an element or all the element of a set
extern void     pt3s_app_pt3 (Tpoint3s* s, Tpoint3 pt3);
extern void     pt3s_app_p2z (Tpoint3s* s, Tpoint pt, int z);
extern void     pt3s_app_xyz (Tpoint3s* s, int x, int y, int z);
extern void     pt3s_app_pts (Tpoint3s* s, const Tpoint3s* pts);

/*======================================================================*/
#endif // FILE_POINTS_H
/*======================================================================*/
