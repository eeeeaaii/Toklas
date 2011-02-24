#include "operations.h"
#include "memory.h"

/*

dream:

get called to go question some incarcerated criminals
go to prison -- meet with:
clock face guy with numbers up to 13 (from another dimension)
fiji water guy
scarecrow and tin man
cop is there with gun
fiji water guy tries to grab it but gets shot
water everywhere

steve storms:
845 216 4807
845 446 9192

*/



void operation::debug(bool sp)
{
	cout << (sp ? "            " : "") << debugstring() << endl;
}

void operation::debugSoFar()
{
	operation* p = this;
	cout << "?";
	while (p->previous) p = p->previous;
	while (p) {
		p->debug(false);
		p = p->next;
	}
}

operation* operation::append(operation* newop)
{
	newop->previous = this;
	newop->next = next;
	if (next) {
		next->previous = newop;
	}
	next = newop;
	return newop;
}

int operation::listcount()
{
	if (next == 0) return 1;
	else return 1 + next->listcount();
}

void operation::number()
{
	operation* p = this;
	int i = 0;
	while(p) {
		p->opnumber = i;
		p = p->next;
		i++;
	}
}


void operation::setDebugText(string c)
{
	debugtext = c;
}

operation::~operation()
{
	if (next) {
		delete next;
	}
}

string operation::debugNum()
{
	stringstream ss; 	
	ss 	<< setw(3) << setiosflags(ios::left) << opnumber;
	return ss.str();
}

string operation::debugReg(registerid reg)
{
	stringstream ss; 	
	ss << " " << setw(3) << setiosflags(ios::left) << mem.debugRegName(reg);
	return ss.str();
}
string operation::debugOffset(offset offs)
{
	stringstream ss;
	ss		<< " " << setw(3) << setiosflags(ios::left) << mem.debugRegName(offs.reg) << "(" << offs.off << ")";
	return ss.str();
}

string operation::debugR_RR(string name)
{
	stringstream ss; 	
	ss << debugNum() << debugReg(result) << " = " << name << debugReg(op1.reg) << debugReg(op2.reg);
	return ss.str();
}
string operation::debugR_R(string name)
{
	stringstream ss; 	
	ss << debugNum() << debugReg(result) << " = " << name << debugReg(op1.reg);
	return ss.str();
}
string operation::debugR_V(string name)
{
	stringstream ss; 	
	ss << debugNum() << debugReg(result) << " = " << name << " " << op1.value.toString();
	return ss.str();
}
string operation::debug_(string name)
{
	stringstream ss;
	ss << debugNum() << "       " << name;
	return ss.str();
}
string operation::debug_J(string name)
{
	stringstream ss;
	ss 	<< debugNum() << "       " << name << " ->" << op1.line->opnumber;
	return ss.str();
}
string operation::debug_R(string name) 
{
	stringstream ss;
	ss 	<< debugNum() << "       " << name << debugReg(op1.reg);
	return ss.str();
}
string operation::debugR_(string name)
{
	stringstream ss;
	ss 	<< debugNum() << debugReg(result) << " = " << name ;
	return ss.str();
}
string operation::debug_RO(string name)
{
	stringstream ss;
	ss 	<< debugNum() << "       " << name << debugReg(op1.reg) << debugOffset(op2.offs);
	return ss.str();
}
string operation::debugR_O(string name)
{
	stringstream ss;
	ss 	<< debugNum() << debugReg(result) << " = " << name << debugOffset(op1.offs);
	return ss.str();
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////

string op_add::debugstring() { return debugR_RR("add"); }
string op_sub::debugstring() { return debugR_RR("sub"); }
string op_mult::debugstring() { return debugR_RR("mult"); }
string op_div::debugstring() { return debugR_RR("div"); }
string op_mod::debugstring() { return debugR_RR("mod"); }

string op_addf::debugstring() { return debugR_RR("addf"); }
string op_subf::debugstring() { return debugR_RR("subf"); }
string op_multf::debugstring() { return debugR_RR("multf"); }
string op_divf::debugstring() { return debugR_RR("divf"); }

string op_lt::debugstring() { return debugR_RR("lt"); }
string op_lteq::debugstring() { return debugR_RR("lteq"); }
string op_eq::debugstring() { return debugR_RR("eq"); }
string op_and::debugstring() { return debugR_RR("and"); }
string op_or::debugstring() { return debugR_RR("or"); }

string op_move::debugstring() { return debugR_R("move"); }
string op_movei::debugstring() { return debugR_V("movei"); }

string op_start::debugstring() { return debug_("start"); }
string op_noop::debugstring() { return debug_("noop"); }
string op_end::debugstring() { return debug_("end"); }

string op_jump::debugstring() { return debug_J("jump"); }
string op_bnz::debugstring() { return debug_J("bnz"); }
string op_bz::debugstring() { return debug_J("bz"); }

string op_jumpreg::debugstring() { return debug_R("jumpreg"); }
string op_push::debugstring() { return debug_R("push"); }
string op_pop::debugstring() { return debugR_("pop"); }
string op_call::debugstring() { return debug_("call"); }
string op_alloc::debugstring() { return debugR_R("alloc"); }
string op_store::debugstring() { return debug_RO("store"); }
string op_load::debugstring() { return debugR_O("load"); }
string op_hstore::debugstring() { return debug_RO("hstore"); }
string op_hload::debugstring() { return debugR_O("hload"); }
string op_inc::debugstring() { return debug_R("inc"); }



//////////////////////////////////////////////////
//////////////////////////////////////////////////

operation* op_alloc::execute() {
	int size = mem.registerget(op1.reg).integer;
	int address = mem.allocate(size);
	mem.registerset(result, datum(address));
	return next;
}
operation* op_hload::execute() {
	int address = mem.registerget(op1.offs.reg).integer;
	address += op1.offs.off;
	datum val = mem.heapget(address);
	mem.registerset(result, val);
	return next;
}
operation* op_load::execute() {
	int address = mem.registerget(op1.offs.reg).integer;
	address += op1.offs.off;
	datum val = mem.stackget(address);
	mem.registerset(result, val);
	return next;
}
operation* op_hstore::execute() {
	datum val = mem.registerget(op1.reg);
	int address = mem.registerget(op2.offs.reg).integer;
	address += op2.offs.off;
	mem.heapset(address, val);
	return next;
}
operation* op_store::execute() {
	datum val = mem.registerget(op1.reg);
	int address = mem.registerget(op2.offs.reg).integer;
	address += op2.offs.off;
	mem.stackset(address, val);
	return next;
}
operation* op_push::execute() {
	int address = mem.registerget(SPREGID).integer;
	datum val = mem.registerget(op1.reg);
	mem.stackset(address, val);
	mem.registerset(SPREGID, address + 1);
	return next;
}
operation* op_pop::execute() {
	int address = mem.registerget(SPREGID).integer;
	datum val = mem.stackget(address);
	mem.registerset(result, val);
	mem.registerset(SPREGID, address - 1);
	return next;
}
operation* op_addf::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).fpoint + mem.registerget(op2.reg).fpoint));
	return next;
}
operation* op_subf::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).fpoint - mem.registerget(op2.reg).fpoint));
	return next;
}
operation* op_multf::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).fpoint * mem.registerget(op2.reg).fpoint));
	return next;
}
operation* op_divf::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).fpoint / mem.registerget(op2.reg).fpoint));
	return next;
}
operation* op_add::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).integer + mem.registerget(op2.reg).integer));
	return next;
}
operation* op_sub::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).integer - mem.registerget(op2.reg).integer));
	return next;
}
operation* op_mult::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).integer * mem.registerget(op2.reg).integer));
	return next;
}
operation* op_div::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).integer / mem.registerget(op2.reg).integer));
	return next;
}
operation* op_mod::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).integer % mem.registerget(op2.reg).integer));
	return next;
}
operation* op_lt::execute() {
	mem.registerset(result, datum((mem.registerget(op1.reg).integer < mem.registerget(op2.reg).integer) ? 1 : 0));
	return next;
}
operation* op_lteq::execute() {
	mem.registerset(result, datum((mem.registerget(op1.reg).integer <= mem.registerget(op2.reg).integer) ? 1 : 0));
	return next;
}
operation* op_eq::execute() {
	mem.registerset(result, datum((mem.registerget(op1.reg).integer == mem.registerget(op2.reg).integer) ? 1 : 0));
	return next;
}
operation* op_and::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).integer == 1 && mem.registerget(op2.reg).integer == 1));
	return next;
}
operation* op_or::execute() {
	mem.registerset(result, datum(mem.registerget(op1.reg).integer == 1 || mem.registerget(op2.reg).integer == 1));
	return next;
}
operation* op_move::execute() {
	datum d = mem.registerget(op1.reg);
	mem.registerset(result, d);
	return next;
}
operation* op_movei::execute() {
	mem.registerset(result, op1.value);
	return next;
}
operation* op_bz::execute() {
	if (mem.registerget(op1.reg).integer == 0) {
		return op2.line;
	} else {
		return next;
	}
}
operation* op_bnz::execute() {
	if (mem.registerget(op1.reg).integer != 0) {
		return op2.line;
	} else {
		return next;
	}
}
operation* op_inc::execute() {
	registerid r = op1.reg;
	datum v = mem.registerget(r);
	v = datum(v.integer + 1);
	mem.registerset(r, v);
	return next;
}
operation* op_jump::execute() {
	return op1.line;
}
operation* op_jumpreg::execute() {
	operation* line = (operation*)mem.registerget(op1.reg).loc;
	return line;
}
operation* op_call::execute() {
	mem.resetregdebug(); // this is used for debugging
	// before you push the return address onto the stack,
	// the framepointer needs to be set to the stack pointer.
	datum spval = mem.registerget(SPREGID);
	mem.registerset(FPREGID, spval);
	mem.stackset(spval.integer, datum((void*)next));
	mem.registerset(SPREGID, datum(spval.integer + 1));
	//memstack.top()->returnaddress = next;
	//memstack.push();
	return op1.line;
}
operation* op_start::execute() {
	return next;
}
operation* op_noop::execute() {
	return next;
}
operation* op_end::execute() {
	return 0;
}






