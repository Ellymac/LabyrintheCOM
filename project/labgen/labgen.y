%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/top.h"
#include "src/vars.h"
int yylex();

int width = -1;
int height = -1;
int has_entry = 0;
char **labyrinthe;
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

/* Initialize labyrinth's size */
void initialize_size(int h, int w){
    if (h < 2 || w < 2){
        printf("Erreur : taille invalide !\n");
        exit(1);
    }
    else{
        width = w;
        height = h;
        labyrinthe = calloc(height+1,sizeof(char *));
        int i;
        for (i = 0 ; i < height ; i++){
            labyrinthe[i] = calloc(width+1,sizeof(char));
        }
    }
}

/*
  Define entry
TODO ajouter une condition pour vérifier les entrées/sorties
    des trous de vers
*/
void put_in(int h, int w){
    if (h!=0 && h!=height && w!=0 && w!=width){
        printf("Erreur : entrée invalide !\n");
        exit(1);
    }
    else if (has_entry == 1){
        printf("Erreur : entrée déjà présente !\n");
        exit(1);
    }
    else{
        if (labyrinthe[h][w] == 'm'){
            printf("Attention : présence d'un mur (supprimé) \n");
        }
        labyrinthe[h][w] = 'e';
        has_entry = 1;
    }
}

/*
  Define output
TODO ajouter une condition pour vérifier les entrées/sorties
    des trous de vers
*/
void put_out(int h, int w){
    if (h!=0 && h!=height && w!=0 && w!=width){
        printf("Erreur : sortie invalide !\n");
        exit(1);
    }
    else{
        if (labyrinthe[h][w] == 'm'){
            printf("Attention : présence d'un mur (supprimé) \n");
        }
        labyrinthe[h][w] = 's';
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
    int pt[2];
    int suite_pt[50][2];
    Cstr id;
}
%type<entier> CNUM expr
%type<id> IDENT
%type<pt> pt
%type<suite_pt> suite_pt


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
  | IN pt ';' {put_in($2[1],$2[0]);}
  | OUT suite_pt ';'
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
  : '(' expr ',' expr ')' {$$[0] = $2; $$[1] = $4;}
;

range
  :'[' expr ':' expr ']'
  |'[' expr ':' expr '['
  |'[' expr ':' expr ':' expr ']'
  |'[' expr ':' expr ':' expr '['
;

/**
TODO finir suite_pt
*/
suite_pt
  : suite_pt pt
  | pt {$$[0][0] = $1[0]; $$[0][1] = $1[1];}
;

suite_pt_val
  : suite_pt_val pt_val
  |
;

pt_val
  : pt
  | pt ':' expr
  | pt ':' '*'
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

void yyerror(const char* mess, ...)
{
    fprintf(stderr,"FATAL (line %d): %s (near %s)\n",yylineno,mess,yytext);
    exit(1);
}
