#pragma once

#define NUMBER_OF_GRAPHS 20

#define GRAPH_TYPE 14, \
	/* 0 */ new NODE(5, new RELATION('l', 1), new RELATION('n', 4), new RELATION('w', 6), new RELATION('b', 9), new RELATION('n', 12)),  \
	/* 1 */ new NODE(1, new RELATION('i', 2)), \
	/* 2 */ new NODE(1, new RELATION('n', 3)), \
	/* 3 */ new NODE(1, new RELATION('e', 14)), \
	/* 4 */ new NODE(1, new RELATION('u', 5)), \
	/* 5 */ new NODE(1, new RELATION('m', 14)), \
	/* 6 */ new NODE(1, new RELATION('a', 7)), \
	/* 7 */ new NODE(1, new RELATION('s', 8)), \
	/* 8 */ new NODE(1, new RELATION('h', 14)), \
	/* 9 */ new NODE(1, new RELATION('o', 10)), \
	/* 10*/ new NODE(1, new RELATION('o', 11)), \
	/* 11*/ new NODE(1, new RELATION('l', 14)), \
	/* 12*/ new NODE(1, new RELATION('i', 13)), \
	/* 13*/ new NODE(1, new RELATION('l', 14)), \
	/* 14*/ new NODE()

#define GRAPH_FUNCTION 8, \
	/* 0 */ new NODE(1, new RELATION('f', 1)), \
	/* 1 */ new NODE(1, new RELATION('u', 2)), \
	/* 2 */ new NODE(1, new RELATION('n', 3)), \
	/* 3 */ new NODE(1, new RELATION('c', 4)), \
	/* 4 */ new NODE(1, new RELATION('t', 5)), \
	/* 5 */ new NODE(1, new RELATION('i', 6)), \
	/* 6 */ new NODE(1, new RELATION('o', 7)), \
	/* 7 */ new NODE(1, new RELATION('n', 8)), \
	/* 8 */ new NODE()

#define GRAPH_RETURN 6, \
	/* 0 */ new NODE(1, new RELATION('r', 1)), \
	/* 1 */ new NODE(1, new RELATION('e', 2)), \
	/* 2 */ new NODE(1, new RELATION('t', 3)), \
	/* 3 */ new NODE(1, new RELATION('u', 4)), \
	/* 4 */ new NODE(1, new RELATION('r', 5)), \
	/* 5 */ new NODE(1, new RELATION('n', 6)), \
	/* 6 */ new NODE()

#define GRAPH_BEGIN 5, \
	/* 0 */ new NODE(1, new RELATION('b', 1)), \
	/* 1 */ new NODE(1, new RELATION('e', 2)), \
	/* 2 */ new NODE(1, new RELATION('g', 3)), \
	/* 3 */ new NODE(1, new RELATION('i', 4)), \
	/* 4 */ new NODE(1, new RELATION('n', 5)), \
	/* 5 */ new NODE()

#define GRAPH_SEMICOLON 1, \
	/* 0 */ new NODE(1, new RELATION(';', 1)), \
	/* 1 */ new NODE()

#define GRAPH_COMMA 1, \
	/* 0 */ new NODE(1, new RELATION(',', 1)), \
	/* 1 */ new NODE()

#define GRAPH_COMPARE 2, \
	/* 0 */ new NODE(4, new RELATION('<', 1), new RELATION('>', 1), new RELATION('=', 1), new RELATION('!', 1)), \
	/* 1 */ new NODE(1, new RELATION('=', 2)), \
	/* 2 */ new NODE()

#define GRAPH_EQUALLY 1, \
	/* 0 */ new NODE(1, new RELATION('=', 1)), \
	/* 1 */ new NODE()

#define GRAPH_OPEN_BRACE 1, \
	/* 0 */ new NODE(1, new RELATION('{', 1)), \
	/* 1 */ new NODE()

#define GRAPH_CLOSE_BRACE 1, \
	/* 0 */ new NODE(1, new RELATION('}', 1)), \
	/* 1 */ new NODE()

#define GRAPH_OPEN_PARENTHESIS 1, \
	/* 0 */ new NODE(1, new RELATION('(', 1)), \
	/* 1 */ new NODE()

#define GRAPH_CLOSE_PARENTHESIS 1, \
	/* 0 */ new NODE(1, new RELATION(')', 1)), \
	/* 1 */ new NODE()

#define GRAPH_OPEN_SQBRACE 1, \
	/* 0 */ new NODE(1, new RELATION('[', 1)), \
	/* 1 */ new NODE()

#define GRAPH_CLOSE_SQBRACE 1, \
	/* 0 */ new NODE(1, new RELATION(']', 1)), \
	/* 1 */ new NODE()

#define GRAPH_OPERATIONS 1, \
	/* 0 */ new NODE(4, new RELATION('+', 1), new RELATION('-', 1), new RELATION('*', 1), new RELATION('/', 1)), \
	/* 1 */ new NODE()

#define GRAPH_NEW_LINE 1,	\
	/* 0 */ new NODE(1, new RELATION('@', 1)), \
	/* 1 */ new NODE()

#define GRAPH_CONDITION 2,	\
	/* 0 */ new NODE(1, new RELATION('i', 1)), \
	/* 1 */ new NODE(1, new RELATION('f', 2)), \
	/* 2 */ new NODE()

#define GRAPH_ID 1, \
	/* 0 */ new NODE(0, 1, "abcdefghijklmnopqrstuvwxyz_"), \
	/* 1 */ new NODE()

#define GRAPH_LITERAL_STRING 3, \
	/* 0 */ new NODE(1, new RELATION('‘', 1)), \
	/* 1 */ new NODE(1, 2, "abcdefghijklmnopqrstuvwxyz. 0123456789"), \
	/* 2 */ new NODE(1, new RELATION('’', 3)), \
	/* 3 */ new NODE()

#define GRAPH_LITERAL_NUMBER 1, \
	/* 0 */ new NODE(0, 1, "0123456789"), \
	/* 1 */ new NODE()