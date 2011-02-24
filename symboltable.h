#pragma once
#include "operations.h"
#include "type.h"

/*
it doesn't make sense to store the code pointer in the symbol table.
the same symbol could point to different code, for example:
var x = function() {return 2;}
x = function() {return 4;}
code is a VALUE for the symbol.
*/

class memallocator
{
	private:
		registerid currentreg;
		int stackoffset;
	public:
		memallocator()
			: currentreg(FIRSTFREEREGID)
			, stackoffset(0)
			{ }
		registerid getNextStackOffset()
			{ return stackoffset++; }
		registerid getNextRegister()
			{ return currentreg++; }
		void reset()
			{ currentreg = FIRSTFREEREGID; stackoffset = 0; }
		registerid getMaxRegister()
			{ return currentreg; }
		registerid getMaxOffset()
			{ return stackoffset; }
};



class symtableentry
{
	public:
		int id;
		string name;
		type* datatype;

		bool pointertype;
//		operation* code; // removed in order to find all references
		bool allocated;
		int stackloc;
		registerid memloc;

		symtableentry(int nid, string nm)
			: id(nid)
			, name(nm)
			, datatype(0)
			, pointertype(false)
//			, code(0) // removed in order to find all references
			, allocated(false)
			, stackloc(-1)
			, memloc(-1)
			{ }
};

class symboltable
{
	private:
		vector<symtableentry*> symbols;
		int nextid;
		bool suppresserrors;
		int addSymbolAt(const char* newsymbol, int id);

	public:
		symboltable();

		int addSymbol(const char* newsymbol);
		symtableentry* reglookup(registerid loc);
		symtableentry* namelookup(string symbolname);
		symtableentry* idlookup(int symbolid);
		void resetScope();
};

