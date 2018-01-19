%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/top.h"
#include "src/expr.h"
#include "src/vars.h"
#include "src/points.h"
#include "src/lds.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

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

/* Init a Tpoint */
Tpoint* init_pt(int x, int y){
    Tpoint* res = malloc(sizeof(struct _Tpoint));
    res->x = x;
    res->y = y;
    return res;
}

/* Initialize labyrinth's size */
void initialize_size(Tlds *labyrinthe, int x, int y){
    if (x < 2 || y < 2){
        printf("Erreur : taille invalide !\n");
        exit(1);
    }
    else{
      /** à réfléchir tout à l'heure */
        labyrinthe->dx = x+1;
        labyrinthe->dy = y+1;
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
void put_in(Tlds *labyrinthe, int x, int y){
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
void put_out(Tlds *labyrinthe, int x, int y){
    if (x!=0 && x!=labyrinthe->dx-1 && y!=0 && y!=labyrinthe->dy-1){
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

/* Fill multiple outputs */
void fill_out(Tlds *labyrinthe, Tpoints *suite){
    int n = suite->nb;
    int i;
    for (i = 0; i<n; i++){
        Tpoint a = suite->t[i];
        put_out(labyrinthe, a.x,a.y);
    }
}

/* Computes operations on Tvar */
void operation(char* var, int value, int opNb) {
  Tvar* getTvar = vars_getR(vars, var);
  int newVal = getTvar->val;
  printf("1 : %s = %d \n", getTvar->name, newVal);

  switch(opNb) {
    case 1 :
      printf("--> %s = %s + %d \n", getTvar->name, getTvar->name, value);
      newVal += value;

      break;
    case 2 :
      newVal -= value;
      printf("--> %s = %s - %d \n", getTvar->name, getTvar->name, value);

      break;
    case 3 :
      newVal /= value;
      printf("--> %s = %s / %d \n", getTvar->name, getTvar->name, value);

      break;
    case 4 :
      newVal *= value;
      printf("--> %s = %s * %d \n", getTvar->name, getTvar->name, value);

      break;
    case 5 :
      newVal %= value;
      printf("--> %s = %s %% %d \n", getTvar->name, getTvar->name, value);

      break;
  }

  printf("2 : %s = %d \n\n", getTvar->name, newVal);
  vars_chgOrAddEated(vars, var, newVal);
}

/* Defines a magic door */
void define_md(Tlds* labyrinthe, Tpoint src, Tpoint dst, Twr dir) {
  Tsqmd* md = lds_sqmd_new(src);
  md->t[dir].chg = 0;
  md->t[dir].wrd = dir;
  md->t[dir].dest = dst;
  labyrinthe->squares[src.x][src.y].opt = LDS_OptMD;
  labyrinthe->squares[src.x][src.y].sq_mdp = md;
  labyrinthe->squares[dst.x][dst.y].opt = LDS_OptMD;
  labyrinthe->squares[dst.x][dst.y].sq_mdp = lds_sqmd_new(dst);

}

/* Draws in a square if not an input or output */
void update_square_xy(Tlds* labyrinthe,TdrawOpt newOpt,int x,int y) {
  TsquareKind kind = labyrinthe->squares[x][y].kind;
  TsquareOpt opt = labyrinthe->squares[x][y].opt;

  if(kind == LDS_IN)
    printf("(%d,%d) is an input. Can't put a wall here.\n",x,y);
  else if (kind == LDS_OUT)
    printf("(%d,%d) is an output. Can't put a wall here.\n",x,y);
  else if (opt == LDS_OptMD)
    printf("(%d,%d) is a magic door. Can't put a wall here.\n",x,y);
  else if (opt == LDS_OptWH)
    printf("(%d,%d) is a magic door. Can't put a wall here.\n",x,y);
  else
    lds_draw_xy(labyrinthe,newOpt,x,y);
}

/* Draws in a square if not an input or output */
void update_square_pt(Tlds* labyrinthe,TdrawOpt newOpt,Tpoint pt) {
  update_square_xy(labyrinthe, newOpt, pt.x, pt.y);
}

void show(Tlds *labyrinthe){
    lds_dump(labyrinthe,stdout);
}
%}

%parse-param {Tlds *ds}

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
    //Texpr* expr;
    Tpoint *pt;
    Tpoints *suite_pt;
    Tpoint3s *suite_pt_val;
    TdrawOpt constr;
    Twr dir;
}
%type<entier> CNUM xcst
/*%type<expr> expr*/
%type<id> IDENT
%type<dir> DIR
%type<pt> pt
%type<suite_pt> suite_pt
%type<suite_pt_val> suite_pt_val pt_val
%type<constr> constr

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
  | IN pt ';'  {put_in(ds,$2->x,$2->y);}
  | OUT suite_pt ';' {fill_out(ds,$2);}
  | SHOW {show(ds);}
  | constr ';' {
    int x,y;
    for(x = 0; x < ds->dx; x++) {
      for(y = 0; y < ds->dy; y++) {
        update_square_xy(ds,$1,x,y);
      }
    }
  }
  | constr PTA suite_pt ';' { lds_draw_pts(ds, $1, $3); }
  | constr PTD pt suite_pt_val ';'
  | constr R pt pt ';' {
    Tpoint min = (Tpoint){MIN($3->x, $4->x), MIN($3->y, $4->y)};
    Tpoint max = (Tpoint){MAX($3->x, $4->x), MAX($3->y, $4->y)};

    int i;
    for(i = min.x; i < max.x; i++) {
      lds_draw_xy(ds, $1, i, min.y);
      lds_draw_xy(ds, $1, i, max.y);
    }

    for(i = min.y; i <= max.y; i++) {
      lds_draw_xy(ds, $1, min.x, i);
      lds_draw_xy(ds, $1, max.x, i);
    }
  }
  | constr R F pt pt ';' {
    Tpoint min = (Tpoint){MIN($4->x, $5->x), MIN($4->y, $5->y)};
    Tpoint max = (Tpoint){MAX($4->x, $5->x), MAX($4->y, $5->y)};

    int x,y;
    for(x = min.x; x <= max.x; x++) {
      for(y = min.y; y <= max.y; y++) {
        lds_draw_xy(ds, $1, x, y);
      }
    }


  }
  | constr FOR for_args pt ';'
  | WH pt ARROW pt pt_arrow
  | MD pt DIR pt dest_list { define_md(ds,*$2,*$4,$3); }
;

instr_size
  : SIZE xcst ';' {initialize_size(ds,$2,$2);}
  | SIZE xcst ',' xcst ';' {initialize_size(ds,$2,$4);}
;

instr_vars
  : IDENT '=' xcst ';' {
    add_var(u_strdup($1),$3);
  }
  | IDENT PLUSE xcst ';' {
    operation(u_strdup($1), $3, 1);
  }
  | IDENT MINUSE xcst ';' {
    operation(u_strdup($1), $3, 2);
  }
  | IDENT DIVE xcst ';' {
    operation(u_strdup($1), $3, 3);
  }
  | IDENT MULTE xcst ';' {
    operation(u_strdup($1), $3, 4);
  }
  | IDENT MODE xcst ';' {
    operation(u_strdup($1), $3, 5);
  }
;

suite_instr_vars
  : suite_instr_vars instr_vars
  |
;

/*expr
  : CNUM { $$ = expr_cst($1); }
  | IDENT { $$ = expr_varEated(u_strdup($1)); }
  | expr '*' expr { $$ = expr_binOp(EXPKD_TIME, $1, $3); }
  | expr '+' expr { $$ = expr_binOp(EXPKD_PLUS, $1, $3); }
  | expr '-' expr { $$ = expr_binOp(EXPKD_MINUS, $1, $3); }
  | expr '/' expr { $$ = expr_binOp(EXPKD_DIV, $1, $3); }
  | expr '%' expr { $$ = expr_binOp(EXPKD_MOD, $1, $3); }
  | '(' expr ')' { $$ = expr_uniOp(EXPKD_NONE, $2); }
  | '+' expr { $$ = expr_uniOp(EXPKD_NONE, $2); }
  | '-' expr { $$ = expr_uniOp(EXPKD_NEG, $2); }
;*/

xcst
	: CNUM { $$ = $1; }
	| IDENT {
      $$ = check_var(u_strdup($1));
		}
	| xcst '+' xcst	{ $$ = $1 + $3;	}
	| xcst '-' xcst	{ $$ = $1 - $3;	}
	| xcst '*' xcst	{ $$ = $1 * $3;	}
	| xcst '/' xcst	{ $$ = $1 / $3;	}
	| xcst '%' xcst	{ $$ = $1 % $3;	}
	| '+' xcst { $$ = $2; }
	| '-' xcst { $$ = - $2; }
	| '(' xcst ')' { $$ = $2; }
;

for_args
  : IDENT IN range
  | IDENT for_args range
;

pt
  : '(' xcst ',' xcst ')' {$$ = init_pt($2,$4);}
;

range
  :'[' xcst ':' xcst ']'
  |'[' xcst ':' xcst '['
  |'[' xcst ':' xcst ':' xcst ']'
  |'[' xcst ':' xcst ':' xcst '['
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
  | pt ':' xcst {
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
  | DIR pt
;

constr
  : WALL { $$ = LG_DrawWall; }
  | UNWALL { $$ = LG_DrawUnwall; }
  | TOGGLE { $$ = LG_DrawToggle; }
;

%%
#include "lex.yy.c"

void yyerror(Tlds *ds, const char* fmt, ...)
{
    fprintf(stderr,"FATAL (line %d): %s (near %s)\n",yylineno,fmt,yytext);
    exit(1);
}
