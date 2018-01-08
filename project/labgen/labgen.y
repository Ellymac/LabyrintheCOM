%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int yylex();
int yyerror(const char* mess);

int width = -1;
int height = -1;
char **labyrinthe;

char **name_var;
int *value_var;
int nb_var = 0;

void init(){
    name_var = calloc(100,sizeof(char *));
    value_var = calloc(100,sizeof(int));
}
int check_var(char* var){
    int i;
    int find = 0;
    int result;
    for (i = 0 ; i < nb_var && find == 0; i++){
        if (strcmp(var, name_var[i]) == 0){
            find = 1;
            result = value_var[i];
        }
    }
    if (find == 1){
        return result;
    }
    else{
        printf("Erreur : nom de variable introuvable !\n");
        exit(1);
    }
}
void add_var(char *var, int value){
    name_var[nb_var] = calloc(strlen(var),sizeof(char));
    name_var[nb_var] = var;
    value_var[nb_var] = value;
    if (value != check_var(var)){
        printf("Cette valeur a déjà été ajouté, elle ne sera pas prise en compte\n");
    }
}
void initialize_size(int h, int w){
    if (h < 2 || w < 2){
        printf("Erreur : taille invalide !\n");
        exit(1);
    }
    else{
        width = w;
        height = h;
        labyrinthe = calloc(height,sizeof(char *));
        int i;
        for (i = 0 ; i < height ; i++){
            labyrinthe[i] = calloc(width,sizeof(char));
        }
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
    char *id;
}
%type<entier> CNUM expr
%type<id> IDENT

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
  | IN pt ';'
  | OUT pt suite_pt ';'
  | SHOW
  | constr ';'
  | constr PTA pt suite_pt ';'
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
      add_var($1, $3);
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
      $$ = check_var($1);
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
  : '(' expr ',' expr ')'
;

range
  :'[' expr ':' expr ']'
  |'[' expr ':' expr '['
  |'[' expr ':' expr ':' expr ']'
  |'[' expr ':' expr ':' expr '['
;

suite_pt
  : suite_pt pt
  |
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

int yyerror(const char* mess)
{
    fprintf(stderr,"FATAL (line %d): %s (near %s)\n",yylineno,mess,yytext);
    exit(1);
}
