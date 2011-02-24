#pragma once
#include "globals.h"
#include "operations.h"


class type;

class property
{
	public:
	string name;
	type* dtype;

	property(string n, type* dt)
		: name(n)
		, dtype(dt)
		{ }
	void debugProperty();
};

class method
{
	public:
	string name;
	operation* code;
	type* rtype;
	vector<type*> ptypes;

	method(string n, type* r, vector<type*> p)
		: name(n)
		, code(new op_start())
		, rtype(r)
		, ptypes(p)
		{ }
	void debugMethod();
};

class type
{
	public:
	string name;
	type* superclass;
	bool ptrtype;  // if it is an object reference, i.e. can have methods assoc with it
	bool arraytype;
	int size; // in bytes
	vector<property*> props;
	vector<method*> methods;

	type(string nm, type* sc, bool pt, bool ar, int sz)
		: name(nm)
		, superclass(sc)
		, ptrtype(pt)
		, arraytype(ar)
		, size(sz)
		{ }
	void addMethod(method* m)
		{ methods.push_back(m); }
	void addProperty(property* p)
		{ props.push_back(p); }
	void numberCode();
	void debugCode();
	void debugType();
	method* getMethodByName(string name);
	property* getPropertyByName(string name);

};


class typelibrary
{
	public:
	vector<type*> types;

	typelibrary();
	void addType(type* t);
	type* getTypeByName(string tname);
	void numberCode();
	void debugCode();
	void debugTypes();
	type* getOrCreateType(string tname);
};
