#include "astprogram.h"

extern symboltable symtab;
extern memallocator alloc;
extern typelibrary typelib;

void program::debug(int ind)
{
	cout << indent(ind) << string("program") << endl;
	defs.debug(ind + 1);
}

void program::semanticCheck() 
{
	for (defs.begin() ; !defs.atEnd() ; defs.gotoNext()) {
		unit* u = defs.getCurrent();
		u->semanticCheck();
	}
}

void program::createTypes()
{
	for (defs.begin() ; !defs.atEnd() ; defs.gotoNext()) {
		unit* u = defs.getCurrent();
		u->createTypes();
	}
}

void program::generateCode()
{
	for (defs.begin() ; !defs.atEnd() ; defs.gotoNext()) {
		unit* u = defs.getCurrent();
		operation* unused;
		u->generateCode(unused);
	}
	/*
		registerid generateCode(operation* &p);

		alloc.reset();
		symtab.resetScope();
		unit* u = defs.getCurrent();
		operation* z;
		u->generateCode(z);
		type* gtype = typelib.getTypeByName("##global");
		method* m = new method(u->getName(), z, 0, vector<type*>());
		gtype->addMethod(m);
	}
	*/
}

