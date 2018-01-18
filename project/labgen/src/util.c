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
#include "lds.h"

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

extern void make_lex(FILE* lstream){
  char *lex = "%{\nint wall=1;\nint eof=0;\n%}\n%option noyywrap\n%option yylineno\n%%\nN {return TK_N;}\nS {return TK_S;}\nW {return TK_W;}\nE {return TK_E;}\nNE {return TK_NE;}\nNW {return TK_NW;}\nSE {return TK_SE;}\nSW {return TK_SW;}\n[ \\t\\n]    ;\n.          { wall=0; return TK_ERROR; }\n    { eof=1; return TK_EOF; }\n%%\nvoid yyerror(const char* mess)\n{\n  if ( eof==1 || wall==1 )\n      mess = \"perdu\";\n  fprintf(stderr,\"%s:%d: %%s-near %s-\n\",\n          infname,yylineno,mess,yytext);\n  exit(1);\n}";
  if (-1 == fwrite(lex, strlen(lex), 1,lstream)){
      fprintf(stderr,"Fail with write");
      exit(1);
  }
}

extern char* int_case_to_string(int x){
    char *result = calloc(10,sizeof(char));
    if (x<10){
        sprintf(result,"00%d",x);
    }
    else if (x < 100){
        sprintf(result,"0%d",x);
    }
    else{
        sprintf(result,"%d",x);
    }
    return result;
}
char **dir_tab;

extern void define_direction(){
    dir_tab = calloc(8,sizeof(char *));
    int i;
    for (i = 0 ; i<8 ; i++){
        dir_tab[i] = calloc(5,sizeof(char));
    }
    dir_tab[0] = "TK_N";
    dir_tab[1] = "TK_NE";
    dir_tab[2] = "TK_E";
    dir_tab[3] = "TK_SE";
    dir_tab[4] = "TK_S";
    dir_tab[5] = "TK_SW";
    dir_tab[6] = "TK_W";
    dir_tab[7] = "TK_NW";
}

extern char* direction(Tlds*ds, int x, int y){
    char *line = calloc(400,sizeof(char));
    char *x_str;
    char *y_str;
    x_str = int_case_to_string(x);
    y_str = int_case_to_string(y);
    sprintf(line,"SQ_%s_%s :",x_str,y_str);
    Tsquare ts = ds->squares[x][y];
    if (ts.opt == LDS_OptMD){

    }
    else{
        int i;
        char *tmp;
        tmp = direction_aux(ds,x,y-1,dir_tab[0]);
        if (strcmp(tmp,"") != 0){
            sprintf(line,"%s%s|",line,tmp);
        }
        tmp = direction_aux(ds,x+1,y-1,dir_tab[1]);
        if (strcmp(tmp,"") != 0){
            sprintf(line,"%s%s|",line,tmp);
        }
        tmp = direction_aux(ds,x+1,y,dir_tab[2]);
        if (strcmp(tmp,"") != 0){
            sprintf(line,"%s%s|",line,tmp);
        }
        tmp = direction_aux(ds,x+1,y+1,dir_tab[3]);
        if (strcmp(tmp,"") != 0){
            sprintf(line,"%s%s|",line,tmp);
        }
        tmp = direction_aux(ds,x,y+1,dir_tab[4]);
        if (strcmp(tmp,"") != 0){
            sprintf(line,"%s%s|",line,tmp);
        }
        tmp = direction_aux(ds,x-1,y+1,dir_tab[5]);
        if (strcmp(tmp,"") != 0){
            sprintf(line,"%s%s|",line,tmp);
        }
        tmp = direction_aux(ds,x-1,y,dir_tab[6]);
        if (strcmp(tmp,"") != 0){
            sprintf(line,"%s%s|",line,tmp);
        }
        tmp = direction_aux(ds,x-1,y-1,dir_tab[7]);
        if (strcmp(tmp,"") != 0){
            sprintf(line,"%s%s|",line,tmp);
        }
    }
    int nb = strlen(line);
    if (line[nb-1] == ':'){
      return "";
    }
    else{
        line[nb-1] = ';';
        return line;
    }
}
extern char* direction_aux(Tlds* ds, int new_x, int new_y, char *dir){
    char *x_str;
    char *y_str;
    char *line = calloc(20,sizeof(char));
    if (lds_check_xy(ds,new_x,new_y) == 0){
        Tsquare ts = ds->squares[new_x][new_y];
        if (ts.kind != LDS_WALL){
            if (ts.opt == LDS_OptWH){
                x_str = int_case_to_string(ts.u.whdest.x);
                y_str = int_case_to_string(ts.u.whdest.y);
                sprintf(line,"%s SQ_%s_%s",dir,x_str,y_str);
                free(x_str);
                free(y_str);
            }
            else{
                x_str = int_case_to_string(new_x);
                y_str = int_case_to_string(new_y);
                sprintf(line,"%s SQ_%s_%s",dir,x_str,y_str);
                free(x_str);
                free(y_str);
            }
        }
    }
    return line;
}

extern int lg_gen(Tlds*ds, FILE* lstream, FILE*ystream, Cstr lcfname)
{
    define_direction();
    make_lex(lstream);
    char *deb = "#define STATUS_GOOD 0\n\nstatic const char* infname;\n%}\n%token TK_ERROR TK_EOF\n%token TK_N  TK_S  TK_E  TK_W\n%token TK_NE TK_NW TK_SE TK_SW\n\n%start sortie\n\n%%\n";
    if (-1 == fwrite(deb, strlen(deb), 1,ystream)){
        fprintf(stderr,"Fail with write");
        exit(1);
    }
    Tpoint entree = ds->in;
    char *x_e = int_case_to_string(entree.x);
    char *y_e = int_case_to_string(entree.y);
    char *entree_str = calloc(400,sizeof(char));
    sprintf(entree_str,"sortie : SG_%s_%s { return STATUS_GOOD; };\n",x_e,y_e);
    if (-1 == fwrite(entree_str, strlen(entree_str), 1,ystream)){
        fprintf(stderr,"Fail with write");
        exit(1);
    }
    free(entree_str);
    free(x_e);
    free(y_e);
    int x = ds->dx;
    int y = ds->dy;
    int i,j;
    for (i = 0 ; i < x ; i++){
        for (j = 0 ; j < y ; j++){
                char *dir_str = direction(ds,i,j);
                if (strcmp("",dir_str) == 0){

                }
                else{
                    dir_str = strcat(dir_str,"\n");
                    if (-1 == fwrite(dir_str, strlen(dir_str), 1,ystream)){
                        fprintf(stderr,"Fail with write");
                        exit(1);
                    }
                }
        }
    }
    char *fin_yacc = "%%\n#include \"labres.lex.c\"\nint main(int argc, char*argv[])\n{\nint status;\nif ( argc==1 )\ninfname=strdup(\"stdin\");\nelse if ( argc==2 ) {\nchar *p = strdup(argv[1]);\ninfname = strdup(basename(p));\nfree(p);\nif ( (yyin=fopen(argv[1],\"r\"))==0 ) {\nfprintf(stderr,\"%s: can not open %s file for reading: %s\\n\",\nargv[0],infname,strerror(errno));\nexit(1);\n}\n} else {\nfprintf(stderr,\"%s: to many arguments. usage: %s if\\n\",\nargv[0],argv[0]);\nexit(1);\n}\nstatus = yyparse();\nprintf(\"%s\\n\",\nstatus==STATUS_GOOD ? \"gagné\" : \"perdu\");\nfree((char*)infname);\nexit(0);\n}";
    if (-1 == fwrite(fin_yacc, strlen(fin_yacc), 1,ystream)){
        fprintf(stderr,"Fail with write");
        exit(1);
    }


  return 0;

}

/*======================================================================*/
