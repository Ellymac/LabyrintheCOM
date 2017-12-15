/*************************************************************************
 *   $cours: lex/yacc
 * $section: projet
 *      $Id: util.c 440 2017-10-20 16:34:42Z ia $
 * $HeadURL: svn://lunix120.ensiie.fr/ia/cours/lex-yacc/src/labyrinthe/labgen/util.c $
 *  $Author: Ivan Auge (Email: auge@ensiie.fr)
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "top.h"

/*======================================================================*/

extern void* u_malloc(int sz)
{
    void* ret = malloc( sz );
    if ( ret==0 ) u_noMoreMem();
    return ret;
}

extern void* u_malloc0(int sz)
{
    void* ret = u_malloc( sz );
    memset(ret,0,sz);
    return ret;
}

extern char* u_strdup (Cstr str)
{
    char* dup = strdup(str);
    if ( dup==0 )
        u_noMoreMem();
    return dup;
}

/*======================================================================*/

extern void u_noMoreMem()
{ 
    fprintf(stderr,"%s:no more memory\n",gl_progname);
    exit(1);
}

extern int   u_error(const char*fmt,...) // like printf, return always 1
{
    char buf[10000];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(buf,fmt,ap);
    va_end(ap);
    fprintf(stderr,"%s: %s\n",gl_progname,buf);
    return 1;
}
extern void u_warning(const char*fmt,...) // like printf, return always 1
{
    if ( gl_warning==0) return;
    char buf[10000];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(buf,fmt,ap);
    va_end(ap);
    fprintf(stderr,"%s: %s\n",gl_progname,buf);
}

/*======================================================================*/
