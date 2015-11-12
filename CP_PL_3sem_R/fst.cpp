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

	NODE::NODE(short numOfRel, RELATION* relation, ...){
		this->numOfRel_ = numOfRel;

		for (int i = 0; i < numOfRel; i++)
			this->relations_.push_back(*(&relation + i));
	}

	NODE::NODE(short firstRelation, short secondRelation, char* str){
		this->numOfRel_ = (strlen(str)) * 2;

		for (int i = 0, j = 0; str[i] != NULL_STR; i++){
			this->relations_.push_back(new RELATION(str[i], firstRelation));
			this->relations_.push_back(new RELATION(str[i], secondRelation));
		};
	}

	short NODE::getNumberOfRel(){
		return this->numOfRel_;
	};

	RELATION* NODE::getRelation(int i){
		return this->relations_[i];
	};

	NODE::~NODE(){
		for (int i = 0; i < static_cast<int> (this->relations_.size()); i++){
			delete this->relations_[i];
			this->relations_[i] = nullptr;
		};
	}

	FST::FST() { }

	FST::FST(char* str, short numberOfStates, NODE* node, ...){
		this->string_ = str;
		this->numberOfStates_ = numberOfStates;
		this->possibleStates_ = new short[numberOfStates];

		for (int i = 0; i < numberOfStates; i++)
			this->nodes_.push_back(*(&node + i));
	}

	void FST::setString(char* str){
		this->string_ = str;
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
						for (j = 0; j < nodes_[i]->getNumberOfRel(); j++)
						if (nodes_[i]->getRelation(j)->getSymbol() == string_[position_])
								posStates2[nodes_[i]->getRelation(j)->getNextNode()] = position_ + 1;

				std::swap(this->possibleStates_, posStates2);
		};
		return (this->possibleStates_[this->numberOfStates_] == (int)(strlen(this->string_)));
	};

	FST::~FST(){
		for (int i = 0; i < static_cast<int> (this->nodes_.size()); i++){
			delete this->nodes_[i];
			this->nodes_[i] = nullptr;
		};
	}
};