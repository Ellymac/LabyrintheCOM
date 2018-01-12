%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/points.h"
#include "src/lds.h"

int yylex();

int has_entry = 0;
Tlds *labyrinthe;

Tpoint* init_pt(int x, int y){
    Tpoint* res = malloc(sizeof(struct _Tpoint));
    res->x = x;
    res->y = y;
    return res;
}
void initialize_size(int h, int w){
    if (h < 2 || w < 2){
        printf("Erreur : taille invalide !\n");
        exit(1);
    }
    else{
      /** à réfléchir tout à l'heure */
        labyrinthe = lds_new();
        labyrinthe->dx = w;
        labyrinthe->dy = h;
        int i,j;
        for (i = 0 ; i < h ; i++){
            for (j = 0 ; j < w ; j++){
                Tsquare sq;
                sq.kind = LDS_FREE;
                sq.opt = LDS_OptNone;
                labyrinthe->squares[j][i] = sq;
            }
        }
    }
}

/**
TODO ajouter une condition pour vérifier les entrées/sorties
    des trous de vers
*/
void put_in(int h, int w){
    if (h!=0 && h!=labyrinthe->dy && w!=0 && w!=labyrinthe->dx){
        printf("Erreur : entrée invalide !\n");
        exit(1);
    }
    else if (has_entry == 1){
        printf("Erreur : entrée déjà présente !\n");
        exit(1);
    }
    else{
        if ((labyrinthe->squares[h][w]).kind == LDS_WALL){
            printf("Attention : présence d'un mur (supprimé) \n");
        }
        (labyrinthe->squares[h][w]).kind = LDS_IN;
        Tpoint pt;
        pt.x = w;
        pt.y = h;
        labyrinthe->in = pt;
        has_entry = 1;
    }
}

/**
TODO ajouter une condition pour vérifier les entrées/sorties
    des trous de vers
*/
void put_out(int h, int w){
    if (h!=0 && h!=labyrinthe->dy && w!=0 && w!=labyrinthe->dx){
        printf("Erreur : sortie invalide !\n");
        exit(1);
    }
    else{
        if ((labyrinthe->squares[h][w]).kind == LDS_WALL){
            printf("Attention : présence d'un mur (supprimé) \n");
        }
        (labyrinthe->squares[h][w]).kind = LDS_OUT;
    }
}

void fill_out(Tpoints *suite){
    int n = suite->nb;
    int i;
    for (i = 0; i<n; i++){
        Tpoint a = suite->t[i];
        put_out(a.y,a.x);
    }
}

%}

%token IDENT CNUM DIR
%token SIZE IN OUT SHOW
%token WALL UNWALL TOGGLE
%token PTA PTD R F FOR
%token WH MD
%token PLUSE MINUSE MULTE DIVE MODE
%token ARROW

%union{
    int entier;
    Tpoint *pt;
    Tpoints *suite_pt;
    Tpoint3s *suite_pt_val;
}
%type<entier> CNUM expr
%type<pt> pt
%type<suite_pt> suite_pt
%type<suite_pt_val> suite_pt_val pt_val

%left '+' '-'
%right '*' '/'
%left '%'

%%


labyrinthe
  : suite_instr_vars instr_size suite_instr
;

suite_instr
  : suite_instr instr
  |
;

instr
  : ';'
  | instr_vars
  | IN pt ';'  {put_in($2->y,$2->x);}
  | OUT suite_pt ';' {fill_out($2);}
  | SHOW
  | constr ';'
  | constr PTA suite_pt ';'
  | constr PTD pt suite_pt_val ';'
  | constr R pt pt ';'
  | constr R F pt pt ';'
  | constr FOR for_args pt ';'
  | WH pt ARROW pt pt_arrow
  | MD pt DIR pt dest_list
;

instr_size
  : SIZE expr ';' {initialize_size($2,$2);}
  | SIZE expr ',' expr ';' {initialize_size($4,$2);}
;

instr_vars
  : IDENT '=' expr ';'
  | IDENT PLUSE expr ';'
  | IDENT MINUSE expr ';'
  | IDENT MULTE expr ';'
  | IDENT DIVE expr ';'
  | IDENT MODE expr ';'
;

suite_instr_vars
  : suite_instr_vars instr_vars
  |
;

expr
  : CNUM
  | IDENT
  | expr '*' expr {$$ = $1 * $3;}
  | expr '+' expr {$$ = $1 + $3;}
  | expr '-' expr {$$ = $1 - $3;}
  | expr '/' expr {$$ = $1 / $3;}
  | expr '%' expr {$$ = $1 % $3;}
  | '(' expr ')' {$$ = $2;}
  | '+' expr {$$ = $2;}
  | '-' expr {$$ = -$2;}
;

for_args
  : IDENT IN range
  | IDENT for_args range
;

pt
  : '(' expr ',' expr ')' {$$ = init_pt($2,$4);}
;

range
  :'[' expr ':' expr ']'
  |'[' expr ':' expr '['
  |'[' expr ':' expr ':' expr ']'
  |'[' expr ':' expr ':' expr '['
;

suite_pt
  : suite_pt pt {pts_app_pt($1, *$2); $$ = $1;}
  | pt {$$ = pts_new_pt(*$1);}
;

suite_pt_val
  : suite_pt_val pt_val {
      int i;
      for (i = 0 ; i < $2->nb ; i++){
          pt3s_app_pt3($1,$2->t[i]);
      }
      pt3s_free($2);
      $$ = $1;
  }
  | {}
;
/**
On définit un pt_val comme un Tpoint3s
(avec z==0 si ce n'est pas une étoile et z==1 sinon)
**/
pt_val
  : pt {$$ = pt3s_new_p2z(*$1,0);}
  | pt ':' expr {
      $$ = pt3s_new();
      int i;
      for (i = 0 ; i < $3 ; i++){
          pt3s_app_p2z($$, *$1,0);
      }
  }
  | pt ':' '*' {$$ = pt3s_new_p2z(*$1,1);}
;

pt_arrow
  : pt_arrow ARROW pt
  |
;

dest_list
  : dest_list DIR pt
  |
;

constr
  : WALL
  | UNWALL
  | TOGGLE
;

%%
#include "lex.yy.c"

void yyerror(const char* fmt, ...)
{
    fprintf(stderr,"FATAL (line %d): %s (near %s)\n",yylineno,fmt,yytext);
    exit(1);
}
