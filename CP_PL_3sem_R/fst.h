#pragma once

#include <iostream>
#include "graphs.h"

namespace FST
{
	class RELATION
	{
	public:
		RELATION(char symbol = 0x00, short nextNode = NULL);
		char getSymbol();
		short getNextNode();

	private:
		char symbol_;
		short nextNode_;
	};

	class NODE
	{
	public:
		NODE();
		NODE::NODE(short firstRelation, short secondRelation, char* str);
		NODE(short numOfRel, RELATION relation, ...);

		short getNumberOfRel();
		RELATION getRelation(int i);

	private:
		short numOfRel_;
		RELATION* relations_;
	};

	class FST
	{
	public:
		FST();
		FST(char* str, short numberOfStates, NODE node, ...);

		FST createFst(int i, char* line);
		bool execute(FST& fst);

		void setString(char* str);

	private:
		char* string_;
		short position_;
		short numberOfStates_;
		NODE* nodes_;
		short* possibleStates_;
	};
};