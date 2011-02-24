
/*
%option noyywrap
 the length of the input token is in yyleng

 support this syntax:
 	open a new file, writeTo it with "hello world", and close it.
 and this:
 	let a, which is a float, be 5.5


	typenames should not start with an uppercase letter.

	identifiers must contain at least one uppercase letter-
	or start with an uppercase letter?
	because they are proper names, and because it will prevent
	future collisions with new reserved words.
	*/


%{
#include <stdlib.h>
#include "astprogram.h"
	// you have to include the astprogram.h file because toklas.tab.h
	// refers to it but doesn't include it for some reason.

void yyerror(char *);
#include "toklas.tab.h"

extern bool debugit;

void tdebug(const char* s);
void t2debug(const char* s, const char* s2);

int linenum = 0;
bool incomment = false;


/*
which{ws}+is{ws}+an?				{ if (!incomment) { tdebug("[WHICH_IS_A]"); return WHICH_IS_A; }}
as{ws}+an?							{ if (!incomment) { tdebug("[AS_A]"); return AS_A; }}
repeat,{ws}+letting				{ if (!incomment) { tdebug("[REPEAT_LETTING]"); return REPEAT_LETTING; }}
a{ws}+new							{ if (!incomment) { tdebug("[A_NEW]"); return A_NEW; }}
the									{ if (!incomment) { tdebug("[THE]"); return THE; }}
of										{ if (!incomment) { tdebug("[OF]"); return OF; }}
it										{ if (!incomment) { tdebug("[IT]"); return IT; }}
*/

%}

al		[a-zA-Z]
alnum [a-zA-Z0-9]
id {al}{alnum}*
digit [0-9]
digseq {digit}+
float {digseq}\.{digit}{digseq}?
ordinalnumber {digseq}(st|nd|rd|th)
ws [ \t\n]
mathop [+-/*%]
compop [<>=]

%%


to{ws}+run							{ if (!incomment) { tdebug("[TO_RUN]"); return TO_RUN; }}
to{ws}+construct					{ if (!incomment) { tdebug("[TO_CONSTRUCT]"); return TO_CONSTRUCT; }}
to{ws}+get							{ if (!incomment) { tdebug("[TO_GET]"); return TO_GET; }}
to{ws}+set							{ if (!incomment) { tdebug("[TO_SET]"); return TO_SET; }}
choose{ws}+one						{ if (!incomment) { tdebug("[CHOOSE_ONE]"); return CHOOSE_ONE; }}
arrays?{ws}+of?					{ if (!incomment) { tdebug("[ARRAY_OF]"); return ARRAY_OF; }}
initialized{ws}+to				{ if (!incomment) { tdebug("[INITIALIZED_TO]"); return INITIALIZED_TO; }}
otherwise							{ if (!incomment) { tdebug("[OTHERWISE]"); return OTHERWISE; }}
do{ws}+this							{ if (!incomment) { tdebug("[DO_THIS]"); return DO_THIS; }}
called								{ if (!incomment) { tdebug("[CALLED]"); return CALLED; }};
an?									{ if (!incomment) { tdebug("[A]"); return A; }};
has									{ if (!incomment) { tdebug("[HAS]"); return HAS; }}
let									{ if (!incomment) { tdebug("[LET]"); return LET; }}
the									{ if (!incomment) { tdebug("[THE]"); return THE; }}
of										{ if (!incomment) { tdebug("[OF]"); return OF; }}
be										{ if (!incomment) { tdebug("[BE]"); return BE; }}
is										{ if (!incomment) { tdebug("[IS]"); return IS; }}
can									{ if (!incomment) { tdebug("[CAN]"); return CAN; }}
with									{ if (!incomment) { tdebug("[WITH]"); return WITH; }}
to										{ if (!incomment) { tdebug("[TO]"); return TO; }}
from									{ if (!incomment) { tdebug("[FROM]"); return FROM; }}
(while)|(if)						{ if (!incomment) { tdebug("[WHILE]"); return WHILE; }}
repeat								{ if (!incomment) { tdebug("[REPEAT]"); return REPEAT; }}
stop									{ if (!incomment) { tdebug("[STOP]"); return STOP; }}

,										{ if (!incomment) { tdebug("<COMMA>"); return *yytext; }}
,{ws}+and							{ if (!incomment) { tdebug("[COMMA_AND]"); return COMMA_AND; }}
and									{ if (!incomment) { tdebug("[AND]"); return AND; }}

:										{ if (!incomment) { tdebug("<COLON>"); return *yytext; }}
\.										{ if (!incomment) { tdebug("<PERIOD>"); return *yytext; }}
\(										{ if (!incomment) { tdebug("<LPAREN>"); return *yytext; }}
\)										{ if (!incomment) { tdebug("<RPAREN>"); return *yytext; }}
\){ws}+th							{ if (!incomment) { tdebug("[PAREN_TH]"); return PAREN_TH; }}

{mathop}								{ if (!incomment) { t2debug("|MATHOP|", yytext); return *yytext; }}


[[]									{ incomment = true; }
\]										{ incomment = false; }

{id}									{ if (!incomment) { t2debug("|ID|", yytext); yylval.str = yytext ; return ID; }}
{digseq} 							{ if (!incomment) { t2debug("|DIGSEQ|", yytext); yylval.intnum = atoi(yytext) ; return INTNUMBER; }}
{float} 								{ if (!incomment) { t2debug("|FLOAT|", yytext); yylval.floatnum = atof(yytext) ; return FLOATNUMBER; }}
{ordinalnumber} 					{ if (!incomment) { t2debug("|ORDINALNUMBER|", yytext); yylval.intnumord = atoi(yytext) ; return ORDINALNUMBER; }}
[<]=										{ if (!incomment) { tdebug("|LEQ|"); return LEQ; }}
>=										{ if (!incomment) { tdebug("|GEQ|"); return LEQ; }}
{compop}								{ if (!incomment) { t2debug("<COMPOP>", yytext); return *yytext; }}

[ \t]									/* nothing */
[\n]									linenum++;

										/* this is here so anything that doesn't fit the above, like weird punctuation,
											gets ignored if you're in a comment -- otherwise obviously we need a better
											error.
										*/
.										{ if (!incomment) { printf("lexer crashed."); exit(1); }}

%%

int tcount = 0;

int yywrap()
{
	// finished parsing input -- now look for program database part?
	// this will get called when the EOF character appears
	// even if there is "more to the file" than that.

	// return 1 to mean, don't continue lexing.
	return 1;
}

void t2debug(const char* c, const char* yyl)
{
	tcount++;
	if (debugit) {
		printf("%3d : %3d : %10s : %10s\n", tcount, linenum, c, yyl);
	}
}

void tdebug(const char* c)
{
	tcount++;
	if (debugit) {
		printf("%3d : %3d : %10s : %10s\n", tcount, linenum, c, c);
	}
}

