#include "symboltable.h"

symboltable::symboltable()
	: nextid(100) , suppresserrors(false)
{
	addSymbol("##run");
}

void symboltable::resetScope()
{
	// for now all we have to do is set allocated to false, but in the future we'll have to do more.
	vector<symtableentry*>::iterator i;
	for (i = symbols.begin() ; i != symbols.end() ; i++) {
		(*i)->allocated = false;
	}
}

int symboltable::addSymbol(const char* newsymbol)
{
	int id = nextid++;
	string s = string(newsymbol);
	suppresserrors = true;
	symtableentry* e = namelookup(s);
	suppresserrors = false;
	if (e) {
		return e->id;
	}
	symtableentry* ste = new symtableentry(id, s);
	symbols.push_back(ste);
	return ste->id;
}

symtableentry* symboltable::reglookup(registerid loc)
{
	vector<symtableentry*>::iterator i;
	for (i = symbols.begin() ; i != symbols.end() ; i++) {
		if ((*i)->memloc == loc) {
			return (*i);
		}
	}
	cout << "WARNING: symbol table lookup failed for register " << loc << endl;
	return 0;
}

// used for debugging right now.
symtableentry* symboltable::namelookup(string symbolname)
{
	vector<symtableentry*>::iterator i;
	for (i = symbols.begin() ; i != symbols.end() ; i++) {
		if ((*i)->name == symbolname) {
			return (*i);
		}
	}
	if (!suppresserrors) {
		cout << "WARNING: symbol table lookup failed for name " << symbolname << endl;
	}
	return 0;
}

// used for debugging right now.
symtableentry* symboltable::idlookup(int symbolid)
{
	vector<symtableentry*>::iterator i;
	for (i = symbols.begin() ; i != symbols.end() ; i++) {
		if ((*i)->id == symbolid) {
			return (*i);
		}
	}
	cout << "WARNING: symbol table lookup failed for id " << symbolid << endl;
	return 0;
}
