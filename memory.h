#pragma once
#include "globals.h"


// this should really be a union but then I can't really debug it effectively
// so it will be a struct for the time being.
struct datum
{
	int integer;
	double fpoint;
	void* loc;

	// for debugging
	string type;

	datum() {
	}

	datum(int i) {
		integer = i;
		type = "int";
	}

	datum(double d) {
		fpoint = d;
		type = "Float";
	}

	datum(void* op) {
		loc = op;
		type = "address";
	}

	string toString();
};

class memory
{
	private:
		datum *registers;
		bool* gets;
		bool* sets;
		datum *heap;
		int heappointer;
		datum *stack;
	public:

		memory()
			: registers(new datum[NUMREGISTERS])
			, gets(new bool[NUMREGISTERS])
			, sets(new bool[NUMREGISTERS])
			, heap(new datum[HEAPSIZE])
			, heappointer(0)
			, stack(new datum[STACKSIZE])
			{
				registerset(SPREGID, datum(0));
				registerset(FPREGID, datum(0));
			}

		/*
		datum stackpop();
		void stackpush(datum n);
		*/
		datum stackget(int address);
		void stackset(int address, datum value);
		datum registerget(registerid id);
		void registerset(registerid id, datum value);
		void debugMemory();
		void resetregdebug();
		int allocate(int n);
		datum heapget(int address);
		void heapset(int address, datum value);
		string debugRegName(registerid r);
};




