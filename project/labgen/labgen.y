%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(const char* mess);
%}

%token IDENT CNUM DIR
%token SIZE IN OUT SHOW
%token WALL UNWALL TOGGLE
%token PTA PTD R F FOR
%token WH MD
%token PLUSE MINUSE MULTE DIVE MODE
%token ARROW

%left '+' '-'
%right '*' '/'
%left '%'

%%


labyrinthe
  : suite_instr
;

suite_instr
  : suite_instr instr
  | instr
;

instr
  : ';'
  | IDENT '=' expr ';'
  | SIZE expr ';'
  | SIZE expr ',' expr ';'
  | IN pt ';'
  | OUT pt suite_pt ';'
  | SHOW ';'
  | IDENT PLUSE expr ';'
  | IDENT MINUSE expr ';'
  | IDENT MULTE expr ';'
  | IDENT DIVE expr ';'
  | IDENT MODE expr ';'
  | constr ';'
  | constr PTA pt suite_pt ';'
  | constr PTD pt suite_pt_val ';'
  | constr R pt pt ';'
  | constr R F pt pt ';'
  | constr FOR IDENT suite_ident IN range suite_range pt';'
  | WH pt ARROW pt pt_arrow
  | MD pt DIR pt dest_list
;

expr
  : CNUM
  | IDENT
  | expr '*' expr
  | expr '+' expr
  | expr '-' expr
  | expr '/' expr
  | expr '%' expr
  | '(' expr ')' {$$ = $2;}
  | '+' expr
  | '-' expr

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

suite_ident
  :suite_ident IDENT
  |
;

suite_range
  :suite_range range
  |
;

%%
#include "lex.yy.c"

int yyerror(const char* mess)
{
    fprintf(stderr,"FATAL (line %d): %s (near %s)\n",yylineno,mess,yytext);
    exit(1);
}
