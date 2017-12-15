/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: top.h 440 2017-10-20 16:34:42Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/top.h $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/
#ifndef FILE_TOP_H
#define FILE_TOP_H
/*======================================================================*/

#include <stdio.h>
#include "points.h"

/*======================================================================*/
/*= Tuning                                                             =*/

#define FNAME_SZ 1000   // max byte length of file name

#define ASSERT_ENABLED 1 // 1/0 enables/disables assertion
#define ASSERT_MESS() do { fprintf(stderr, \
        "%s:%d: in %s func: precondition violation (aborting)\n", \
        __FILE__,__LINE__,__func__); exit(1); } while (0)

/*======================================================================*/
/*= Miscellaneous types and utilities                                   =*/

typedef const char* Cstr;

extern void* u_malloc (int sz);
extern void* u_malloc0(int sz);
extern char* u_strdup (Cstr str);
extern void  u_noMoreMem();                 // error messsage + and exit(1)
extern int   u_error  (const char*fmt,...); // like printf, return always 1
extern void  u_warning(const char*fmt,...); // like printf, return immediately
                                            // if gl_warning is null

/*======================================================================*/
/*= Type definition or predefinition                                   =*/

typedef struct _Tpdt Tpdt;   // parser private data
typedef struct _Tlds  Tlds;  // labyrinth data structure

extern Tpdt * pdt_new ();
extern void   pdt_free(Tpdt *pdt);
extern Tlds * lds_new ();
extern void   lds_free(Tlds *ds);

// drawing mode
typedef enum {
    LG_DrawWall, LG_DrawUnwall, LG_DrawToggle
} TdrawOpt;

// wind rose 
typedef enum {
    LG_WrNN=0, LG_WrNE=1, LG_WrEE=2, LG_WrSE=3,
    LG_WrSS=4, LG_WrSW=5, LG_WrWW=6, LG_WrNW=7,
    LG_WrFirst=0, // To perform a work for all the directions:
    LG_WrNb=8,    // Twr d; for (d=LG_WrFirst ; d<LG_WrNb ; d++) work(d);
    LG_WrUU=9     // undefined direction
} Twr;
// Wind Rose Information
typedef struct {
    const char* name;
    Tpoint      move;   // relative move to next square
    // for diagonal move extra square to check for wall.
    //   (-1,-1) if none
    Tpoint      ew1,ew2;
} TwrInfo;
extern TwrInfo wri_t[];

extern Cstr   wr_uname(Twr wrd);
extern Tpoint wr_next(Twr wrd, Tpoint from);

/*======================================================================*/
/*= Global variables                                                   =*/

extern const char* gl_progname;  // base name of running program.
extern const char* gl_infname;   // base name of input file name
extern Tpdt* gl_pdt;             // parser private data
extern Tlds* gl_lds;             // labyrinth data structure
extern int   gl_warning;         // 0:do not print warnings  

/*======================================================================*/
/*= top functions                                                      =*/

/**
 * yyparse fills in gl_pdt and gl_pdt.
 * lg_sem0 updates ds with pdt skipping works that can not been
 * performed.
 * lg_sem updates ds with pdt and performs all semantic checks
 * lg_gen generates parser (.y) and lexical (.l) from ds.
 *
 * All functions return 0 on success and 1 on failure.
 * They print error messages on failure.
**/
extern int   yyparse();
extern void  lg_sem0(Tlds*ds, const Tpdt*pdt);
extern int   lg_sem (Tlds*ds, const Tpdt*pdt);
extern int   lg_gen (Tlds*ds, FILE* lstream, FILE*ystream, Cstr lcfname);

// It prints error messages like printf and then exits with 1 status
extern void  yyerror(const char* fmt, ...);

/*======================================================================*/
#endif // FILE_TOP_H
/*======================================================================*/
