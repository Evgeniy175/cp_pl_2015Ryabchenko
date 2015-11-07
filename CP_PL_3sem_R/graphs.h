#pragma once

#define NUMBER_OF_GRAPHS 19

#define GRAPH_TYPE 14, \
	/* 0 */ NODE(5, RELATION('l', 1), RELATION('n', 4), RELATION('w', 6), RELATION('b', 9), RELATION('n', 12)),  \
	/* 1 */ NODE(1, RELATION('i', 2)), \
	/* 2 */ NODE(1, RELATION('n', 3)), \
	/* 3 */ NODE(1, RELATION('e', 14)), \
	/* 4 */ NODE(1, RELATION('u', 5)), \
	/* 5 */ NODE(1, RELATION('m', 14)), \
	/* 6 */ NODE(1, RELATION('a', 7)), \
	/* 7 */ NODE(1, RELATION('s', 8)), \
	/* 8 */ NODE(1, RELATION('h', 14)), \
	/* 9 */ NODE(1, RELATION('o', 10)), \
	/* 10*/ NODE(1, RELATION('o', 11)), \
	/* 11*/ NODE(1, RELATION('l', 14)), \
	/* 12*/ NODE(1, RELATION('i', 13)), \
	/* 13*/ NODE(1, RELATION('l', 14)), \
	/* 14*/ NODE()

#define GRAPH_FUNCTION 8, \
	/* 0 */ NODE(1, RELATION('f', 1)), \
	/* 1 */ NODE(1, RELATION('u', 2)), \
	/* 2 */ NODE(1, RELATION('n', 3)), \
	/* 3 */ NODE(1, RELATION('c', 4)), \
	/* 4 */ NODE(1, RELATION('t', 5)), \
	/* 5 */ NODE(1, RELATION('i', 6)), \
	/* 6 */ NODE(1, RELATION('o', 7)), \
	/* 7 */ NODE(1, RELATION('n', 8)), \
	/* 8 */ NODE()

#define GRAPH_RETURN 6, \
	/* 0 */ NODE(1, RELATION('r', 1)), \
	/* 1 */ NODE(1, RELATION('e', 2)), \
	/* 2 */ NODE(1, RELATION('t', 3)), \
	/* 3 */ NODE(1, RELATION('u', 4)), \
	/* 4 */ NODE(1, RELATION('r', 5)), \
	/* 5 */ NODE(1, RELATION('n', 6)), \
	/* 6 */ NODE()

#define GRAPH_BEGIN 5, \
	/* 0 */ NODE(1, RELATION('b', 1)), \
	/* 1 */ NODE(1, RELATION('e', 2)), \
	/* 2 */ NODE(1, RELATION('g', 3)), \
	/* 3 */ NODE(1, RELATION('i', 4)), \
	/* 4 */ NODE(1, RELATION('n', 5)), \
	/* 5 */ NODE()

#define GRAPH_SEMICOLON 1, \
	/* 0 */ NODE(1, RELATION(';', 1)), \
	/* 1 */ NODE()

#define GRAPH_COMMA 1, \
	/* 0 */ NODE(1, RELATION(',', 1)), \
	/* 1 */ NODE()

#define GRAPH_EQUALLY 1, \
	/* 0 */ NODE(1, RELATION('=', 1)), \
	/* 1 */ NODE()

#define GRAPH_OPEN_BRACE 1, \
	/* 0 */ NODE(1, RELATION('{', 1)), \
	/* 1 */ NODE()

#define GRAPH_CLOSE_BRACE 1, \
	/* 0 */ NODE(1, RELATION('}', 1)), \
	/* 1 */ NODE()

#define GRAPH_OPEN_PARENTHESIS 1, \
	/* 0 */ NODE(1, RELATION('(', 1)), \
	/* 1 */ NODE()

#define GRAPH_CLOSE_PARENTHESIS 1, \
	/* 0 */ NODE(1, RELATION(')', 1)), \
	/* 1 */ NODE()

#define GRAPH_OPEN_SQBRACE 1, \
	/* 0 */ NODE(1, RELATION('[', 1)), \
	/* 1 */ NODE()

#define GRAPH_CLOSE_SQBRACE 1, \
	/* 0 */ NODE(1, RELATION(']', 1)), \
	/* 1 */ NODE()

#define GRAPH_OPERATIONS 1, \
	/* 0 */ NODE(4, RELATION('+', 1), RELATION('-', 1), RELATION('*', 1), RELATION('/', 1)), \
	/* 1 */ NODE()

#define GRAPH_NEW_LINE 1,	\
	/* 0 */ NODE(1, RELATION('@', 1)), \
	/* 1 */ NODE()

#define GRAPH_CONDITION 2,	\
	/* 0 */ NODE(1, RELATION('i', 1)), \
	/* 1 */ NODE(1, RELATION('f', 2)), \
	/* 2 */ NODE()

#define GRAPH_ID 1, \
	/* 0 */ NODE(0, 1, "abcdefghijklmnopqrstuvwxyz_"), \
	/* 1 */ NODE()

#define GRAPH_LITERAL_STRING 3, \
	/* 0 */ NODE(1, RELATION('\‘', 1)), \
	/* 1 */ NODE(1, 2, "abcdefghijklmnopqrstuvwxyz. 0123456789"), \
	/* 2 */ NODE(1, RELATION('\’', 3)), \
	/* 3 */ NODE()

#define GRAPH_LITERAL_NUMBER 1, \
	/* 0 */ NODE(0, 1, "0123456789"), \
	/* 1 */ NODE()