#include "astexpressions.h"

extern memallocator alloc;
extern symboltable symtab;


/*
hey.
everything in this language is an imperative.
what if there were other uses of language
that I could parse and that could become part of this.
of example
"are you going to normalize the vector?" search query
"I normalized the vector." adds data to a data store
rather than parsing language and then having some intermediary step
that maps it to some fake ruleset that you made up
(in other words, where the computer pretends to talk)

you could use language as a direct way to communicate with the machine
and then actually talk to it about what it's actually doing
or what needs to be done.
*/

//
//
//
////////////////// conditionalExpression
//
//
//

void conditionalExpression::debug(int ind)
{
	cout << indent(ind) << "conditionalExpression (" << debugoptype(ctype) << ")" << endl;
	leftOperand->debug(ind + 1);
	rightOperand->debug(ind + 1);
}

void conditionalExpression::semanticCheck()
{
	// determine datatype
	leftOperand->semanticCheck();
	rightOperand->semanticCheck();
	if (leftOperand->datatype->name != "int" || rightOperand->datatype->name != "int") {
		cout << "incompatible data types in:" << endl;
		debug(0);
		exit(1);
	}
	datatype = leftOperand->datatype;
}

registerid conditionalExpression::generateCode(operation* &p)
{
	// if it's >, subtract 2 from 1 and test for > 0
	// if it's =, also test = 0
	registerid reg1 = leftOperand->generateCode(p);
	registerid reg2 = rightOperand->generateCode(p);
	registerid rloc = alloc.getNextRegister();
	// you have to switch on the opcode I guess
	operation* n;
	switch(ctype) {
		case OPTYPE_AND: n = new op_and (rloc, reg1, reg2); break;
		case OPTYPE_OR:  n = new op_or  (rloc, reg1, reg2); break;
		case OPTYPE_LT:  n = new op_lt  (rloc, reg1, reg2); break;
		case OPTYPE_LTE: n = new op_lteq(rloc, reg1, reg2); break;
		case OPTYPE_GT:  n = new op_lt  (rloc, reg2, reg1); break; // note : reversed
		case OPTYPE_GTE: n = new op_lteq(rloc, reg2, reg1); break; // note : reversed
		case OPTYPE_EQ:  n = new op_eq  (rloc, reg1, reg2); break;
	}
	p = p->append(n);
	return rloc;
}

conditionalExpression::~conditionalExpression()
{
	delete leftOperand;
	delete rightOperand;
}

//
//
//
//////////////// theexpr
//
//
//

void theExpr::debug(int ind)
{
	if (sid != -1) {
		cout << indent(ind) << "theExpr (" << symtab.idlookup(sid)->name << ")" << endl;
	} else {
		cout << indent(ind) << "theExpr (ordinal)" << endl;
		ord->debug(ind + 1);
	}
}
registerid theExpr::generateCode(operation* &p)
{
	cout << "not implemented: theexpr" << endl;
	exit(1);
	return 0;
}
void theExpr::semanticCheck()
{
}

//
//
//
//////////////// literal
//
//
//

void literal::debug(int ind) { 
	cout << indent(ind) << "literal: " << value.toString() << endl;
}

void literal::semanticCheck()
{
	// datatype has already been set
}

registerid literal::generateCode(operation* &p)
{
	registerid loc = alloc.getNextRegister();
	operation *n = new op_movei(loc, value);
	p = p->append(n);
	return loc;
}

//
//
//
//////////////// binaryExpression
//
//
//

void binaryExpression::debug(int ind)
{ 
	cout << indent(ind) << "binaryExpression (" << debugoptype(optype) << ")" << endl;
	leftOperand->debug(ind + 1);
	rightOperand->debug(ind + 1);
}

void binaryExpression::semanticCheck()
{
	// determine datatype
	leftOperand->semanticCheck();
	rightOperand->semanticCheck();
	if (leftOperand->datatype != rightOperand->datatype) {
		cout << "incompatible data types in:" << endl;
		debug(0);
		cout << leftOperand->datatype << endl;
		cout << rightOperand->datatype << endl;
		exit(1);
	}
	datatype = leftOperand->datatype;
	if (datatype->name != "int" && datatype->name != "float") {
		cout << "invalid type for expression:" << endl;
		debug(0);
		cout << leftOperand->datatype << endl;
		cout << rightOperand->datatype << endl;
		exit(1);
	}
	if (datatype->name != "int" && optype == OPTYPE_MOD) {
		cout << "invalid type for expression:" << endl;
		debug(0);
		cout << leftOperand->datatype << endl;
		cout << rightOperand->datatype << endl;
		exit(1);
	}
}

registerid binaryExpression::generateCode(operation* &p)
{
	registerid reg1 = leftOperand->generateCode(p);
	registerid reg2 = rightOperand->generateCode(p);
	registerid rloc = alloc.getNextRegister();
	operation *n;
	// figure out opcode
	switch(optype) {
		case OPTYPE_ADD: {
			if (datatype->name == "int") {
				n = new op_add(rloc, reg1, reg2);
			} else {
				n = new op_addf(rloc, reg1, reg2);
			}
			break;
		}
		case OPTYPE_SUB: {
			if (datatype->name == "int") {
				n = new op_sub(rloc, reg1, reg2);
			} else {
				n = new op_subf(rloc, reg1, reg2);
			}
			break;
		}
		case OPTYPE_MULT: {
			if (datatype->name == "int") {
				n = new op_mult(rloc, reg1, reg2);
			} else {
				n = new op_multf(rloc, reg1, reg2);
			}
			break;
		}
		case OPTYPE_DIV: {
			if (datatype->name == "int") {
				n = new op_div(rloc, reg1, reg2);
			} else {
				n = new op_divf(rloc, reg1, reg2);
			}
			break;
		}
		case OPTYPE_MOD:  n = new op_mod(rloc, reg1, reg2); break;
	}
	p = p->append(n);
	return rloc;
}

binaryExpression::~binaryExpression()
{
	delete leftOperand;
	delete rightOperand;
}

//
//
//
//////////////// constructExpression
//
//
//

void constructExpression::debug(int ind)
{ 
	cout << indent(ind) << "constructExpression" <<  endl;
	t->debug(ind + 1);
	prepvals->debug(ind + 1);
}

void constructExpression::semanticCheck() 
{
}

registerid constructExpression::generateCode(operation* &p) {
	// when something is constructed, we have to allocate space
	// for it on the heap.   to do this we just advance the heap
	// pointer.  We will do garbage collection at some point later
	// and when we do that we'll compact the memory and fix references.
	// also see the article in delicious bookmarks about having a
	// "level 0, 1, and 2" heap.
	registerid r = alloc.getNextRegister();
	operation *n = new op_alloc(r, t->getSize());
	n->append(p);
	p = n;
	return r;
}


constructExpression::~constructExpression()
{
	delete t;
	if (prepvals) delete prepvals;
}

//
//
//
//////////////// call
//
//
//

void call::debug(int ind)
{ 
	cout << indent(ind) << "call" << endl;
	funcname->debug(ind + 1);
	if (objref) {
		objref->debug(ind + 1);
	}
	prepval->debug(ind + 1);
}

void call::semanticCheck() 
{
	// this function needs to set the datatype for the expression based on the
	// return value of the function being called.
	cout << "NOT YET IMPLEMENTED: call::semanticCheck" << endl;
	exit(1);
	// objref needs to evaluate to an object reference
	if (objref) {
		objref->semanticCheck();
		if (!objref->datatype->ptrtype) {
			cout << "error: of statement does not evaluate to an object reference." << endl;
			exit(1);
		}
	}
}

registerid call::generateCode(operation* &p)
{
	// look it up in the symbol table -- BUT what if the code has not been created yet?
	operation* entrypoint;
	if (objref) {
		// something like this: registerid objreg = objref->generateCode(p);
		// this gives you an address of a heap variable
		// find the type from the type tree which gives you the code to call
		// pass the address of the object as the first parameter to the function
		// i.e.
		// Car c = new Car();
		// c.drive(destination); // is really Car#drive(c, destination);
		//
		cout << "NOT YET IMPLEMENTED: call::generateCode (1)" << endl;
		exit(1);
	} else {
		// need to retrieve the code pointer from the type tree, not the symbol table
		cout << "also NOT YET IMPLEMENTED: call::generateCode (2)" << endl;
		exit(1);
		/*
		symtableentry* se = symtab.idlookup(funcname);
		operation* entrypoint = se->code;
		if (entrypoint == 0) {
			entrypoint = new op_start();
			se->code = entrypoint;
		}
		*/
	}
	// now you need to push all the active registers.
	registerid mr = alloc.getMaxRegister();
	for (registerid reg = FIRSTFREEREGID ; reg < mr ; reg++) {
		p = p->append(new op_push(reg));
	}
	p = p->append(new op_call(entrypoint));
	// the first thing you pop off the stack is the return value
	// it goes in a new register
	registerid rval = alloc.getNextRegister();
	p = p->append(new op_pop(rval));
	// the rest of the things you pop are saved registers
	for (registerid reg = (mr - 1) ; reg >= 0 ; reg--) {
		p = p->append(new op_pop(reg));
	}
	return rval;
}

call::~call()
{
	if (objref) delete objref;
	if (prepval) delete prepval;
}

//
//
//
//////////////// varRef
//
//
//

void varRef::debug(int ind) { 
	cout << indent(ind) << "varRef (" << symtab.idlookup(varname)->name << ")" << endl;
}

void varRef::semanticCheck() 
{
	symtableentry* ste = symtab.idlookup(varname);
	if (!ste) {
		// idk if this is possible or not
		cout << "unknown variable " << endl;
		exit(1);
	}
	datatype = ste->datatype;
}

registerid varRef::generateCode(operation* &p)
{
	// 
	registerid a = alloc.getNextRegister();
	symtableentry* e = symtab.idlookup(varname);
	int sloc = e->stackloc;
	if (sloc == -1) {
		cout << "uninitialized variable " << e->name << endl;
		exit(1);
	}
	registerid fp = FPREGID;
	p = p->append(new op_load(a, offset(fp, sloc)));
	return a;
}

void varRef::generateStoreCode(registerid r, operation* &p)
{
	// the value to be stored is in r...?
	registerid fp = FPREGID;
	registerid sp = SPREGID;
	int theoffset;
	symtableentry* e = symtab.idlookup(varname);
	bool doinc = false;
	if (e->stackloc != -1) {
		theoffset = e->stackloc;
	} else {
		theoffset = alloc.getNextStackOffset();
		e->stackloc = theoffset;
		doinc = true;
	}
	datum currentsp = mem.registerget(sp);
	p = p->append(new op_store(r, offset(fp, theoffset)));
	if (doinc) {
		p = p->append(new op_inc(sp));
	}
	/*
	registerid a = alloc.getNextRegister();
	registerid b = alloc.getNextRegister();
	operation* n1 = new op_movei(a, datum(1));
	operation* n2 = new op_alloc(b, a);
	operation* n3 = new op_store(r, offset(0, b));
	p->append(n1);
	n1->append(n2);
	n2->append(n3);
	p = n3;
	*/
}

//
//
//
//////////////// ordinal
//
//
//
void ordinal::debug(int ind) { 
	cout << indent(ind) << "ordinal" << endl;
	e->debug(ind + 1);
}

void ordinal::semanticCheck() 
{
}

registerid ordinal::generateCode(operation* &p)
{
	// not yet implemented
	cout << "not yet implemented." << endl;
	exit(1);
	return 0;
}

ordinal::~ordinal()
{
	delete e;
}

//
//
//
//////////////// arrayAccess
//
//
//

void arrayAccess::debug(int ind) { 
	cout << indent(ind) << "arrayAccess" << endl;
	n->debug(ind + 1);
	of->debug(ind + 1);
}

void arrayAccess::semanticCheck()
{
}

registerid arrayAccess::generateCode(operation* &p)
{
	printf("Error with extreme prejudice");
	exit(1);
	/*
	registerid a = of->addressOf(p); // needs to return the address of A though
	registerid b = n->generateCode(p); // generates the offset
	registerid c = alloc.getNextRegister();
	operation *p1 = new op_add(c, a, b);
	registerid d = alloc.getNextRegister();
	operation *p2 = new op_load(d, offset(0, c));
	p->append(p1); p = p1;
	p->append(p2); p = p2;
	return d;
	// 
	// to get the nth of A
	// LOAD $d offset($k)
	// 
	*/
}

arrayAccess::~arrayAccess()
{
	delete n;
	delete of;
}

//
//
//////////////////// prepositionval
//
//


void prepositionval::debug(int ind)
{
	cout << indent(ind) << "prepositionval" << endl;
	value->debug(ind + 1);
}

registerid prepositionval::generateCode(operation* &p)
{
	cout << "NOT IMPLEMENTED: prepositionval" << endl;
	exit(1);
	return -1;
}

void prepositionval::semanticCheck()
{
}

prepositionval::~prepositionval()
{
	delete value;
}

//
//
//////////////////// prepvallist
//
//


void prepvallist::debug(int ind)
{
	cout << indent(ind) << "prepvallist" << endl;
	preps.debug(ind + 1);
}

registerid prepvallist::generateCode(operation* &p)
{
	cout << "not implemented: prepvallist" << endl;
	exit(1);
	return -1;
}

void prepvallist::semanticCheck()
{
}


