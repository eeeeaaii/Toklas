#include "type.h"


void property::debugProperty()
{
	cout << "    prop: " << name << " (a " << dtype->name << ")" << endl;
}

void method::debugMethod()
{
	if (rtype) {
		cout << "    function: " << name << " (returns a " << rtype->name << ")";
	} else {
		cout << "    procedure: " << name;
	}
	vector<type*>::iterator i;
	cout << "    params:" << endl;
	for (i = ptypes.begin() ; i != ptypes.end(); i++) {
		type* t = *i;
		cout << "      " << t->name << endl;
	}
}

void type::numberCode()
{
	vector<method*>::iterator i;
	for (i = methods.begin() ; i != methods.end(); i++) {
		method* t = *i;
		t->code->number();
	}
}

void type::debugCode()
{
	vector<method*>::iterator i;
	for (i = methods.begin() ; i != methods.end(); i++) {
		method* m = *i;
		cout << "---- " << m->name << " ----" << endl;
		operation* p = m->code;
		while(p) {
			p->debug(false);
			p = p->next;
		}
	}
}

void type::debugType()
{
	cout << "  type: " << name;
	if (superclass) {
		cout << " (extends " << superclass->name << ")";
	}
	cout << endl;
	vector<property*>::iterator pi;
	for (pi = props.begin() ; pi != props.end(); pi++) {
		property* p = *pi;
		p->debugProperty();
	}
	vector<method*>::iterator i;
	for (i = methods.begin() ; i != methods.end(); i++) {
		method* t = *i;
		t->debugMethod();
	}
}

method* type::getMethodByName(string name)
{
	vector<method*>::iterator i;
	for (i = methods.begin() ; i != methods.end(); i++) {
		method* t = *i;
		if (t->name == name) return t;
	}
	return 0;
}

property* type::getPropertyByName(string name)
{
	vector<property*>::iterator i;
	for (i = props.begin() ; i != props.end(); i++) {
		property* t = *i;
		if (t->name == name) return t;
	}
	return 0;
}

typelibrary::typelibrary()
{
//	type(string nm, type* sc, bool pt, bool ar, int sz);
	addType(new type("##global", 0, true, false, 0));
	addType(new type("int", 0, false, false, 1));
	addType(new type("float", 0, false, false, 1));
	addType(new type("int*", 0, true, false, 1));
	addType(new type("float*", 0, true, false, 1));
}

void typelibrary::debugCode()
{
	vector<type*>::iterator i;
	for (i = types.begin() ; i != types.end(); i++) {
		type* t = *i;
		t->debugCode();
	}
}

void typelibrary::numberCode()
{
	vector<type*>::iterator i;
	for (i = types.begin() ; i != types.end(); i++) {
		type* t = *i;
		t->numberCode();
	}
}

void typelibrary::debugTypes()
{
	cout << "type library:" << endl;
	vector<type*>::iterator i;
	for (i = types.begin() ; i != types.end(); i++) {
		type* t = *i;
		t->debugType();
	}
}


void typelibrary::addType(type* t)
{
	if (!getTypeByName(t->name)) {
		types.push_back(t);
	} else {
		cout << "error: type already added." << endl;
		exit(1);
	}
}


type* typelibrary::getTypeByName(string tname)
{
	vector<type*>::iterator i;
	for (i = types.begin() ; i != types.end() ; i++) {
		type* t = *i;
		if (t->name == tname) {
			return t;
		}
	}
	return 0;
}

type* typelibrary::getOrCreateType(string tname)
{
	type* t = getTypeByName(tname);
	if (!t) {
		t = new type(tname, 0, true, false, 1);
		addType(t);
	}
	return t;
}

