#pragma once
#include "astexpressions.h"

class statement : public ast
{
	private:

	public:
		statement()
			{ }
};

class statementlist : public ast
{
	private:
		astnodelist<statement*> statements;
		bool repeat;

	public:
		statementlist()
			{ }

		void addStatement(statement* st)
			{ statements.add(st); }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		void setRepeat(bool val)
			{ repeat = val; }
		bool repeats()
			{ return repeat; }
		astnodelist<statement*> getNodeList()
			{ return statements; }
};


class letstatement : public statement
{
	private:
		expression* lv;
		expression* value;

	public:
		letstatement(expression* _lv, expression* vl)
			: lv(_lv)
			, value(vl)
			{ }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~letstatement();
};

class callstatement : public statement
{
	private:
		symbolid n;
		expression* e;

	public:
		callstatement(symbolid _s, expression* _e)
			: n(_s)
			, e(_e)
			{ }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~callstatement()
			{ delete e; }
};


class dostatement : public statement
{
	private:
		expression* condexp;
		statementlist* allstatements;

	public:
		dostatement(expression* exp, statementlist* lst)
			: condexp(exp)
			, allstatements(lst)
			{ }
			
		dostatement(statementlist* lst)
			: condexp(0)
			, allstatements(lst)
			{ }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual expression* getConditional()
			{ return condexp; }
		virtual void semanticCheck();
		virtual ~dostatement();
};

class choosestatement : public statement
{
	private:
		statementlist* allstatements;

	public:
		choosestatement(statementlist* lst)
			: allstatements(lst)
			{ }
			
		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~choosestatement();
};


class expressionstatement : public statement
{
	private:
		expression* expr;

	public:
		expressionstatement(expression* x)
			: expr(x)
			{ }
			
		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~expressionstatement();
};

/*
class returnstatement : public statement
{
	private:
		typespec* t;
		expression* value; // this expression needs to evaluate to an object reference.

	public:
		returnstatement(typespec* tt, expression* x)
			: t(tt)
			, value(x)
			{ }
			
		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~returnstatement();
};
*/

