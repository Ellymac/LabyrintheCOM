
%option noyywrap

%%
N { return N;}
S { return S;}
E { return E;}
W { return W;}
[ \t\n] ;
#[^\n]*\n       ;
.          return *yytext;
