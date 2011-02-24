#pragma once
#include "ast.h"





/////////////// typing

class aExpr : public ast
{
	private:
		symbolid sid;
	public:
		aExpr(symbolid id)
			: sid(id)
			{ }
		aExpr(string id);
		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual int getSize()
			{ return 1; }
		virtual int getSymbolid()
			{ return sid; }
};

class arrayAExpr : public aExpr
{
	public:
		aExpr* t;
		int n;
		arrayAExpr(int nvals, aExpr* tt)
			: aExpr(-1)
			, t(tt)
			, n(nvals)
			{ }
		virtual int getSize()
			{ return n * t->getSize(); }
		virtual int getSymbolid()
			{ 
				cout << "not implemented: arrayAExpr" << endl;
				exit(1);
			}
		virtual ~arrayAExpr();
};

class preposition : public ast
{
	private:
		symbolid name;
		aExpr* atype;

	public:
		preposition(symbolid n, aExpr* a)
			: name(n)
			, atype(a)
			{ }
		preposition(symbolid n)
			: name(n)
			, atype(0)
			{ }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		virtual ~preposition();
		symbolid getName()
			{ return name; }
};


class preplist : public ast
{
	private:
		astnodelist<preposition*> preps;

	public:
		preplist()
			{ }

		void addPreposition(preposition* p)
			{ preps.add(p); }

		virtual void debug(int ind);
		virtual registerid generateCode(operation* &p);
		virtual void semanticCheck();
		vector<type*> getTypes();
};

