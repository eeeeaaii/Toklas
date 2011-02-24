#include "memory.h"



string datum::toString()
{
	char buf[100];
	if (type == "float") {
		sprintf(buf, "%f", fpoint);
		return string(buf);
	} else if (type == "int") {
		sprintf(buf, "%d", integer);
		return string(buf);
	} else {
		return string("ptr");
	}
}

int memory::allocate(int n)
{
	int r = heappointer;
	heappointer += n;
	return r;
}

// this is used for debugging record keeping
void memory::resetregdebug()
{
	for (int i = 0 ; i < NUMREGISTERS ; i++) {
		gets[i] = false;
		sets[i] = false;
	}
}

datum memory::stackget(int address)
{ 
	return stack[address];
}

void memory::stackset(int address, datum n)
{
	stack[address] = n;
}
/*
datum memory::stackpop()
{ 
	return stack[--stackpointer];
}

void memory::stackpush(datum n)
{
	stack[stackpointer++] = n;
}
*/

datum memory::registerget(registerid id)
{
	gets[id] = true;
	return registers[id];
}

void memory::registerset(registerid id, datum value)
{
	sets[id] = true;
	registers[id] = value;
}

datum memory::heapget(int address)
{
	return heap[address];
}

void memory::heapset(int address, datum value)
{
	heap[address] = value;
}


void memory::debugMemory()
{
	const char* sp = "                                    ";
	cout << sp << "registers: " << endl;
	for (registerid i = 0 ; i < NUMREGISTERS ; i++) {
		if (gets[i] || sets[i]) {
			string st;
			if (sets && gets) st == "(s, g)";
			else if (sets) st == "(s, )";
			else st == "( ,g)";
			cout
				<< sp << "  "
				<< debugRegName(i)
				<< " = " << registers[i].toString()
				<< "  " << st
				<< endl;
		}
	}
	cout << sp << "stack: " << endl;
	for (int i = 0 ; i < registers[SPREGID].integer ; i++) {
		cout << sp << i << "=" << stack[i].toString() << endl;
	}
}

string memory::debugRegName(registerid r)
{
	if (r == SPREGID) return "$SP";
	if (r == FPREGID) return "$FP";
	stringstream ss; 	
	ss << "$" << r;
	return ss.str();
}

