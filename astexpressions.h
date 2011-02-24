#pragma once
#include "asttyping.h"

//////////////// expressions

class expression : public ast
{
	public:
		type* datatype;
		expression(type* dtype)
			: datatype(dtype)
			{ }
		expression()
			{ }
		virtual void generateStoreCode(registerid reg, operation* &p)
			{ printf("cannot store.\n"); exit(1); }
		virtual registerid addressOf(operation* &p)
			{ printf("cannot get address.\n"); exit(1); }
		// note: we would not destroy datatype when destroying this object
};

class ordinal : public expression
{
	private:
		expression* e;

	public:
		ordinal(expression* _e)
			: e(_e)
			{ }
		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~ordinal();
};

class theExpr : public expression
{
	private:
		symbolid sid;
		ordinal* ord;
	public:
		theExpr(symbolid id)
			: sid(id)
			, ord(0)
			{ }
		theExpr(ordinal* _ord)
			: sid(-1)
			, ord(_ord)
			{ }
		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		symbolid getSymbolid()
			{ return sid; }
};


class literal : public expression
{
	private:
		datum value;
	public:
		literal(type* dtype, datum val)
			: expression(dtype)
			, value(val)
			{ }
		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
};


class conditionalExpression : public expression
{
	private:
		//int opcode;
		int ctype;
		expression* leftOperand;
		expression* rightOperand;

	public:
		conditionalExpression(int ct, expression * lo, expression *ro)
			: ctype(ct)
			, leftOperand(lo)
			, rightOperand(ro)
			{ }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~conditionalExpression();
};

class binaryExpression : public expression
{
	private:
		//int opcode;
		int optype;
		expression* leftOperand;
		expression* rightOperand;

	public:
		binaryExpression(int opt, expression * lo, expression *ro)
			: optype(opt)
			, leftOperand(lo)
			, rightOperand(ro)
			{ }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~binaryExpression();
};


class prepositionval : public ast
{
	private:
		expression* value;

	public:
		prepositionval(expression* val)
			: value(val)
			{ }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~prepositionval();
};


class prepvallist : public ast
{
	private:
		astnodelist<prepositionval*> preps;

	public:
		prepvallist()
			{ }

		void addPrepositionValue(prepositionval* p)
			{ preps.add(p); }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
};

class constructExpression : public expression
{
	private:
		aExpr* t;
		prepvallist* prepvals;

	public:
		constructExpression(aExpr* tt, prepvallist* pvals)
			: t(tt)
			, prepvals(pvals)
			{ }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~constructExpression();
};




// could be a getter OR a setter?
class call : public expression
{
	private:
		theExpr* funcname;
		expression* objref; // this expression needs to evaluate to an object reference.
		prepvallist* prepval;

	public:
		call(theExpr* nm)
			: funcname(nm)
			, objref(0)
			, prepval(0)
			{ }

		call(theExpr* nm, expression* x)
			: funcname(nm)
			, objref(x)
			, prepval(0)
			{ }

		call(theExpr* nm, expression* x, prepvallist* pval)
			: funcname(nm)
			, objref(x)
			, prepval(pval)
			{ }
			
		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~call();
};

class varRef : public expression
{
	private:

	public:
		symbolid varname;
		varRef(symbolid vn)
			: varname(vn)
			{ }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual void generateStoreCode(registerid reg, operation* &p);
};



class arrayAccess : public expression
{
	private:
		ordinal* n;
		expression* of;
	
	public:
		arrayAccess(ordinal* _n, expression* _of)
			: n(_n)
			, of(_of)
			{ }

	virtual void debug(int ind);
	virtual registerid generateCode(operation* &p);
//	virtual void generateStoreCode(registerid result, operation* &p);
	virtual void semanticCheck();
	virtual ~arrayAccess();
};


