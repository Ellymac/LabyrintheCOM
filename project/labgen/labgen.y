%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/top.h"
#include "src/vars.h"
#include "src/points.h"
#include "src/lds.h"

int yylex();

int has_entry = 0;
Tvars* vars;

/* Init vars set */
void init_vars() {
  vars = vars_new();
}

/* Check if var exists in vars */
int check_var(char* var){
    Tvar* found;
    // if var exists
    if ((found = vars_getR(vars, var)) != 0){
        return found->val;
    }
    else{
        printf("Erreur : nom de variable introuvable !\n");
        exit(1);
    }
}

/* Create a Tvar with var and its value, adds the Tvar to vars. TODO cloned avec Cstr ? */
void add_var(char *var, int value){
    // create var
    Tvar* v = var_new(var, value);
    // add var to set of vars
    vars_chgOrAddEated(vars, var, value);
}

Tlds *labyrinthe;

Tpoint* init_pt(int x, int y){
    Tpoint* res = malloc(sizeof(struct _Tpoint));
    res->x = x;
    res->y = y;
    return res;
}

/* Initialize labyrinth's size */
void initialize_size(int x, int y){
    if (x < 2 || y < 2){
        printf("Erreur : taille invalide !\n");
        exit(1);
    }
    else{
      /** à réfléchir tout à l'heure */
        labyrinthe = lds_new();
        labyrinthe->dx = x;
        labyrinthe->dy = y;
        int i,j;
        for (i = 0 ; i < x ; i++){
            for (j = 0 ; j < y ; j++){
                Tsquare sq;
                sq.kind = LDS_FREE;
                sq.opt = LDS_OptNone;
                labyrinthe->squares[i][j] = sq;
            }
        }
    }
}

/*
  Define entry
TODO ajouter une condition pour vérifier les entrées/sorties
    des trous de vers
*/
void put_in(int x, int y){
    if (x!=0 && x!=labyrinthe->dx-1 && y!=0 && y!=labyrinthe->dy-1){
        printf("Erreur : entrée invalide !\n");
        exit(1);
    }
    else if (has_entry == 1){
        printf("Erreur : entrée déjà présente !\n");
        exit(1);
    }
    else{
        if ((labyrinthe->squares[x][y]).kind == LDS_WALL){
            printf("Attention : présence d'un mur (supprimé) \n");
        }
        (labyrinthe->squares[x][y]).kind = LDS_IN;
        Tpoint pt;
        pt.x = x;
        pt.y = y;
        labyrinthe->in = pt;
        has_entry = 1;
    }
}

/*
  Define output
TODO ajouter une condition pour vérifier les entrées/sorties
    des trous de vers
*/
void put_out(int x, int y){
    if (x!=0 && y!=labyrinthe->dx-1 && y!=0 && y!=labyrinthe->dy-1){
        printf("Erreur : sortie invalide !\n");
        exit(1);
    }
    else{
        if ((labyrinthe->squares[x][y]).kind == LDS_WALL){
            printf("Attention : présence d'un mur (supprimé) \n");
        }
        (labyrinthe->squares[x][y]).kind = LDS_OUT;
    }
}

void fill_out(Tpoints *suite){
    int n = suite->nb;
    int i;
    for (i = 0; i<n; i++){
        Tpoint a = suite->t[i];
        put_out(a.x,a.y);
    }
}
void show(){
    lds_dump(labyrinthe,stdout);
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
    Cstr id;
    Tpoint *pt;
    Tpoints *suite_pt;
    Tpoint3s *suite_pt_val;
}
%type<entier> CNUM expr
%type<id> IDENT
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
  | IN pt ';'  {put_in($2->x,$2->y);}
  | OUT suite_pt ';' {fill_out($2);}
  | SHOW {show();}
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
  | SIZE expr ',' expr ';' {initialize_size($2,$4);}
;

instr_vars
  : IDENT '=' expr ';' {
      add_var(u_strdup($1),$3);
  }
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
  | IDENT {
    $$ = check_var(u_strdup($1));
  }
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
