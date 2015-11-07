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
		switch (switchCount)
		{
		case 0:		return LEX_TYPE;
		case 1:		return LEX_NEW_LINE;
		case 2:		return LEX_SEMICOLON;
		case 3:		return LEX_COMMA;
		case 4:		return LEX_COMPARE;
		case 5:		return LEX_EQUALLY;
		case 6:		return LEX_OPEN_BRACE;
		case 7:		return LEX_CLOSE_BRACE;
		case 8:		return LEX_OPEN_PARENTHESIS;
		case 9:		return LEX_CLOSE_PARENTHESIS;
		case 10:		return LEX_OPEN_SQBRACE;
		case 11:	return LEX_CLOSE_SQBRACE;
		case 12:	return LEX_OPERATION;
		case 13:	return LEX_CONDITION;
		case 14:	return LEX_FUNCTION;
		case 15:	return LEX_RETURN;
		case 16:	return LEX_BEGIN;
		case 17:	return LEX_ID;
		case 18:	return LEX_LITERAL;
		case 19:	return LEX_LITERAL;
		default:	break;
		};
		return LT_NULL_LEX;
	};
};