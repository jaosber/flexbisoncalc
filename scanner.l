%option yylineno

%{

#include <stdio.h>
#include <stdlib.h>
#include "grammar.tab.h"

extern YYSTYPE yylval;
void yyerror(char*);

extern char *yyoriginaltext;
int column_number = 1;


%}

%%

"+" { column_number += yyleng; return OP_ADD; }

"-" { column_number += yyleng; return OP_SUB; }

"*" { column_number += yyleng; return OP_MUL; }

"/" { column_number += yyleng; return OP_DIV; }

"^" { column_number += yyleng; return OP_POW; }

"=" { column_number += yyleng; return OP_EQL; }

"(" { column_number += yyleng; return SYM_PRNL; }

")" { column_number += yyleng; return SYM_PRNR; }


"sin"|"SIN"   { column_number += yyleng; return FUNC_SIN; }

"cos"|"COS"   { column_number += yyleng; return FUNC_COS; }

"tan"|"TAN"   { column_number += yyleng; return FUNC_TAN; }

"sqrt"|"SQRT" { column_number += yyleng; return FUNC_SQRT; }

"exp"|"EXP"   { column_number += yyleng; return FUNC_EXP; }

"cosh"|"COSH" { column_number += yyleng; return FUNC_COSH; }

"sinh"|"SINH" { column_number += yyleng; return FUNC_SINH; }

"tanh"|"TANH" { column_number += yyleng; return FUNC_TANH; }

"abs"|"fabs"|"ABS"|"FABS" { column_number += yyleng; return FUNC_ABS; }

"log"|"LOG" { column_number += yyleng; return FUNC_LOG; }

"ln"|"LN"   { column_number += yyleng; return FUNC_LN; }

"floor"|"FLOOR"   { column_number += yyleng; return FUNC_FLOOR; }

"ceil"|"CEIL"   { column_number += yyleng; return FUNC_CEIL; }

[0-9]+ { column_number += yyleng; sscanf(yytext, "%lf", &yylval); return T_INT; }

[0-9]+(\.?[0-9]*e?-?[0-9]*)?  { column_number += yyleng; sscanf(yytext, "%lf", &yylval); return T_FLOAT; }

[a-zA-Z]([a-zA-Z]|[0-9])* { column_number += yyleng; return T_IDEN; }

\n { column_number = 1; return EOL; }

[ \t]+ { column_number += yyleng; }

"quit"|"exit" { column_number += yyleng; return CMD_EXT; }

. { column_number += yyleng; yyerror("Unrecognized input!"); }

%%

int yywrap(){
  

	return 1;
}


