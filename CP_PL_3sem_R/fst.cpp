#include "stdafx.h"
#include "fst.h"

namespace FST{
	RELATION::RELATION(char symbol, short nextNode){
		this->symbol_ = symbol;
		this->nextNode_ = nextNode;
	}

	char RELATION::getSymbol(){
		return this->symbol_;
	};

	short RELATION::getNextNode(){
		return this->nextNode_;
	};

	NODE::NODE() { }

	NODE::NODE(short numOfRel, RELATION relation, ...){
		this->relations_ = new RELATION[numOfRel];
		this->numOfRel_ = numOfRel;

		for (int i = 0; i < numOfRel; i++)
			this->relations_[i] = *(&relation + i);
	}

	NODE::NODE(short firstRelation, short secondRelation, char* str){
		this->relations_ = new RELATION[strlen(str)];
		this->numOfRel_ = (strlen(str)) * 2;

		for (int i = 0, j = 0; str[i] != NULL_STR; i++)
		{
			this->relations_[j++] = *(new RELATION(str[i], firstRelation));
			this->relations_[j++] = *(new RELATION(str[i], secondRelation));
		};
	}

	short NODE::getNumberOfRel(){
		return this->numOfRel_;
	};

	RELATION NODE::getRelation(int i){
		return this->relations_[i];
	};

	FST::FST() { }

	FST::FST(char* str, short numberOfStates, NODE node, ...){
		this->nodes_ = new NODE[numberOfStates];

		this->string_ = str;
		this->numberOfStates_ = numberOfStates;
		this->possibleStates_ = new short[numberOfStates];

		for (int i = 0; i < numberOfStates; i++)
			this->nodes_[i] = *(&node + i);
	}

	void FST::setString(char* str){
		this->string_ = str;
	};

	void FST::createFst(){
		for (int i = 0; i < NUMBER_OF_GRAPHS; i++){
			switch (i){
			case 0:		this[i] = *(new FST(NULL_STR, GRAPH_TYPE));					break;
			case 1:		this[i] = *(new FST(NULL_STR, GRAPH_NEW_LINE));				break;
			case 2:		this[i] = *(new FST(NULL_STR, GRAPH_SEMICOLON));			break;
			case 3:		this[i] = *(new FST(NULL_STR, GRAPH_COMMA));				break;
			case 4:		this[i] = *(new FST(NULL_STR, GRAPH_EQUALLY));				break;
			case 5:		this[i] = *(new FST(NULL_STR, GRAPH_LEFTBRACE));			break;
			case 6:		this[i] = *(new FST(NULL_STR, GRAPH_RIGHTBRACE));			break;
			case 7:		this[i] = *(new FST(NULL_STR, GRAPH_LEFTHESIS));			break;
			case 8:		this[i] = *(new FST(NULL_STR, GRAPH_RIGHTHESIS));			break;
			case 9:		this[i] = *(new FST(NULL_STR, GRAPH_SQBRACE_OPEN));			break;
			case 10:	this[i] = *(new FST(NULL_STR, GRAPH_SQBRACE_CLOSE));		break;
			case 11:	this[i] = *(new FST(NULL_STR, GRAPH_OPERATIONS));			break;
			case 12:	this[i] = *(new FST(NULL_STR, GRAPH_CONDITION));			break;
			case 13:	this[i] = *(new FST(NULL_STR, GRAPH_FUNCTION));				break;
			case 14:	this[i] = *(new FST(NULL_STR, GRAPH_PRINT));				break;
			case 15:	this[i] = *(new FST(NULL_STR, GRAPH_RETURN));				break;
			case 16:	this[i] = *(new FST(NULL_STR, GRAPH_BEGIN));				break;
			case 17:	this[i] = *(new FST(NULL_STR, GRAPH_ID));					break;
			case 18:	this[i] = *(new FST(NULL_STR, GRAPH_LITERAL_STRING));		break;
			case 19:	this[i] = *(new FST(NULL_STR, GRAPH_LITERAL_NUMBER));		break;
			default:	break;
			};
		};
	};

	bool FST::execute(){
		int i, j;
		short* posStates2 = new short[this->numberOfStates_];

		memset(this->possibleStates_, -1, sizeof(short)*(this->numberOfStates_ + 1));
		memset(posStates2, -1, sizeof(short)*(this->numberOfStates_ + 1));

		for (this->possibleStates_[0] = 0, this->position_ = 0;
			 this->position_ < static_cast<int> (strlen(string_)); this->position_++){
				
				for (i = 0; i < numberOfStates_; i++)
					if (this->possibleStates_[i] == this->position_)
						for (j = 0; j < nodes_[i].getNumberOfRel(); j++)
							if (nodes_[i].getRelation(j).getSymbol() == string_[position_])
								posStates2[nodes_[i].getRelation(j).getNextNode()] = position_ + 1;

				std::swap(this->possibleStates_, posStates2);
		};

		return (this->possibleStates_[this->numberOfStates_] == (int)(strlen(this->string_)));
	};
};