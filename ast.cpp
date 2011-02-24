#include "ast.h"



string ast::indent(int lvl)
{
	string s = string("");
	for (int i = 0 ; i < lvl ; i++) {
		s += string("| ");
	}
	return s;
}

string ast::debugoptype(int optype)
{
	switch(optype) {
		case OPTYPE_ADD: return "+";
		case OPTYPE_SUB: return "-";
		case OPTYPE_MULT: return "*";
		case OPTYPE_DIV: return "/";
		case OPTYPE_MOD: return "%";
		case OPTYPE_AND: return "&&";
		case OPTYPE_OR: return "||";
		case OPTYPE_LT: return "<";
		case OPTYPE_LTE: return "<=";
		case OPTYPE_GT: return ">";
		case OPTYPE_GTE: return ">=";
		case OPTYPE_EQ: return "==";
	}
	return "";
}


