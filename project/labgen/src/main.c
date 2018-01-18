/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: main.c 440 2017-10-20 16:34:42Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/main.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <errno.h>

#include "top.h"

/*======================================================================*/
/*= Global variables                                                   =*/

const char* gl_progname;  // base name of running program.
const char* gl_infname;   // base name of input file name
Tlds* gl_lds;             // labyrinth data structure
int   gl_warning;         // 0:do not print warnings

/*======================================================================*/
/*= program argument                                                   =*/

typedef struct _Tparam {
    char* infilename;
    FILE* instream;
    char* outfilename;
} Tparam;

static Tparam main_getParam(int argc, char*argv[])
{
    char *p = strdup(argv[0]);
    gl_progname = strdup(basename(p));
    free(p);

    Tparam param;
    if ( argc==1 ) {
        param.infilename="stdin";
        param.instream=stdin;
        param.outfilename="labres";
        return param;
    }
    if ( argc==2 ) {
        param.infilename=argv[1];
        param.instream=0;
        param.outfilename="labres";
    } else if ( argc==3 ) {
        param.infilename=argv[1];
        param.instream=0;
        param.outfilename=argv[3];
    } else {
        fprintf(stderr,"%s: to many arguments. usage: %s if exec\n",
                gl_progname,gl_progname);
        exit(1);
    }

    if ( (param.instream=fopen(param.infilename,"r"))==0 ) {
        fprintf(stderr,"%s: can not open %s file for reading: %s\n",
                gl_progname,param.infilename,strerror(errno));
        exit(1);
    }

    return param;
}

/*======================================================================*/
/*= main program                                                       =*/

int main(int argc, char*argv[])
{
    Tparam param=main_getParam(argc,argv);

    char* p = strdup(param.infilename);
    gl_infname = strdup(basename(p));
    free(p);

    gl_lds = lds_new();

    // parsing
    init_vars(); // init vars set
    extern FILE* yyin;
    yyin = param.instream;
    if ( yyparse(gl_lds) )
        return 1; // mess. printed by yyparse
    fclose( param.instream );
    // génération of labres lex & yacc codes
    // into lfname and yfname files
    char  lfname[FNAME_SZ],  yfname[FNAME_SZ];
    char  lcfname[FNAME_SZ], ycfname[FNAME_SZ];
    FILE *lstream,         *ystream;
    sprintf(lfname,  "%s.l",     param.outfilename);
    sprintf(yfname,  "%s.y",     param.outfilename);
    sprintf(lcfname, "%s.lex.c", param.outfilename);
    sprintf(ycfname, "%s.tab.c", param.outfilename);
    if ( (lstream=fopen(lfname,"w"))==0 ) {
        fprintf(stderr,"%s: can not open %s file for writing: %s\n",
                gl_progname,lfname,strerror(errno));
        exit(1);
    }
    if ( (ystream=fopen(yfname,"w"))==0 ) {
        fprintf(stderr,"%s: can not open %s file for writing: %s\n",
                gl_progname,yfname,strerror(errno));
        exit(1);
    }
  if ( lg_gen(gl_lds,lstream,ystream,lcfname) )
        return 1; // mess. printed by lg_gen
    fclose(lstream); fclose(ystream);
    lds_free( gl_lds );

    // génération of labres from lfname (lex) and yfname (yacc) files
  /* TODO int status=0;
    char cmd[4*FNAME_SZ+100];
    sprintf(cmd,"bison -o %s %s 2>/dev/null",ycfname,yfname);
    if ( system(cmd)!=0 )
        exit(u_error("parser generation fails: %s",cmd));
    sprintf(cmd,"flex -o %s %s",lcfname,lfname);
    if ( system(cmd)!=0 )
        exit(u_error("parser generation fails: %s",cmd));
    sprintf(cmd,"gcc -g -o %s %s",param.outfilename,ycfname);
    if ( system(cmd)!=0 ) {
        u_error("parser generation fails: %s",cmd);
        u_error("no way from input to output",cmd);
        status = 1;
    }
#if 0
    sprintf(cmd,"rm -f %s %s %s %s",lfname,lcfname,yfname,ycfname);
    if ( system(cmd)!=0 )
        u_error("cleanup of parser generation fails: %s\n",
            gl_progname,cmd);
#endif

    if ( status==0 )
        fprintf(stderr,"%s: %s labyrinth resolver is generated.\n",
            gl_progname,param.outfilename);
    free( (char*)gl_progname );
    free( (char*)gl_infname );
    return status;*/
    return 0;
}

/*======================================================================*/
