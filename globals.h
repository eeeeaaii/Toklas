#pragma once

#ifdef UBUNTU
#include "stdio.h"
#include "stdlib.h"
#include <typeinfo>
#endif
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#define STACKSIZE 4096
#define HEAPSIZE 4096
#define NUMREGISTERS 256
#define SPREGID 0
#define FPREGID 1
#define FIRSTFREEREGID 2

#define DEBUGACTION(s) cout << s << endl;

// function types
#define FTYPE_PROCEDURE 0
#define FTYPE_CONSTRUCTOR 1
#define FTYPE_RUN 2
#define FTYPE_GETTER 3
#define FTYPE_SETTER 4

// operation types (for parser -- not an exact 1-1 mapping to opcodes)
#define OPTYPE_ADD   0
#define OPTYPE_SUB   1
#define OPTYPE_MULT  2
#define OPTYPE_DIV   3
#define OPTYPE_MOD   4
#define OPTYPE_AND   5
#define OPTYPE_OR    6
#define OPTYPE_LT    7
#define OPTYPE_LTE   8
#define OPTYPE_GT    9
#define OPTYPE_GTE   10
#define OPTYPE_EQ    11


using namespace std;

typedef long dataaddress;
typedef short registerid;
typedef int symbolid;



