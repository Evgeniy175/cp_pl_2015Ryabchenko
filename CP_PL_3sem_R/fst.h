#pragma once

#include "graphs.h"
#include <iostream>
#include <vector>

#define FST_ARRAY {\
	FST::FST(NULL_STR, GRAPH_TYPE),\
	FST::FST(NULL_STR, GRAPH_NEW_LINE), \
	FST::FST(NULL_STR, GRAPH_SEMICOLON),\
	FST::FST(NULL_STR, GRAPH_COMMA),\
	FST::FST(NULL_STR, GRAPH_COMPARE),\
	FST::FST(NULL_STR, GRAPH_EQUAL),\
	FST::FST(NULL_STR, GRAPH_OPEN_BRACE),\
	FST::FST(NULL_STR, GRAPH_CLOSE_BRACE),\
	FST::FST(NULL_STR, GRAPH_OPEN_PARENTHESIS),\
	FST::FST(NULL_STR, GRAPH_CLOSE_PARENTHESIS),\
	FST::FST(NULL_STR, GRAPH_OPEN_SQBRACE),\
	FST::FST(NULL_STR, GRAPH_CLOSE_SQBRACE),\
	FST::FST(NULL_STR, GRAPH_OPERATIONS),\
	FST::FST(NULL_STR, GRAPH_CONDITION),\
	FST::FST(NULL_STR, GRAPH_FUNCTION),\
	FST::FST(NULL_STR, GRAPH_RETURN),\
	FST::FST(NULL_STR, GRAPH_BEGIN),\
	FST::FST(NULL_STR, GRAPH_ID),\
	FST::FST(NULL_STR, GRAPH_LITERAL_STRING),\
	FST::FST(NULL_STR, GRAPH_LITERAL_NUMBER)\
}

namespace FST{
	class RELATION{
	public:
		RELATION(
			char symbol = 0x00,
			short nextNode = NULL);

		char							getSymbol();					// return symbol that allow jump to a next node
		short							getNextNode();					// return next node number

	private:
		char							symbol_;						// symbol that allow jump to a next node
		short							nextNode_;						// next node number
	};

	class NODE{
	public:
		NODE();
		NODE(
			short numOfRel,
			RELATION* relation, ...);
		NODE(
			short firstRelation,
			short secondRelation,
			char* str
			);

		short							getNumberOfRel();				// number of relations in this node
		RELATION*						getRelation(int index);			// return relation using index

		~NODE();

	private:
		short							numOfRel_;						// number of relations
		std::vector<RELATION*>			relations_;						// vector if relations
	};

	class FST{
	public:
		FST();
		FST(
			char* str,
			short numberOfStates,
			NODE* node, ...);

		void							setString(char* str);
		bool							execute();						// try to execute current line using fst

		~FST();

	private:
		char*							string_;
		short							position_;
		short							numberOfStates_;
		std::vector<NODE*>				nodes_;
		short*							possibleStates_;
	};
};