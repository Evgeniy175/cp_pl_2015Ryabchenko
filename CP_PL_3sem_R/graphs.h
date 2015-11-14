#pragma once

#define NUMBER_OF_GRAPHS 20

#define GRAPH_TYPE 14, \
	/* 0 */ new FST::NODE(5, new FST::RELATION('l', 1), new FST::RELATION('n', 4), new FST::RELATION('w', 6), new FST::RELATION('b', 9), new FST::RELATION('n', 12)),  \
	/* 1 */ new FST::NODE(1, new FST::RELATION('i', 2)), \
	/* 2 */ new FST::NODE(1, new FST::RELATION('n', 3)), \
	/* 3 */ new FST::NODE(1, new FST::RELATION('e', 14)), \
	/* 4 */ new FST::NODE(1, new FST::RELATION('u', 5)), \
	/* 5 */ new FST::NODE(1, new FST::RELATION('m', 14)), \
	/* 6 */ new FST::NODE(1, new FST::RELATION('a', 7)), \
	/* 7 */ new FST::NODE(1, new FST::RELATION('s', 8)), \
	/* 8 */ new FST::NODE(1, new FST::RELATION('h', 14)), \
	/* 9 */ new FST::NODE(1, new FST::RELATION('o', 10)), \
	/* 10*/ new FST::NODE(1, new FST::RELATION('o', 11)), \
	/* 11*/ new FST::NODE(1, new FST::RELATION('l', 14)), \
	/* 12*/ new FST::NODE(1, new FST::RELATION('i', 13)), \
	/* 13*/ new FST::NODE(1, new FST::RELATION('l', 14)), \
	/* 14*/ new FST::NODE()

#define GRAPH_FUNCTION 8, \
	/* 0 */ new FST::NODE(1, new FST::RELATION('f', 1)), \
	/* 1 */ new FST::NODE(1, new FST::RELATION('u', 2)), \
	/* 2 */ new FST::NODE(1, new FST::RELATION('n', 3)), \
	/* 3 */ new FST::NODE(1, new FST::RELATION('c', 4)), \
	/* 4 */ new FST::NODE(1, new FST::RELATION('t', 5)), \
	/* 5 */ new FST::NODE(1, new FST::RELATION('i', 6)), \
	/* 6 */ new FST::NODE(1, new FST::RELATION('o', 7)), \
	/* 7 */ new FST::NODE(1, new FST::RELATION('n', 8)), \
	/* 8 */ new FST::NODE()

#define GRAPH_RETURN 6, \
	/* 0 */ new FST::NODE(1, new FST::RELATION('r', 1)), \
	/* 1 */ new FST::NODE(1, new FST::RELATION('e', 2)), \
	/* 2 */ new FST::NODE(1, new FST::RELATION('t', 3)), \
	/* 3 */ new FST::NODE(1, new FST::RELATION('u', 4)), \
	/* 4 */ new FST::NODE(1, new FST::RELATION('r', 5)), \
	/* 5 */ new FST::NODE(1, new FST::RELATION('n', 6)), \
	/* 6 */ new FST::NODE()

#define GRAPH_BEGIN 5, \
	/* 0 */ new FST::NODE(1, new FST::RELATION('b', 1)), \
	/* 1 */ new FST::NODE(1, new FST::RELATION('e', 2)), \
	/* 2 */ new FST::NODE(1, new FST::RELATION('g', 3)), \
	/* 3 */ new FST::NODE(1, new FST::RELATION('i', 4)), \
	/* 4 */ new FST::NODE(1, new FST::RELATION('n', 5)), \
	/* 5 */ new FST::NODE()

#define GRAPH_SEMICOLON 1, \
	/* 0 */ new FST::NODE(1, new FST::RELATION(';', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_COMMA 1, \
	/* 0 */ new FST::NODE(1, new FST::RELATION(',', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_COMPARE 2, \
	/* 0 */ new FST::NODE(6, new FST::RELATION('<', 1), new FST::RELATION('<', 2), new FST::RELATION('>', 1), new FST::RELATION('>', 2), new FST::RELATION('=', 1), new FST::RELATION('!', 1)), \
	/* 1 */ new FST::NODE(1, new FST::RELATION('=', 2)), \
	/* 2 */ new FST::NODE()

#define GRAPH_EQUAL 1, \
	/* 0 */ new FST::NODE(1, new FST::RELATION('=', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_OPEN_BRACE 1, \
	/* 0 */ new FST::NODE(1, new FST::RELATION('{', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_CLOSE_BRACE 1, \
	/* 0 */ new FST::NODE(1, new FST::RELATION('}', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_OPEN_PARENTHESIS 1, \
	/* 0 */ new FST::NODE(1, new FST::RELATION('(', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_CLOSE_PARENTHESIS 1, \
	/* 0 */ new FST::NODE(1, new FST::RELATION(')', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_OPEN_SQBRACE 1, \
	/* 0 */ new FST::NODE(1, new FST::RELATION('[', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_CLOSE_SQBRACE 1, \
	/* 0 */ new FST::NODE(1, new FST::RELATION(']', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_OPERATIONS 1, \
	/* 0 */ new FST::NODE(4, new FST::RELATION('+', 1), new FST::RELATION('-', 1), new FST::RELATION('*', 1), new FST::RELATION('/', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_NEW_LINE 1,	\
	/* 0 */ new FST::NODE(1, new FST::RELATION('@', 1)), \
	/* 1 */ new FST::NODE()

#define GRAPH_CONDITION 2,	\
	/* 0 */ new FST::NODE(1, new FST::RELATION('i', 1)), \
	/* 1 */ new FST::NODE(1, new FST::RELATION('f', 2)), \
	/* 2 */ new FST::NODE()

#define GRAPH_IDENTIFICATOR 1, \
	/* 0 */ new FST::NODE(0, 1, "abcdefghijklmnopqrstuvwxyz_"), \
	/* 1 */ new FST::NODE()

#define GRAPH_LITERAL_LINE 3, \
	/* 0 */ new FST::NODE(1, new FST::RELATION('‘', 1)), \
	/* 1 */ new FST::NODE(1, 2, "abcdefghijklmnopqrstuvwxyz. 0123456789"), \
	/* 2 */ new FST::NODE(1, new FST::RELATION('’', 3)), \
	/* 3 */ new FST::NODE()

#define GRAPH_LITERAL_NUMBER 1, \
	/* 0 */ new FST::NODE(0, 1, "0123456789"), \
	/* 1 */ new FST::NODE()