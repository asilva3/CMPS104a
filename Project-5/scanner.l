%{
// $Id: scanner.l,v 1.2 2014-11-12 18:13:43-08 - - $

#include "auxlib.h"
#include "lyutils.h"

#define YY_USER_ACTION  { scanner_useraction (); }
#define IGNORE(THING)   { }

%}

%option 8bit
%option debug
%option nodefault
%option nounput
%option noyywrap
%option verbose
%option warn

LETTER          [A-Za-z_]
DIGIT           [0-9]
IDENT           ({LETTER}({LETTER}|{DIGIT})*)
CHAR            ('([^\\'\n]|\\[\\'"0nt])')
STRING          (\"([^\\"\n]|\\[\\'"0nt])*\")
Integer         ({DIGIT}+)
MANTISSA        ({DIGIT}+\.?{DIGIT}*|\.{DIGIT}+)
EXPONENT        ([Ee][+-]?{DIGIT}+)
NUMBER          ({MANTISSA}{EXPONENT}?)
NOTIDENT        ({DIGIT}({LETTER}|{DIGIT})*)
NOTCHAR         ('([^\\'\n]|\\.)'?)
NOTSTRING       (\"([^\\"\n]|\\.)*(\")?)
NOTNUMBER       ({MANTISSA}[Ee][+-]?)

%%

"#".*           { scanner_include(); }
[ \t]+          { IGNORE (white space) }
\n              { scanner_newline(); }


"void"          { return yylval_token(TOK_VOID);}
"bool"          { return yylval_token(TOK_BOOL);}
"char"          { return yylval_token(TOK_CHAR);}
"int"           { return yylval_token(TOK_INT);}
"string"        { return yylval_token(TOK_STRING);}
"struct"        { return yylval_token(TOK_STRUCT);}
"if"            { return yylval_token(TOK_IF);}
"else"          { return yylval_token(TOK_ELSE);}
"while"         { return yylval_token(TOK_WHILE);}
"return"        { return yylval_token(TOK_RETURN);}
"false"         { return yylval_token(TOK_FALSE);}
"true"          { return yylval_token(TOK_TRUE);}
"null"          { return yylval_token(TOK_NULL);}
"ord"           { return yylval_token(TOK_ORD);}
"chr"           { return yylval_token(TOK_CHR);}
"new"           { return yylval_token(TOK_NEW);}

{IDENT}         { return yylval_token(TOK_IDENT);}
{CHAR}          { return yylval_token(TOK_CHARCON);}
{STRING}        { return yylval_token(TOK_STRINGCON);}
{Integer}       { return yylval_token(TOK_INTCON);}
{NOTIDENT}      { scanner_badtoken(yytext);
                  return yylval_token(TOK_IDENT);
                }
{NOTCHAR}       { scanner_badtoken(yytext);
                  return yylval_token(TOK_CHARCON);
                }
{NOTSTRING}     { scanner_badtoken(yytext);
                  return yylval_token(TOK_STRINGCON);
                }


"[]"            { return yylval_token(TOK_ARRAY);}
"["             { return yylval_token('[');}
"]"             { return yylval_token(']');}
"{"             { return yylval_token('{');}
"}"             { return yylval_token('}');}
","             { return yylval_token(',');}
"."             { return yylval_token('.');}
"=="            { return yylval_token(TOK_EQ);}
"!="            { return yylval_token(TOK_NE);}
"<"             { return yylval_token(TOK_LT);}
"<="            { return yylval_token(TOK_LE);}
">"             { return yylval_token(TOK_GT);}
">="            { return yylval_token(TOK_GE);}
"%"             { return yylval_token('%');}
"!"             { return yylval_token('!');}
"="             { return yylval_token ('='); }
"+"             { return yylval_token ('+'); }
"-"             { return yylval_token ('-'); }
"*"             { return yylval_token ('*'); }
"/"             { return yylval_token ('/'); }
"^"             { return yylval_token ('^'); }
"("             { return yylval_token ('('); }
")"             { return yylval_token (')'); }
";"             { return yylval_token (';'); }


.               { scanner_badchar (*yytext); }


%%

