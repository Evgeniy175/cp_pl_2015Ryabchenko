#include "stdafx.h"
#include "lexTable.h"

namespace LT
{
	Element::Element() { } 

	char Element::getLex(){
		return this->lexeme_;
	};

	int	Element::getIdx(){
		return this->itIndex_;
	};

	int Element::getLineNumber(){
		return this->lineNumber_;
	};

	void Element::setIdx(int itIndex){
		this->itIndex_ = itIndex;
	};

	void Element::setElem(int& i, int& lineNumber){
		this->lexeme_ = LT::getLex(i);
		this->lineNumber_ = lineNumber;
		this->itIndex_ = LT_TI_NULL_IDX;
	};

	LexTable::LexTable(){
		this->size_ = NULL;
	}

	LexTable::LexTable(int maxSize){
		this->size_ = NULL;

		if (maxSize < LT_MAXSIZE) this->maxSize_ = maxSize;
		else throw ERROR_THROW(200);

		this->table_ = new Element[this->maxSize_];
	}

	int LexTable::getSize(){
		return this->size_;
	};

	Element* LexTable::getElem(int i){
		return (table_ + i);
	};

	void LexTable::addElem(Element elem){
		this->table_[this->size_++] = elem;
	};
	
	LexTable::~LexTable(){
		delete[] this->table_;
	};

	char getLex(int switchCount){
		char rc;

		switch (switchCount)
		{
		case 0:		rc = LEX_TYPE;					break;
		case 1:		rc = LEX_NEWLINE;				break;
		case 2:		rc = LEX_COLON;					break;
		case 3:		rc = LEX_SEMICOLON;				break;
		case 4:		rc = LEX_COMMA;					break;
		case 5:		rc = LEX_EQUALLY;				break;
		case 6:		rc = LEX_LEFTBRACE;				break;
		case 7:		rc = LEX_RIGHTBRACE;			break;
		case 8:		rc = LEX_LEFTHESIS;				break;
		case 9:		rc = LEX_RIGHTHESIS;			break;
		case 10:	rc = LEX_SQBRACEOPEN;			break;
		case 11:	rc = LEX_SQBRACECLOSE;			break;
		case 12:	rc = LEX_ACTIONS;				break;
		case 13:	rc = LEX_CONDITION;				break;
		case 14:	rc = LEX_EXTERN;				break;
		case 15:	rc = LEX_FUNCTION;				break;
		case 16:	rc = LEX_PRINT;					break;
		case 17:	rc = LEX_RETURN;				break;
		case 18:	rc = LEX_BEGIN;					break;
		case 19:	rc = LEX_ID;					break;
		case 20:	rc = LEX_LITERAL;				break;
		case 21:	rc = LEX_LITERAL;				break;
		default:	break;
		};

		return rc;
	};
};