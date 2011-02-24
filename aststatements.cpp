#include "aststatements.h"

extern symboltable symtab;
extern memallocator alloc;

//
//
//
//////////// statementlist
//
//
//

void statementlist::debug(int ind)
{ 
	cout << indent(ind) << string("statementlist.") << endl;
	statements.debug(ind + 1);
}
void statementlist::semanticCheck() 
{
	for (statements.begin() ; !statements.atEnd() ; statements.gotoNext()) {
		statement* s = statements.getCurrent();
		// for now do the first one
		s->semanticCheck();
	}
}
registerid statementlist::generateCode(operation* &p)
{
	for (statements.begin() ; !statements.atEnd() ; statements.gotoNext()) {
		statement* s = statements.getCurrent();
		// for now do the first one
		s->generateCode(p);
	}
	return -1;
}

//
//
//
//////////////// letstatement
//
//
//

void letstatement::debug(int ind)
{ 
	cout << indent(ind) << "letstatement" << endl;
	lv->debug(ind + 1);
	value->debug(ind + 1);
}
void letstatement::semanticCheck() 
{
	// if the let statement is a simple assignment to a variable,
	// the variable doesn't have to be typed.
	// otherwise it does.
	lv->semanticCheck();
	value->semanticCheck();
	if (typeid(*lv) == typeid(varRef)) {
		varRef* vr = (varRef*)lv;
		symtableentry* ste = symtab.idlookup(vr->varname);
		type* tid = ste->datatype;
		if (tid == 0) {
			ste->datatype = value->datatype;
		} else if (tid != value->datatype) {
			cout << "previously typed identifier " << ste->name << " redefined." << endl;
			debug(0);
			exit(1);
		}
	} else {
		// in this case, just make sure the types are the same.
		if (lv->datatype != value->datatype) {
			cout << "previously typed variable redefined." << endl;
			debug(0);
			exit(1);
		}
	}
}

registerid letstatement::generateCode(operation* &p) {
	// the issue here is that the let statement could in fact be a return statement,
	// if the name we are "letting" to is the name of the function we are in.
	cout << "NOT IMPLEMENTED: letstatement::generatecode" << endl;
	exit(1);
	registerid valloc = value->generateCode(p);
	lv->generateStoreCode(valloc, p);
	return 0;
}

letstatement::~letstatement()
{
	delete lv;
	delete value;
}
//
//
//
////////////// dostatement
//
//
//

void dostatement::debug(int ind)
{ 
	cout << indent(ind) << "dostatement " << endl;
	allstatements->debug(ind + 1);
}

void dostatement::semanticCheck() 
{
	if (condexp) {
		condexp->semanticCheck();
	}
	allstatements->semanticCheck();
}
registerid dostatement::generateCode(operation* &p)
{
	// the dostatement code consists of
	// 1. cond/noop
	// 2. body
	// 3. jump/noop
	// 4. noop
	// where
	// 1. is either a noop, or code that tests the condition and jumps to 4 if it's not true
	// 2. is the statementlist
	// 3. is a jump back to 1. (if repeat) or not (if no repeat)
	// 4. is always a noop
	// 
	operation* jumpback;
	operation* end = new op_noop();
	if (condexp != 0) {
		operation* startpos = p;
		registerid testloc = condexp->generateCode(p);
		jumpback = startpos->next;
		operation *test = new op_bz(testloc, end);
		p = p->append(test);
	} else {
		jumpback = new op_noop();
		p = p->append(jumpback);
	}
	allstatements->generateCode(p);
	if (allstatements->repeats()) {
		operation *k = new op_jump(jumpback);
		p = p->append(k);
	}
	p = p->append(end);
	return 0; 
}

dostatement::~dostatement()
{
	delete allstatements;
	delete condexp;
}

//
//
//
//////////// choosestatement
//
//
// 




void choosestatement::debug(int ind)
{ 
	cout << indent(ind) << "choosestatement " << endl;
	allstatements->debug(ind + 1);
}
void choosestatement::semanticCheck() 
{
	allstatements->semanticCheck();
}
registerid choosestatement::generateCode(operation* &p)
{
	/*
	first I create a no-op to jump to after everything is done.
	then I generate the code for all do statements,
	each time saving the jump-in op pointer for that statement.
	after each do statement, I insert a hard jump to the no-op.
	finally, I extract the conditional from each do-statement
	and regenerate the code for that conditional.
	finally I insert those tests with jumps before the start of the dostatements
	
	
	test1
	jump to 1
	test2
	jump to 2
	test3
	jump to 3
	jump to end
	dostatement1
	jump to end
	dostatement2
	jump to end
	dostatement2
	jump to end
	end (noop)
	*/
	operation* end = new op_noop();
	operation* startp = p; // we are going to insert the conditions later
	astnodelist<statement*> nodelist = allstatements->getNodeList();
	vector<operation*> jumptos;
	for (nodelist.begin() ; !nodelist.atEnd() ; nodelist.gotoNext()) {
		// these all should be dostatements or we will have serious problems.
		dostatement* dost = (dostatement*)nodelist.getCurrent();
		operation* before = p;
		dost->generateCode(p);
		jumptos.push_back(before->next);
		operation* endjump = new op_jump(end);
		p = p->append(endjump);
	}
	p = p->append(end);
	// that's all good, now add the conditions.
	operation* newp = new op_noop();
	operation* anotherstart = newp;
	vector<operation*>::iterator jumpiter;
	for ( nodelist.begin() , jumpiter = jumptos.begin()
		 ; !nodelist.atEnd() , jumpiter != jumptos.end()
		 ; nodelist.gotoNext() , jumpiter++
		 ) {
		// these all should be dostatements or we will have serious problems.
		dostatement* dost = (dostatement*)nodelist.getCurrent();
		operation* jumpto = (*jumpiter);
		expression * test = dost->getConditional();
		if (test != 0) {
			registerid loc = test->generateCode(newp);
			// if loc is true, then branch to jumpto
			operation* testit = new op_bnz(loc, jumpto);
			newp = newp->append(testit);
		} else {
			// if there is no test, then we always jump.
			operation* alwaysjump = new op_jump(jumpto);
			newp = newp->append(alwaysjump);
		}
	}
	// if nothing tested true, we jump out of here
	operation* nothingjump = new op_jump(end);
	newp = newp->append(nothingjump);
	// ok now splice it in.
	operation *spn = startp->next;
	startp->next = anotherstart;
	anotherstart->previous = startp;
	newp->next = spn;
	spn->previous = newp;
	return 0;
}

choosestatement::~choosestatement()
{
	delete allstatements;
}

//
//
//
//////////// expressionStatement
//
//
// 
void expressionstatement::debug(int ind)
{
	cout << indent(ind) << "expressionstatement " << endl;
	expr->debug(ind + 1);
}
void expressionstatement::semanticCheck() 
{
	expr->semanticCheck();
}
registerid expressionstatement::generateCode(operation* &p)
{
	expr->generateCode(p);
	return 0;
}

expressionstatement::~expressionstatement()
{
	delete expr;
}

/*
//
//
//
//////////// returnstatement
//
//
// 

void returnstatement::debug(int ind)
{ 
	cout << indent(ind) << "returnstatement " << endl;
	value->debug(ind + 1);
}
void returnstatement::semanticCheck() 
{
	value->semanticCheck();
}
registerid returnstatement::generateCode(operation* &p)
{
	// return is really
	// set the stack pointer to be equal to the frame pointer
	// pop the return address off the stack
	// push the return value
	// then jump to the address
	//p = p->append(new op_return());
	registerid val = value->generateCode(p);
	registerid jumpto = alloc.getNextRegister();
	registerid sp = SPREGID;
	registerid fp = FPREGID;
	p = p->append(new op_move(sp, fp));
	p = p->append(new op_pop(jumpto));
	p = p->append(new op_push(val));
	p = p->append(new op_jumpreg(jumpto));
	return 0;
}

returnstatement::~returnstatement()
{
	delete t;
	delete value;
}

*/

//
//
//
//////////// callstatement
//
//
// 


void callstatement::debug(int ind)
{
	cout << indent(ind) << "callstatement (" << symtab.idlookup(n)->name << ")" << endl;
	e->debug(ind + 1);
}

registerid callstatement::generateCode(operation* &p)
{
	cout << "not implemented:callstatement" <<endl;
	exit(1);
	return 0;
}

void callstatement::semanticCheck()
{
	cout << "not implemented:callstatement" <<endl;
	exit(1);
}
