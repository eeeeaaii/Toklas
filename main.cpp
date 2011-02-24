#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "astprogram.h"

extern FILE *yyin;
extern int yyparse();

// all global objects
memallocator alloc;
memory mem;
symboltable symtab;
program* parsed_program;
typelibrary typelib;



bool stage_parse;
bool stage_check;
bool stage_generate;
bool stage_run;
bool debugit;
string filename = string("");

void die()
{
	printf("usage: toklas [-P | -C | -G] [-V] <file>\n");
	printf("  where flags are:\n");
	printf("  -P   parse only\n");
	printf("  -C   parse and check semantics only\n");
	printf("  -G   stop after generating code\n");
	printf("  -V   print out debugging information\n");
	exit(1);
}

void getOptions(int argc, char** argv)
{
	if (argc < 2) {
		die();
	}
	stage_parse = stage_check = stage_generate = stage_run = true;
	debugit = false;
	bool foundastage = false;
	for (int i = 1 ; i< argc ; i++) {
		if (string(argv[i]) == "-P") {
			if (foundastage) die();
			foundastage = true;
			stage_check = false;
			stage_generate = false;
			stage_run = false;
		} else if (string(argv[i]) == "-C") {
			if (foundastage) die();
			foundastage = true;
			stage_generate = false;
			stage_run = false;
		} else if (string(argv[i]) == "-G") {
			if (foundastage) die();
			foundastage = true;
			stage_run = false;
		} else if (string(argv[i]) == "-V") {
			debugit =  true;
		} else {
			filename = string(argv[i]);
		}
	}
	if (filename == "") die();
}

int main(int argc, char** argv)
{
	getOptions(argc, argv);
	if (stage_parse) {
		if (debugit) cout << "parsing..." << endl;
		yyin = fopen(filename.c_str(), "r");
		int error = yyparse();
		if (error) {
			cout << "parse error." << endl;
			exit(1);
		}
		if (debugit) {
			cout << "output from parse:" << endl;
			parsed_program->debug(0);
		}
	}
	if (stage_check) {
		if (debugit) cout << "checking semantics..." << endl;
		parsed_program->createTypes();
		typelib.debugTypes();
		parsed_program->semanticCheck();
	}
	if (stage_generate) {
		if (debugit) cout << "generating code..." << endl;
		parsed_program->generateCode();
		typelib.numberCode();
		if (debugit) {
			cout << "generated program was:" << endl;
			typelib.debugCode();
		}
	}
	if (stage_run) {
		if (debugit) cout << "running program..." << endl;
		operation* pc;
		type* gt = typelib.getTypeByName("##global");
		method* gtm = gt->getMethodByName("##run");
		pc = gtm->code;
		while((pc = pc->execute())) {
			if (debugit) {
				mem.debugMemory();
				pc->debug(true);
			}
		}
		if (debugit) mem.debugMemory();
	}
	exit(0);
}

