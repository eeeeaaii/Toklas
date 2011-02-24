#pragma once
#include "astunits.h"


class program : public ast
{
	private:
		astnodelist<unit*> defs;

	public:
		program()
			{ };

		void addUnit(unit* u)
			{ defs.add(u); }

		virtual void debug(int ind);
		virtual void generateCode();
		virtual void semanticCheck();
		virtual void createTypes();
};
 
