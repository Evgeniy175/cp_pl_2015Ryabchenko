#include "stdafx.h"
#include "lexTable.h"

namespace LT
{
	Element::Element(){
		this->lexeme_ = LT_NULL_LEX;
		this->lineNumber_ = LT_AUX_NULL_LINE;
		this->auxIndex_ = LT_AUX_NULL_IDX;
		this->parameterCounter = LT_NULL_PARM;
	} 

	char Element::getLex(){
		return this->lexeme_;
	};

	int	Element::getIdx(){
		return this->auxIndex_;
	};

	int Element::getParmCount(){
		return this->parameterCounter;
	};

	int Element::getLineNumber(){
		return this->lineNumber_;
	};

	void Element::setIdx(int auxIndex){
		this->auxIndex_ = auxIndex;
	};

	void Element::setElem(){
		this->lexeme_ = LT_NULL_LEX;
		this->lineNumber_ = LT_AUX_NULL_LINE;
		this->auxIndex_ = LT_AUX_NULL_IDX;
		this->parameterCounter = LT_NULL_PARM;
	};

	void Element::setElem(int& i, int& lineNumber){
		this->lexeme_ = LT::getLex(i);
		this->lineNumber_ = lineNumber;
		this->auxIndex_ = LT_AUX_NULL_IDX;
		this->parameterCounter = LT_NULL_PARM;
	};

	void Element::setElem(const Element& elem){
		this->lexeme_ = elem.lexeme_;
		this->lineNumber_ = elem.lineNumber_;
		this->auxIndex_ = elem.auxIndex_;
		this->parameterCounter = elem.parameterCounter;
	};

	Table::Table(){
		this->size_ = NULL;
	}

	Table::Table(int maxSize){
		this->size_ = NULL;

		if (maxSize < LT_MAXSIZE) this->maxSize_ = maxSize;
		else throw ERROR_THROW(200);

		this->table_ = new Element[maxSize];
	}

	int Table::getSize(){
		return this->size_;
	};

	Element* Table::getElem(int i){
		return (table_ + i);
	};

	AT::TYPE Table::getType(){
		AT::TYPE rc = AT::TYPE::U;

		if (this->getElem(this->getSize() - 3)->getLex() == LEX_FUNCTION){
			rc = this->getElem(this->getSize() - 4)->getLex() == LEX_EXTERN ? AT::TYPE::E : AT::TYPE::F;
		}
		else if (this->getElem(this->getSize() - 1)->getLex() == LEX_OPERATION){
			rc = AT::TYPE::O;
		}
		else{
			switch (this->getElem(this->getSize() - 2)->getLex())
			{
			case LEX_COMMA: case LEX_SQBRACEOPEN: rc = AT::TYPE::P; break;
			case LEX_TYPE: rc = AT::TYPE::V; break;
			case LEX_COLON: rc = AT::TYPE::S; break;
			case LEX_RETURN: case LEX_EQUALLY: case LEX_OPERATION: rc = AT::TYPE::L; break;
			default: break;
			};
		};
		return rc;
	};

	void Table::addElem(Element& elem){
		this->table_[this->size_++] = elem;
	};

	void Element::setParmCount(int value){
		this->parameterCounter = value;
	};
	
	Table::~Table(){
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
		case 12:	rc = LEX_OPERATION;				break;
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