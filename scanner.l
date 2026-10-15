%{
#include "ast.h"
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>
%}

%option noyywrap

%%

[ \t\r\n]+             ; /* Skip whitespace */

"move"                 { return TOKEN_MOVE; }
"stay"                 { return TOKEN_STAY; }
"behavior"             { return TOKEN_BEHAVIOR; }
"give"                 { return TOKEN_GIVE; }
"void"                 { return TOKEN_VOID; }

"true"                 { yylval.int_val = 1; return TOKEN_BOOL_LITERAL; }
"false"                { yylval.int_val = 0; return TOKEN_BOOL_LITERAL; }

"Number"               { yylval.str_val = strdup(yytext); return TOKEN_TYPE; }
"Text"                 { yylval.str_val = strdup(yytext); return TOKEN_TYPE; }
"Boolean"              { yylval.str_val = strdup(yytext); return TOKEN_TYPE; }
"Angle"                { yylval.str_val = strdup(yytext); return TOKEN_TYPE; }
"Distance"             { yylval.str_val = strdup(yytext); return TOKEN_TYPE; }
"Joint"                { yylval.str_val = strdup(yytext); return TOKEN_TYPE; }
"Direction"            { yylval.str_val = strdup(yytext); return TOKEN_TYPE; }

":"                    { return ':'; }
"="                    { return '='; }
";"                    { return ';'; }
"{"                    { return '{'; }
"}"                    { return '}'; }
"("                    { return '('; }
")"                    { return ')'; }
","                    { return ','; }

\"([^\\\"]|\\.)*\"     {
                         yytext[yyleng - 1] = '\0';
                         yylval.str_val = strdup(yytext + 1);
                         return TOKEN_STRING_LITERAL;
                       }

[0-9]+\.[0-9]+         { yylval.float_val = atof(yytext); return TOKEN_FLOAT_LITERAL; }
[0-9]+                 { yylval.int_val = atoi(yytext); return TOKEN_INT_LITERAL; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.str_val = strdup(yytext); return TOKEN_IDENTIFIER; }

.                      { fprintf(stderr, "Unexpected char: %s\n", yytext); }

%%