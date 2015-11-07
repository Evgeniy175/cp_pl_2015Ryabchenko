#include "stdafx.h"
#include "lexTable.h"

namespace LT
{
	Element::Element(){
		this->lexeme_ = LT_NULL_LEX;
		this->line_ = LT_AUX_NULL_LINE;
		this->atIndex_ = LT_AUX_NULL_INDEX;
		this->parameterCounter_ = LT_NULL_PARM;
	} 

	char Element::getLex(){
		return this->lexeme_;
	};

	int	Element::getIndex(){
		return this->atIndex_;
	};

	int Element::getParmCount(){
		return this->parameterCounter_;
	};

	int Element::getLine(){
		return this->line_;
	};

	void Element::setIndex(int auxIndex){
		this->atIndex_ = auxIndex;
	};

	void Element::setElem(){
		this->lexeme_ = LT_NULL_LEX;
		this->line_ = LT_AUX_NULL_LINE;
		this->atIndex_ = LT_AUX_NULL_INDEX;
		this->parameterCounter_ = LT_NULL_PARM;
	};

	void Element::setElem(const Element& elem){
		this->lexeme_ = elem.lexeme_;
		this->line_ = elem.line_;
		this->atIndex_ = elem.atIndex_;
		this->parameterCounter_ = elem.parameterCounter_;
	};

	void Element::setElem(int& i, int& lineNumber){
		this->lexeme_ = LT::getLex(i);
		this->line_ = lineNumber;
		this->atIndex_ = LT_AUX_NULL_INDEX;
		this->parameterCounter_ = LT_NULL_PARM;
	};

	Table::Table(){
		this->size_ = NULL;
	}

	Table::Table(int maxSize){
		this->size_ = NULL;

		if (maxSize < LT_MAX_SIZE) this->maxSize_ = maxSize;
		else throw ERROR_THROW(200);

		this->table_ = new Element[maxSize];
	}

	int Table::getSize(){
		return this->size_;
	};

	Element* Table::getElem(int i){
		return (table_ + i);
	};

	void Table::addElem(Element& elem){
		this->table_[this->size_++] = elem;
	};

	void Element::setParmCount(int value){
		this->parameterCounter_ = value;
	};
	
	Table::~Table(){
		delete[] this->table_;
	};

	char getLex(int switchCount){
		char rc;
		switch (switchCount)
		{
		case 0:		rc = LEX_TYPE;					break;
		case 1:		rc = LEX_NEW_LINE;				break;
		case 2:		rc = LEX_SEMICOLON;				break;
		case 3:		rc = LEX_COMMA;					break;
		case 4:		rc = LEX_EQUALLY;				break;
		case 5:		rc = LEX_OPEN_BRACE;			break;
		case 6:		rc = LEX_CLOSE_BRACE;			break;
		case 7:		rc = LEX_OPEN_PARENTHESIS;		break;
		case 8:		rc = LEX_CLOSE_PARENTHESIS;		break;
		case 9:		rc = LEX_OPEN_SQBRACE;			break;
		case 10:	rc = LEX_CLOSE_SQBRACE;			break;
		case 11:	rc = LEX_OPERATION;				break;
		case 12:	rc = LEX_CONDITION;				break;
		case 13:	rc = LEX_FUNCTION;				break;
		case 14:	rc = LEX_RETURN;				break;
		case 15:	rc = LEX_BEGIN;					break;
		case 16:	rc = LEX_ID;					break;
		case 17:	rc = LEX_LITERAL;				break;
		case 18:	rc = LEX_LITERAL;				break;
		default:	break;
		};
		return rc;
	};
};