%{
#include <stdio.h>
#include <stdlib.h>
int yylex();
int yyerror(const char* mess);

// size
int labHeight = 3;
int labWidth = 3;
// entry
int eX = 0;
int eY = 0;
// exit
int sX = 2;
int sY = 2;
// walls
int wall1X = 2;
int wall1Y = 0;
int wall2X = 1;
int wall2Y = 1;
int wall3X = 0;
int wall3Y = 2;
// current position
int currentPosX = 0;
int currentPosY = 0;
// game status
int status;

int changePosition(int* currentX, int* currentY, char dir) {
  switch(dir) {
    case 'N' :
      *currentY -= 1;
      break;
    case 'S' :
      *currentY += 1;
      break;
    case 'E' :
      *currentX += 1;
      break;
    case 'W' :
      *currentX -= 1;
      break;
    default :
      break;
  }
  // invalid path
  if(*currentY < 0 || *currentX < 0 || *currentY > labHeight - 1 || *currentX > labWidth - 1)
    return -1;
  // exit
  if(*currentX == sX && *currentY == sY)
    return 1;
  // walls
  if(*currentX == 2 && *currentY == 0 || *currentX == 1 && *currentY == 1 || *currentX == 0 && *currentY == 2)
    return -1;
  // else, valid direction
  return 0;
}
%}

%token N S E W

%%

chemin
  : suite_dir
;

suite_dir
  : suite_dir dir
  | dir
;

dir
  : N {
      status = changePosition(&currentPosX, &currentPosY, 'N');
      if(status == -1) {
        printf("Perdu ! Chemin non valide. Position courante : (%d,%d).\n\n", currentPosX, currentPosY);
        exit(1);
      }
      else if(status == 1) {
        printf("Gagné ! Position courante : (%d,%d).\n\n", currentPosX, currentPosY);
        exit(0);
      }
    }
  | S {
    status = changePosition(&currentPosX, &currentPosY, 'S');
    if(status == -1) {
      printf("Perdu ! Chemin non valide. Position courante : (%d,%d).\n\n", currentPosX, currentPosY);
      exit(1);
    }
    else if(status == 1) {
      printf("Gagné ! Position courante : (%d,%d).\n\n", currentPosX, currentPosY);
      exit(0);
    }
  }
  | E {
    status = changePosition(&currentPosX, &currentPosY, 'E');
    if(status == -1) {
      printf("Perdu ! Chemin non valide. Position courante : (%d,%d).\n\n", currentPosX, currentPosY);
      exit(1);
    }
    else if(status == 1) {
      printf("Gagné ! Position courante : (%d,%d).\n\n", currentPosX, currentPosY);
      exit(0);
    }
  }
  | W {
    status = changePosition(&currentPosX, &currentPosY, 'W');
    if(status == -1) {
      printf("Perdu ! Chemin non valide. Position courante : (%d,%d).\n\n", currentPosX, currentPosY);
      exit(1);
    }
    else if(status == 1) {
      printf("Gagné ! Position courante : (%d,%d).\n\n", currentPosX, currentPosY);
      exit(0);
    }
  }
;

%%

#include "lex.yy.c"

int yyerror(const char* mess)
{
    fprintf(stderr,"FATAL: %s (near %s at line %d)\n",mess,yytext, yylineno);
    exit(1);
}

int main() {
  yyparse();
  if(status != 1) {
    printf("Perdu ! Sortie non atteinte. Position courante : (%d,%d).\n\n", currentPosX, currentPosY);
    exit(1);
  }
  return 0;
}
