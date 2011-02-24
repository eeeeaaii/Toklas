#pragma once
#include "aststatements.h"

///////////// units

class unit : public ast
{
	public:
		unit()
			{ };
		virtual string getName()
			{ return ""; }
		virtual void createTypes()
			{ }
};


class subclassdefine : public unit
{
	private:
		aExpr* thisclass;
		aExpr* superclass;


	public:
		subclassdefine(aExpr* _thisclass, aExpr* _superclass)
			: thisclass(_thisclass)
			, superclass(_superclass)
			{ }

		virtual void debug(int ind);
		virtual void createTypes();
		virtual ~subclassdefine();

};


class propertydefine : public unit
{
	private:
		aExpr* obj;
		aExpr* proptype;
		symbolid propid;


	public:
		propertydefine(aExpr* _obj, aExpr* _proptype, symbolid _propid)
			: obj(_obj)
			, proptype(_proptype)
			, propid(_propid)
			{ }

		virtual void debug(int ind);
		virtual void createTypes();
		virtual ~propertydefine();

};

class proceduredeclare : public unit
{
	private:
		aExpr* thisclass;
		symbolid procname;
		preplist* params;


	public:
		proceduredeclare(aExpr* _thisclass, symbolid _procname, preplist* _params)
			: thisclass(_thisclass)
			, procname(_procname)
			, params(_params)
			{ }

		virtual void debug(int ind);
		virtual void createTypes();
		virtual ~proceduredeclare();

};

class functiondeclare : public unit
{
	private:
		theExpr* funcname;
		aExpr* classname;
		preplist* params;
		aExpr* returntype;


	public:
		functiondeclare(theExpr* _funcname, aExpr* _classname, preplist* _params, aExpr* _returntype)
			: funcname(_funcname)
			, classname(_classname)
			, params(_params)
			, returntype(_returntype)
			{ }

		virtual void debug(int ind);
		virtual void createTypes();
		virtual ~functiondeclare();

};


class functionunit : public unit
{
	protected:
		statementlist* statements;
		preplist* preps;
		method* mymethod;

		functionunit(statementlist* slist, preplist* p)
			: statements(slist)
			, preps(p)
			, mymethod(0)
			{ }
		virtual void createTypes()
			{ }
		registerid generateCode(operation* &p);
		virtual ~functionunit();
};

class run : public functionunit
{
	public:
		run(statementlist* slist)
			: functionunit(slist, 0)
			{ }

		virtual void debug(int ind);
		virtual void createTypes();
};

class proceduredefine : public functionunit
{
	private:
		symbolid name;
		aExpr* obj;

	public:
		proceduredefine(symbolid nm, aExpr* ob, preplist* parameters, statementlist* slist)
			: functionunit(slist, parameters)
			, name(nm)
			, obj(ob)
			{ }

		virtual void debug(int ind);
		virtual void createTypes();
		virtual ~proceduredefine();

};

class methoddefineunit : public functionunit
{
	private:
		theExpr* name;
		aExpr* obj;


	public:
		methoddefineunit(theExpr* nm, aExpr* ob, preplist* parameters, statementlist* slist)
			: functionunit(slist, parameters)
			, name(nm)
			, obj(ob)
			{ }

		virtual void debug(int ind);
		virtual void createTypes();
		virtual ~methoddefineunit();

};

class constructordefine : public functionunit
{
	private:
		aExpr* obj;


	public:
		constructordefine(aExpr* ob, preplist* parameters, statementlist* slist)
			: functionunit(slist, parameters)
			, obj(ob)
			{ }

		virtual void debug(int ind);
		virtual void createTypes();
		virtual ~constructordefine();

};
