#pragma once
#include "memory.h"


extern memory mem;

class operation;

struct offset {
	registerid reg;
	int off;

	offset(registerid _reg, int _off)
		: reg(_reg)
		, off(_off)
		{ }

	offset()
		{ }
};

struct operand {
	registerid reg;
	datum value;
	operation* line;
	offset offs;

	operand()
		: reg(-1)
		{ }

	operand(registerid nreg)
		: reg(nreg)
		{ }

	operand(datum nval)
		: value(nval)
		{ }

	operand(operation* nline)
		: line(nline)
		{ }

	operand(offset _offs)
		: offs(_offs)
		{ }
};

class operation
{
	public:
		int opnumber;
		registerid result;
		operand op1;
		operand op2;
		operation* next;
		operation* previous;
		string debugtext;

		operation(registerid _r, operand _op1, operand _op2)
			: result(_r)
			, op1(_op1)
			, op2(_op2)
			, next(0)
			, previous(0)
			{ }

		operation(registerid _r, operand _op1)
			: result(_r)
			, op1(_op1)
			, next(0)
			, previous(0)
			{ }

		operation(registerid _r)
			: result(_r)
			, next(0)
			, previous(0)
			{ }

		operation()
			: result(-1)
			, next(0)
			, previous(0)
			{ }

		virtual operation* execute() = 0;
		virtual string debugstring() = 0;
		virtual ~operation();

		void number();
		operation* append(operation* newop);
		void debug(bool sp);
		void debugSoFar();
		int listcount();
		void setDebugText(string c);

		string debugNum();
		string debugReg(registerid reg);
		string debugOffset(offset offs);
		string debugR_RR(string name);
		string debugR_R(string name);
		string debugR_V(string name);
		string debug_(string name);
		string debug_J(string name);
		string debug_R(string name);
		string debugR_(string name);
		string debug_RO(string name);
		string debugR_O(string name);

};


class op_noop : public operation {
	public:
	operation* execute();
	string debugstring();
	op_noop()
		: operation()
		{ }
};
class op_add : public operation {
	public:
	operation* execute();
	string debugstring();
	op_add(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_sub : public operation {
	public:
	operation* execute();
	string debugstring();
	op_sub(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_mult : public operation {
	public:
	operation* execute();
	string debugstring();
	op_mult(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_div : public operation {
	public:
	operation* execute();
	string debugstring();
	op_div(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_mod : public operation {
	public:
	operation* execute();
	string debugstring();
	op_mod(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_move : public operation {
	public:
	operation* execute();
	string debugstring();
	op_move(registerid r, registerid op)
		: operation(r, operand(op))
		{ }
};
class op_movei : public operation {
	public:
	operation* execute();
	string debugstring();
	op_movei(registerid r, datum op)
		: operation(r, operand(op))
		{ }
};
class op_end : public operation {
	public:
	operation* execute();
	string debugstring();
};
class op_start : public operation {
	public:
	operation* execute();
	string debugstring();
	op_start()
		: operation()
		{ }
};
class op_jump : public operation {
	public:
	operation* execute();
	string debugstring();
	op_jump(operation* op)
		: operation(0, operand(op))
		{ }
};
class op_lt : public operation {
	public:
	operation* execute();
	string debugstring();
	op_lt(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_lteq : public operation {
	public:
	operation* execute();
	string debugstring();
	op_lteq(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_eq : public operation {
	public:
	operation* execute();
	string debugstring();
	op_eq(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_and : public operation {
	public:
	operation* execute();
	string debugstring();
	op_and(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_or : public operation {
	public:
	operation* execute();
	string debugstring();
	op_or(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_bnz : public operation {
	public:
	operation* execute();
	string debugstring();
	op_bnz(registerid op1, operation* op2)
		: operation(0, operand(op1), operand(op2))
		{ }
};
class op_bz : public operation {
	public:
	operation* execute();
	string debugstring();
	op_bz(registerid op1, operation* op2)
		: operation(0, operand(op1), operand(op2))
		{ }
};
class op_push : public operation {
	public:
	operation* execute();
	string debugstring();
	op_push(registerid op)
		: operation(0, operand(op))
		{ }
};
class op_pop : public operation {
	public:
	operation* execute();
	string debugstring();
	op_pop(registerid r)
		: operation(r)
		{ }
};
class op_call : public operation {
	public:
	operation* execute();
	string debugstring();
	op_call(operation* op)
		: operation(0, operand(op))
		{ }
};
class op_jumpreg : public operation {
	public:
	operation* execute();
	string debugstring();
	op_jumpreg(registerid op)
		: operation(0, operand(op))
		{ }
};
class op_addf : public operation {
	public:
	operation* execute();
	string debugstring();
	op_addf(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_subf : public operation {
	public:
	operation* execute();
	string debugstring();
	op_subf(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_multf : public operation {
	public:
	operation* execute();
	string debugstring();
	op_multf(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_divf : public operation {
	public:
	operation* execute();
	string debugstring();
	op_divf(registerid r, registerid o1, registerid o2)
		: operation(r, operand(o1), operand(o2))
		{ }
};
class op_alloc : public operation {
	public:
	operation* execute();
	string debugstring();
	op_alloc(registerid r, registerid op)
		: operation(r, operand(op))
		{ }
};
class op_load : public operation {
	public:
	operation* execute();
	string debugstring();
	op_load(registerid r, offset op1)
		: operation(r, operand(op1))
		{ }
};
class op_store : public operation {
	public:
	operation* execute();
	string debugstring();
	op_store(registerid op1, offset op2)
		: operation(0, operand(op1), operand(op2))
		{ }
};
class op_hload : public operation {
	public:
	operation* execute();
	string debugstring();
	op_hload(registerid r, offset op1)
		: operation(r, operand(op1))
		{ }
};
class op_hstore : public operation {
	public:
	operation* execute();
	string debugstring();
	op_hstore(registerid op1, offset op2)
		: operation(0, operand(op1), operand(op2))
		{ }
};
class op_inc : public operation {
	public:
	operation* execute();
	string debugstring();
	op_inc(registerid op1)
		: operation(0, operand(op1))
		{ }
};


