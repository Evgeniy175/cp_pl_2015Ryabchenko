#include "stdafx.h"
#include "fst.h"

namespace FST
{
	RELATION::RELATION(char symbol, short nextNode)
	{
		this->symbol_ = symbol;
		this->nextNode_ = nextNode;
	}

	char RELATION::getSymbol()
	{
		return this->symbol_;
	};

	short RELATION::getNextNode()
	{
		return this->nextNode_;
	};

	NODE::NODE()
	{

	}

	NODE::NODE(short firstRelation, short secondRelation, char* str)
	{
		this->relations_ = new RELATION[strlen(str)];
		this->numOfRel_ = (strlen(str)) * 2;

		for (int i = 0, j = 0; str[i] != NULL_STR; i++)
		{
			this->relations_[j++] = *(new RELATION(str[i], firstRelation));
			this->relations_[j++] = *(new RELATION(str[i], secondRelation));
		};
	}

	NODE::NODE(short numOfRel, RELATION relation, ...)
	{
		RELATION* temp = &relation;

		this->relations_ = new RELATION[numOfRel];
		this->numOfRel_ = numOfRel;

		for (int i = 0; i < numOfRel; i++)	this->relations_[i] = *(temp + i);
	}

	short NODE::getNumberOfRel()
	{
		return this->numOfRel_;
	};


	RELATION NODE::getRelation(int i)
	{
		return this->relations_[i];
	};

	FST::FST()
	{

	}

	FST::FST(char* str, short numberOfStates, NODE node, ...)
	{
		this->nodes_ = new NODE[numberOfStates];
		NODE* temp = &node;

		this->string_ = str;
		this->numberOfStates_ = numberOfStates;
		this->possibleStates_ = new short[numberOfStates];

		for (int i = 0; i < numberOfStates; i++)	this->nodes_[i] = *(temp + i);
	}

	FST FST::createFst(int i, char* line)
	{
		FST rc;

		switch (i)
		{
		case 0:		rc = *(new FST(line, GRAPH_TYPE));					break;
		case 1:		rc = *(new FST(line, GRAPH_NEW_LINE));				break;
		case 2:		rc = *(new FST(line, GRAPH_COLON));					break;
		case 3:		rc = *(new FST(line, GRAPH_SEMICOLON));				break;
		case 4:		rc = *(new FST(line, GRAPH_COMMA));					break;
		case 5:		rc = *(new FST(line, GRAPH_EQUALLY));			break;
		case 6:		rc = *(new FST(line, GRAPH_LEFTBRACE));			break;
		case 7:		rc = *(new FST(line, GRAPH_RIGHTBRACE));			break;
		case 8:		rc = *(new FST(line, GRAPH_LEFTHESIS));		break;
		case 9:		rc = *(new FST(line, GRAPH_RIGHTHESIS));		break;
		case 10:	rc = *(new FST(line, GRAPH_SQBRACE_OPEN));		break;
		case 11:	rc = *(new FST(line, GRAPH_SQBRACE_CLOSE));		break;
		case 12:	rc = *(new FST(line, GRAPH_ACTIONS));				break;
		case 13:	rc = *(new FST(line, GRAPH_CONDITION));				break;
		case 14:	rc = *(new FST(line, GRAPH_EXTERN));				break;
		case 15:	rc = *(new FST(line, GRAPH_FUNCTION));				break;
		case 16:	rc = *(new FST(line, GRAPH_PRINT));					break;
		case 17:	rc = *(new FST(line, GRAPH_RETURN));				break;
		case 18:	rc = *(new FST(line, GRAPH_BEGIN));					break;
		case 19:	rc = *(new FST(line, GRAPH_ID));					break;
		case 20:	rc = *(new FST(line, GRAPH_LITERAL_STRING));		break;
		case 21:	rc = *(new FST(line, GRAPH_LITERAL_NUMBER));		break;
		default:	break;
		};

		return rc;
	};

	void FST::setString(char* str)
	{
		this->string_ = str;
	};

	bool FST::execute(FST& fst)
	{
		int i, j;
		short* rstates2 = new short[fst.numberOfStates_];

		memset(fst.possibleStates_, -1, sizeof(short)*(fst.numberOfStates_ + 1));
		memset(rstates2, -1, sizeof(short)*(fst.numberOfStates_ + 1));

		for (fst.possibleStates_[0] = 0, fst.position_ = 0; fst.position_ < (int)(strlen(fst.string_)); fst.position_++)
		{
			for (i = 0; i < fst.numberOfStates_; i++)
				if (fst.possibleStates_[i] == fst.position_)
					for (j = 0; j < fst.nodes_[i].getNumberOfRel(); j++)
						if (fst.nodes_[i].getRelation(j).getSymbol() == fst.string_[fst.position_])
							rstates2[fst.nodes_[i].getRelation(j).getNextNode()] = fst.position_ + 1;

			std::swap(fst.possibleStates_, rstates2);
		};

		return (fst.possibleStates_[fst.numberOfStates_] == (int)(strlen(fst.string_)));
	};
};