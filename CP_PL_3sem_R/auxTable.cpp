#include "stdafx.h"
#include "auxTable.h"

namespace AT{
	Info::Info(){
		int i;
		char* nameTemp[] = AT_PRIMITIVE_TYPES_NAMES;
		char* funcNameTemp[] = AT_STL_FUNCTIONS;
		char* typeNameTemp[] = AT_ELEMENT_TYPES_NAMES;
		char* compareNameTemp[] = AT_COMPARE_NAME;
		char* operationNameTemp[] = AT_OPERATION_NAME;
		DATATYPE typeValTemp[AT_PRIMITIVE_TYPES_SIZE] = AT_PRIMITIVE_TYPES;
		DATATYPE funcTypeValTemp[AT_STL_FUNCSIZE] = AT_STL_FUNCTIONS_TYPES;
		TYPE elemTypeValTemp[AT_ELEMENT_TYPES_SIZE] = AT_ELEMENT_TYPES;
		COMPARE compareValTemp[AT_COMPARE_SIZE] = AT_COMPARE_VALUE;
		OPERATION operationValTemp[AT_OPERATION_SIZE] = AT_OPERATION_VALUE;

		for (i = 0; i < AT_PRIMITIVE_TYPES_SIZE; i++){			// заполнение векторов для встроенных типов
			this->name_.push_back(nameTemp[i]);
			this->typeValue_.push_back(typeValTemp[i]);
		};
		for (i = 0; i < AT_STL_FUNCSIZE; i++){
			this->funcName_.push_back(funcNameTemp[i]);
			this->funcTypeValue_.push_back(funcTypeValTemp[i]);
		};
		for (i = 0; i < AT_ELEMENT_TYPES_SIZE; i++){
			this->typeName_.push_back(typeNameTemp[i]);
			this->elemTypeValue_.push_back(elemTypeValTemp[i]);
		};
		for (i = 0; i < AT_COMPARE_SIZE; i++){
			this->compareName_.push_back(compareNameTemp[i]);
			this->compareValue_.push_back(compareValTemp[i]);
		};
		for (i = 0; i < AT_OPERATION_SIZE; i++){
			this->operationName_.push_back(operationNameTemp[i]);
			this->operationValue_.push_back(operationValTemp[i]);
		};
	};

	char* Info::getName(DATATYPE type){
		return type < static_cast<int> (this->typeValue_.size())
			? this->name_[type] : "unknown";
	};

	char* Info::getFuncName(DATATYPE type){
		return type < static_cast<int> (this->funcTypeValue_.size())
			? this->funcName_[type] : NULL_STR;
	};

	char* Info::getTypeName(TYPE type){
		return type < static_cast<int> (this->elemTypeValue_.size())
			? this->typeName_[type] : NULL_STR;
	};

	char* Info::getCompareName(COMPARE value){
		return value < static_cast<int> (this->compareValue_.size())
			? this->compareName_[value] : NULL_STR;
	};

	char* Info::getOperationName(OPERATION value){
		return value < static_cast<int> (this->operationValue_.size())
			? this->operationName_[value] : NULL_STR;
	};

	DATATYPE Info::getType(char* name){
		std::vector<char*>::iterator firstIt = this->name_.begin();
		std::vector<DATATYPE>::iterator secondIt = this->typeValue_.begin();
		for (; firstIt != this->name_.end(); firstIt++, secondIt++){
			if (!strcmp(*firstIt, name)) return *secondIt;
		};
		return DATATYPE::UNKNOWN;
	};

	DATATYPE Info::getFuncType(char* name){
		std::vector<char*>::iterator firstIt = this->funcName_.begin();
		std::vector<DATATYPE>::iterator secondIt = this->funcTypeValue_.begin();
		for (; firstIt != this->funcName_.end(); firstIt++, secondIt++){
			if (!strcmp(*firstIt, name)) return *secondIt;
		};
		return DATATYPE::UNKNOWN;
	};

	TYPE Info::getElemType(char* name){
		std::vector<char*>::iterator firstIt = this->typeName_.begin();
		std::vector<TYPE>::iterator secondIt = this->elemTypeValue_.begin();
		for (; firstIt != this->typeName_.end(); firstIt++, secondIt++){
			if (!strcmp(*firstIt, name)) return *secondIt;
		};
		return TYPE::U;
	};

	COMPARE Info::getCompareValue(char* name){
		std::vector<char*>::iterator firstIt = this->compareName_.begin();
		std::vector<COMPARE>::iterator secondIt = this->compareValue_.begin();
		for (; firstIt != this->compareName_.end(); firstIt++, secondIt++){
			if (!strcmp(*firstIt, name)) return *secondIt;
		};
		return COMPARE::OTHER;
	};

	OPERATION Info::getOperationValue(char* name){
		std::vector<char*>::iterator firstIt = this->operationName_.begin();
		std::vector<OPERATION>::iterator secondIt = this->operationValue_.begin();
		for (; firstIt != this->operationName_.end(); firstIt++, secondIt++){
			if (!strcmp(*firstIt, name)) return *secondIt;
		};
		return OPERATION::NOT_ALLOWED;
	};

	bool Info::isNewFunction(char* name){
		return getFuncType(name) == DATATYPE::UNKNOWN ? true : false;
	};

	void Info::pushFuncName(char* line){
		this->funcName_.push_back(line);
	};

	void Info::pushFuncType(DATATYPE type){
		this->funcTypeValue_.push_back(type);
	};

	Element::Element(){
		this->dataType_ = DATATYPE::UNKNOWN;
		this->type_ = TYPE::U;
		setNumVal(AT_NUM_DEFAULT);
		setLineVal(AT_LINE_DEFAULT);
	}

	int	Element::getIndex(){
		return this->ltIndex_;
	};

	char* Element::getName(){
		return this->name_;
	};

	char* Element::getFuncName(){
		return this->funcName_;
	};

	int Element::getNumVal(){
		return this->value_.numValue_;
	};

	char* Element::getLineVal(){
		return this->value_.lineValue_;
	};

	char* Element::getOtherVal(){
		return this->value_.otherValue_;
	};

	char Element::getOperation(){
		return this->value_.otherValue_[0];
	};

	TYPE Element::getType(){
		return this->type_;
	};

	DATATYPE Element::getDataType(){
		return this->dataType_;
	};

	void Element::setIndex(int value){
		this->ltIndex_ = value;
	};

	void Element::setName(char* name){
		strncpy_s(this->name_, name, AT_NAME_MAXSIZE - 1);
	};

	void Element::setFuncName(char* name){
		strncpy_s(this->funcName_, name, AT_NAME_MAXSIZE - 1);
	};

	void Element::setNumVal(int value){
		this->value_.numValue_ = value;
	};
	
	void Element::setLineVal(char* value){
		if (value != NULL)
			strcpy_s(this->value_.lineValue_, value);
		
		else
			memset(this->value_.lineValue_, AT_ARR_MAXSIZE, 'M');
	};

	void Element::setOtherVal(char* value){
		strcpy_s(this->value_.otherValue_, value);
	};

	void Element::setElem(LA::LexAnalyser* la, char* funcName,
		char** arrOfLines, int& i, int counter){
		char lexeme = la->getElemLt(la->getLtSize() - 1)->getLex();
		this->ltIndex_ = la->getLtSize() - 1;
		this->dataType_ = la->getDataType(arrOfLines, i);
		this->type_ = lexeme == LEX_BEGIN ? TYPE::F : la->getElemType(arrOfLines[i]);
		this->setFuncName(funcName);
		switch (lexeme){
		case LEX_LITERAL:
			_itoa_s(counter++, this->name_, 10);
			addPrefix(this->name_, AT_PREFIX_LITERAL);
			setValue(lexeme, arrOfLines[i]);
			break;

		case LEX_IDENTIFIER: case LEX_BEGIN:
			this->setName(arrOfLines[i]);
			setValue(lexeme);
			break;

		case LEX_OPERATION:
			_itoa_s(counter++, this->name_, 10);
			addPrefix(this->name_, AT_PREFIX_OPERATION);
			setValue(lexeme, arrOfLines[i]);
			break;

		case  LEX_COMPARE:
			_itoa_s(counter++, this->name_, 10);
			addPrefix(this->name_, AT_PREFIX_COMPARE);
			setValue(lexeme, arrOfLines[i]);
			break;

		default: break;
		};
	};

  	void Element::setValue(char lexeme, char* line){
		switch (lexeme){
		case LEX_LITERAL:
			if (this->dataType_ == DATATYPE::NUM){
				this->setNumVal(std::atoi(line));
			}
			else if (this->dataType_ == DATATYPE::LINE){
				line = createStrVal(line);
				this->setLineVal(line);
			};
			break;

		case LEX_IDENTIFIER:
			if (this->dataType_ == DATATYPE::NUM){
				this->setNumVal(AT_NUM_DEFAULT);
			}
			else if (this->dataType_ == DATATYPE::LINE){
				this->setLineVal(AT_LINE_DEFAULT);
			};
			break;

		case LEX_OPERATION: case LEX_COMPARE: this->setOtherVal(line); break;
		default: break;
		};
  	};

	void Element::reset(){
		memset(this->name_, -52, AT_NAME_MAXSIZE);
		this->dataType_ = DATATYPE::UNKNOWN;
		this->type_ = TYPE::U;
		this->ltIndex_ = AT_NULL_INDEX;
		this->setNumVal(AT_NUM_DEFAULT);
		this->setLineVal(AT_LINE_DEFAULT);
		this->setOtherVal(AT_NULL_OTHER_VALUE);
	};

	Table::Table(){
		this->info_ = new Info();
	}

	Table::Table(int maxSize){
		this->info_ = new Info();
		this->size_ = NULL;

		if (maxSize < AT_MAX_SIZE) this->maxSize_ = maxSize;
		else throw ERROR_THROW(201);

		this->table_ = new Element[maxSize];
	}

	Info* Table::getInfo(){
		return this->info_;
	};

	Element* Table::getElem(int i){
		return (table_ + i);
	};

	int Table::getSize(){
		return this->size_;
	};

	void Table::addElem(Element& elem){
		this->table_[this->size_++] = elem;
	};

	bool Table::isIncluded(char* name, char* funcName){
		char tempName[AT_NAME_MAXSIZE];
		char tempFuncName[AT_NAME_MAXSIZE];
		strncpy_s(tempName, name, AT_NAME_MAXSIZE - 1);
		strncpy_s(tempFuncName, funcName, AT_NAME_MAXSIZE - 1);
		if (this->size_ == NULL) return false;

		for (int i = 0; i < size_; i++){
			if ((!strcmp(this->table_[i].getName(), tempName)
				&& !strcmp(this->table_[i].getFuncName(), tempFuncName))
				|| (!strcmp(this->table_[i].getFuncName(), tempName))){
					return true;
			};
		};
		return false;
	};

	int Table::getIndex(char* name, char* funcName){
		if (this->size_ == NULL) return -1;

		for (int i = 0; i < this->size_; i++){
			if (!strcmp(this->table_[i].getName(), name)
				&& !strcmp(this->table_[i].getFuncName(), funcName)){
					return i;
			};
		};
		return -1;
	};

	Table::~Table(){
		delete[] this->table_;
		delete this->info_;
	};

	void addPrefix(char* dest, char* prefix){
		char temp[AT_ARR_MAXSIZE];
		strcpy_s(temp, dest);
		strncpy(dest, prefix, AT_NAME_MAXSIZE - 1);
		strncat(dest, temp, AT_NAME_MAXSIZE - static_cast<int> (strlen(prefix)) - 1);
	};

	char* createStrVal(char* line){
		char* rc = new char;
		strncpy(rc, line + 1, static_cast<int> (strlen(line)) - 2); // due to 2	quotes
		rc[static_cast<int> (strlen(line)) - 2] = NULL_STR;
		return rc;
	};
};