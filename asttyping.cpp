#include "asttyping.h"

extern symboltable symtab;
extern typelibrary typelib;

//
//
//////////////////// aExpr
//
//

aExpr::aExpr(string id)
{
	sid = symtab.namelookup(id)->id;
}

void aExpr::debug(int ind)
{
	cout << indent(ind) << "aexpr (" << symtab.idlookup(sid)->name << ")" << endl;
}
registerid aExpr::generateCode(operation* &p)
{
	cout << "not implementeddddd." << endl;
	exit(1);
	return 0;
}
void aExpr::semanticCheck()
{
}
//
//
//////////////////// preposition
//
//

void preposition::debug(int ind)
{
	cout << indent(ind) << "preposition (" << symtab.idlookup(name)->name << ")" << endl;
	if (atype) atype->debug(ind + 1);
}

registerid preposition::generateCode(operation* &p)
{
	cout << "NOT IMPLEMENTED: preposition" << endl;
	exit(1);
	return -1;
}

void preposition::semanticCheck()
{
}

preposition::~preposition()
{
	if (atype) delete atype;
}

//
//
//////////////////// preplist
//
//


void preplist::debug(int ind)
{
	cout << indent(ind) << "preplist" << endl;
	preps.debug(ind + 1);
}

registerid preplist::generateCode(operation* &p)
{
	cout << "not implemented: preplist" << endl;
	exit(1);
	return -1;
}

void preplist::semanticCheck()
{
}

vector<type*> preplist::getTypes()
{
	vector<type*> r;

	for (preps.begin() ; !preps.atEnd(); preps.gotoNext())
	{
		preposition* p = preps.getCurrent();
		symbolid n = p->getName();
		string namestr = symtab.idlookup(n)->name;
		r.push_back(typelib.getOrCreateType(namestr));
	}
	return r;
}

//
//
//////////////////// arrayAExpr
//
//


arrayAExpr::~arrayAExpr()
{
	delete t;
}

