%{

#include "astprogram.h"
#include <stdio.h>

extern program* parsed_program;
extern symboltable symtab;
extern typelibrary typelib;

int yylex(void);
void yyerror(char *);

%}


%token TO_RUN
%token TO_CONSTRUCT
%token TO_GET
%token TO_SET
%token CHOOSE_ONE
%token DO_THIS
%token ARRAY_OF
%token INITIALIZED_TO
%token OF
%token CALLED
%token WHILE
%token REPEAT
%token STOP
%token LET
%token THE
%token IS
%token CAN
%token A
%token BE
%token HAS
%token PAREN_TH
%token OTHERWISE
%token COMMA_AND

%token WITH
%token FROM
%token TO
/*
%token WHICH_IS_A
%token AS_A
%token REPEAT_LETTING
%token A_NEW
%token THE
%token OF
%token IT
*/

%token AND
%token OR


%token LEQ
%token GEQ

%token <str> ID
%token <intnum> INTNUMBER
%token <floatnum> FLOATNUMBER
%token <intnumord> ORDINALNUMBER
%token <mathop> MATHOP
%token <compop> COMPOP

/*
%left '+'
%left '-'
%left '*'
%left '/'
%left '%'
*/

%union {
	char* str;
	int intnum;
	float floatnum;
	int intnumord;
	symbolid stref;
	char mathop;
	program* _yprogram;
	unit* _yunit;
	preplist* _ypreplist;
	statementlist* _ystatementlist;
	statement* _ystatement;
	literal* _yliteral;
	ordinal* _yordinal;
	expression* _yexpression;
	aExpr* _yaexpr;
	theExpr* _ytheexpr;
	preposition* _ypreposition;
	prepvallist* _yprepvallist;
	prepositionval* _yprepositionval;

}


%type <_yprogram> unitlist;
%type <_yunit> unit;
%type <_ypreplist> prepositions;
%type <_ypreplist> preplist;
%type <_ypreposition> prep;
%type <_yprepvallist> prepositionval;
%type <_yprepvallist> prepvallist;
%type <_yprepositionval> prepval;
%type <_yaexpr> typespec
%type <_yaexpr> aexpr
%type <_yaexpr> typebase
%type <_ystatementlist> statementlist;
%type <_ystatementlist> block;
%type <_ystatementlist> dolist;

%type <_ystatement> statement;
%type <_ystatement> letstatement;
%type <_ystatement> dostatement;
%type <_ystatement> choosestatement;
%type <_ystatement> callstatement;
%type <_ystatement> otherwise;

%type <_yexpression> expression;
%type <_yexpression> sum;
%type <_yexpression> term;
%type <_yexpression> callvalue;
%type <_yexpression> singlevalue;
%type <_yexpression> literal;
%type <_yexpression> conditional;
%type <_yexpression> logdisj;
%type <_yexpression> logconj;
%type <_yordinal> ordinal;
%type <_ytheexpr> theexpr;
%type <stref> id




%%

top				:  unitlist { parsed_program = $1; }
					;

unitlist			:	unitlist unit { $1->addUnit($2); $$ = $1; }
					| { $$ = new program(); }
					;

/* top-level units */

unit				:	TO id aexpr prepositions ':' statementlist  { $$ = new proceduredefine($2, $3, $4, $6); }
					|	TO_RUN ':' statementlist  { $$ = new run($3); }
					|	TO_GET theexpr OF aexpr prepositions ':' statementlist  { $$ = new methoddefineunit($2, $4, $5, $7); }
					|	TO_CONSTRUCT aexpr prepositions ':' statementlist  { $$ = new constructordefine($2, $3, $5); }
					|  aexpr HAS aexpr CALLED id '.' { $$ = new propertydefine($1, $3, $5); }
					|  aexpr IS aexpr '.' { $$ = new subclassdefine($1, $3); }
					|	aexpr CAN id prepositions '.' { $$ = new proceduredeclare($1, $3, $4); }
					|	theexpr OF aexpr prepositions IS aexpr '.' { $$ = new functiondeclare($1, $3, $4, $6); }
					;



/* right now prepositions is set up only for declarations
when it is used for CALLING functions it will create conflicts with AND used in conjunctions
answer is not to allow conjunctions to be used as parameters to functions

a Blank CAN foo with this and with that and with that.

*/


prepositions	:	preplist { $$ = $1; }
					|  { $$ = new preplist(); }
					;

preplist			:  preplist conj prep { $$ = $1; $1->addPreposition($3); }
					|  prep { $$ = new preplist(); $$->addPreposition($1); }
					;

prep				:	WITH id typespec { $$ = new preposition($2, $3); }
					|	FROM id typespec { $$ = new preposition($2, $3); }
					|	TO id typespec { $$ = new preposition($2, $3); }
					|	WITH id { $$ = new preposition($2); }
					|	FROM id { $$ = new preposition($2); }
					|	TO id { $$ = new preposition($2); }
					;
			



/* lists of statements */

statementlist	:	statementlist statement { $1->addStatement($2); $$ = $1; }
					|  { $$ = new statementlist(); }
					;

statement		:	letstatement { $$ = $1; }
					|	dostatement { $$ = $1; }
					|	choosestatement { $$ = $1; }
					|	callstatement { $$ = $1; }
					;

callstatement	:	id callvalue '.' { $$ = new callstatement($1, $2); }
					;

letstatement	:	LET expression BE expression '.' { $$ = new letstatement($2, $4); }
					;

dostatement		: DO_THIS block  	  { $$ = new dostatement($2); }
					| WHILE expression DO_THIS block   { $$ = new dostatement($2, $4); } 
					| DO_THIS WHILE expression block   { $$ = new dostatement($3, $4); } 
					;


choosestatement	:	CHOOSE_ONE ':' dolist otherwise { $3->addStatement($4); $$ = new choosestatement($3);  }
						;

dolist				:		dolist dostatement	{ $1->addStatement($2); $$ = $1; }
						|		dostatement	{ $$ = new statementlist(); $$->addStatement($1); }
						;

otherwise			:	OTHERWISE DO_THIS block { $$ = new dostatement($3); }
						;


block				:	':' statementlist REPEAT '.' { $2->setRepeat(true); $$ = $2; }
					|	':' statementlist STOP '.'  { $2->setRepeat(false); $$ = $2; }
					;

					;


/* expressions */

expression		:	logdisj { $$ = $1; }
					;

logdisj			:	logdisj OR logconj { $$ = new conditionalExpression(OPTYPE_OR, $1, $3); }
					|	logconj { $$ = $1; }
					;

logconj			:	logconj AND conditional { $$ = new conditionalExpression(OPTYPE_AND, $1, $3); }
					|	conditional { $$ = $1; }
					;

conditional		:	sum '<' sum { $$ = new conditionalExpression(OPTYPE_LT, $1, $3); }
					|	sum '>' sum { $$ = new conditionalExpression(OPTYPE_GT, $1, $3); }
					|	sum '=' sum { $$ = new conditionalExpression(OPTYPE_EQ, $1, $3); }
					|	sum LEQ sum { $$ = new conditionalExpression(OPTYPE_LTE, $1, $3); }
					|	sum GEQ sum { $$ = new conditionalExpression(OPTYPE_GTE, $1, $3); }
					|	sum { $$ = $1; }
					;

sum				:	sum '+' term { $$ = new binaryExpression(OPTYPE_ADD, $1, $3); }
					|	sum '-' term { $$ = new binaryExpression(OPTYPE_SUB, $1, $3); }
					|	term { $$ = $1; }
					;

term				:	term '*' callvalue { $$ = new binaryExpression(OPTYPE_MULT, $1, $3); }
					|	term '/' callvalue { $$ = new binaryExpression(OPTYPE_DIV, $1, $3); }
					|	term '%' callvalue { $$ = new binaryExpression(OPTYPE_MOD, $1, $3); }
					| 	callvalue { $$ = $1; }
					;

callvalue		:	theexpr OF singlevalue prepositionval { $$ = new call($1, $3, $4); }
					|  aexpr prepositionval { $$ = new constructExpression($1, $2); }
					|	singlevalue { $$ = $1; }
					;

singlevalue		:	literal { $$ = $1; }
					|	id { $$ = new varRef($1); }
					|	theexpr { $$ = $1; }
					|	'(' expression ')' { $$ = $2; }
					;

prepositionval	:	prepvallist { $$ = $1; }
					|  { $$ = new prepvallist(); }
					;

prepvallist		:  prepvallist conj prepval { $$ = $1; $1->addPrepositionValue($3); }
					|  prepval { $$ = new prepvallist(); $$->addPrepositionValue($1); }
					;

prepval			:	WITH singlevalue { $$ = new prepositionval($2); }
					|	FROM singlevalue { $$ = new prepositionval($2); }
					|	TO singlevalue { $$ = new prepositionval($2); }
					;



theexpr			:	THE id { $$ = new theExpr($2); }
					|	THE ordinal { $$ = new theExpr($2); }
					;

ordinal			:	ORDINALNUMBER { $$ = new ordinal(new literal(typelib.getTypeByName("int"), datum($1))); }
					|	'(' expression PAREN_TH { $$ = new ordinal($2); }
					;

literal			: INTNUMBER { $$ = new literal(typelib.getTypeByName("int"), datum($1)); }
					| FLOATNUMBER { $$ = new literal(typelib.getTypeByName("float"), datum($1)); }
					;


/* general rules */

conj				:	COMMA_AND
					|	','
					;

typespec			:	'(' aexpr ')' { $$ = $2; }
					;

aexpr				:	A typebase { $$ = $2; }
					;

typebase			:	id { $$ = new aExpr($1); }
					|	ARRAY_OF INTNUMBER typebase { $$ = new arrayAExpr($2, $3); }
					;


id					: ID { int ref = symtab.addSymbol($1) ; $$ = ref; }
					;



%%

/*


classpropdef:	A id HAS ':' classproplist
				;


classproplist: classproplist vdecl 
				|	classproplist vdecl init 
				:
				;

vdecl:		id '(' A id ')'
				;

init:			INITIALIZED_TO expression
				;


classverbdef:	TO id A id prepositions IS_TO ':' statementlist
				;

prepositions:		prepositions WITH id vdecl conjunction
				|
				;

conjunction	:	AND
				|	','
				|	';'
				;


classgetdef:	TO_GET_THE id OF_A id prepositions IS_TO ':' statementlist
				;

classsetdef:	TO_SET_THE id OF_A id prepositions IS_TO ':' statementlist
				;


statementlist	:	statementlist '.' statement
					| statementlist conjunction statement
					| 
					;

statement:	letstatement
			|	verbstatement	
			|	dostatement
			|	repeatstatement
			|  ifstatement
			|	stopstatement
			;

letstatement	:	LET id BE expression
					|	LET setexpression BE expression
					;


verbstatement	: id expression prepositions
					;


whilecondition	:	WHILE expression
					;


repeatstatement	:	REPEAT 
						|	REPEAT ',' letstatement
						;

ifstatement		:	IF expression THEN_DO_THIS ':'
					;

stopstatement	:	STOP
					;

expression	:	mathexpression
				|	getexpression
				|	convertexpression
				;

convertexpression	:	expression AS id
						;

getexpression	:	THE id OF id prepositions
					;

setexpression	:	THE id OF id prepositions
					;



*/




void yyerror(char *s)
{
	printf("%s\n", s);
}

/*
int main(void)
{
	int result = yyparse();
	printf("done parsing... result: %s\n", (result ? "error" : "success"));
	return 0;
}
*/
