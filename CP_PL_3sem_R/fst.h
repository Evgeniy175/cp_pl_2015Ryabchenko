#pragma once

#include "graphs.h"
#include <iostream>
#include <vector>

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
			char* str);

		short							getNumberOfRel();				// number of relations in this node
		RELATION*						getRelation(int index);			// return relation using index

		~NODE();

	private:
		short							numOfRel_;						// number of relation's
		std::vector<RELATION*>			relations_;						// vector if relation's
	};

	class FST{
	public:
		FST();
		FST(
			char* str,
			short numberOfStates,
			NODE* node, ...);

		void							setString(char* str);

		void							createFst();					// create fst with null strings and default graphs
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