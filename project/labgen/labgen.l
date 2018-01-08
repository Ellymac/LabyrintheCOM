
%option noyywrap

%%
[0-9]+ {yylval.entier = atoi(yytext); return CNUM;}
(N|E|S|W|NE|NW|SE|SW) { return DIR;}
SIZE {return SIZE;}
IN {return IN;}
OUT {return OUT;}
SHOW {return SHOW;}
PTA {return PTA;}
PTD {return PTD;}
FOR {return FOR;}
R {return R;}
F {return F;}
WH {return WH;}
MD {return MD;}

WALL {return WALL;}
UNWALL {return UNWALL;}
TOGGLE {return TOGGLE;}

\+= {return PLUSE;}
-= { return MINUSE;}
\*= { return MULTE;}
\/= { return DIVE;}
%= { return MODE;}
(-->|->) {return ARROW;}

([a-z]|[A-Z])([a-z]|[A-Z]|[0-9])* {yylval.id = strdup(yytext); return IDENT;}

[ \t\n] ;

#[^\n]*\n       ;
.          return *yytext;
