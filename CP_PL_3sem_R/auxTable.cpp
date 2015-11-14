#include "stdafx.h"
#include "auxTable.h"

namespace AT{
	Info::Element::Element(char* name, int value){
		this->name_ = name;
		this->value_ = value;
	}

	Info::Info(){
		int i;
		char* nameTemp[] = AT_PRIMITIVE_TYPES_NAMES;
		char* funcNameTemp[] = AT_STL_FUNCTIONS;
		char* typeNameTemp[] = AT_ELEMENT_TYPES_NAMES;
		char* compareNameTemp[] = AT_COMPARES_NAMES;
		char* operationNameTemp[] = AT_OPERATIONS_NAMES;
		DATATYPE funcTypeValTemp[AT_STL_FUNCTIONS_SIZE] = AT_STL_FUNCTIONS_TYPES;
		DATATYPE primTypesValueTemp[AT_PRIMITIVE_TYPES_SIZE] = AT_PRIMITIVE_TYPES_VALUES;

		for (i = 0; i < AT_PRIMITIVE_TYPES_SIZE; i++){
			this->primTypes_.push_back(Element(nameTemp[i], i));
		};
		for (i = 0; i < AT_STL_FUNCTIONS_SIZE; i++){
			this->functions_.push_back(Element(funcNameTemp[i], funcTypeValTemp[i]));
		};
		for (i = 0; i < AT_ELEMENT_TYPES_SIZE; i++){
			this->elementsTypes_.push_back(Element(typeNameTemp[i], primTypesValueTemp[i]));
		};
		for (i = 0; i < AT_COMPARES_SIZE; i++){
			this->compares_.push_back(Element(compareNameTemp[i], i));
		};
		for (i = 0; i < AT_OPERATIONS_SIZE; i++){
			this->operations_.push_back(Element(operationNameTemp[i], i));
		};
	};

	char* Info::getPrimTypeName(int value){
		return (value < static_cast<int> (this->primTypes_.size()) && value >= NULL)
			? this->primTypes_[value].name_ : "unknown";
	};

	char* Info::getFuncName(int value){
		return (value < static_cast<int> (this->functions_.size()) && value >= NULL)
			? this->functions_[value].name_ : NULL_STR;
	};

	char* Info::getElemTypeName(int value){
		return (value < static_cast<int> (this->elementsTypes_.size()) && value >= NULL)
			? this->elementsTypes_[value].name_ : NULL_STR;
	};

	char* Info::getCompareName(int value){
		return (value < static_cast<int> (this->compares_.size()) && value >= NULL)
			? this->compares_[value].name_ : NULL_STR;
	};

	char Info::getOperationName(int value){
		return (value < static_cast<int> (this->operations_.size()) && value >= NULL)
			? this->operations_[value].name_[0] : NULL_STR;
	};

	int Info::getPrimTypeValue(char* name){
		std::vector<Element>::iterator it = this->primTypes_.begin();
		for (; it != this->primTypes_.end(); it++){
			if (!strcmp(*(&it->name_), name)){
				return *(&it->value_);
			};
		};
		return SERVICE::ERROR_VALUE;
	};

	int Info::getFuncValue(char* name){
		std::vector<Element>::iterator it = this->functions_.begin();
		for (; it != this->functions_.end(); it++){
			if (!strcmp(*(&it->name_), name)){
				return *(&it->value_);
			};
		};
		return SERVICE::ERROR_VALUE;
	};

	int Info::getElemTypeValue(char* name){
		std::vector<Element>::iterator it = this->elementsTypes_.begin();
		for (; it != this->elementsTypes_.end(); it++){
			if (!strcmp(*(&it->name_), name)){
				return *(&it->value_);
			};
		};
		return SERVICE::ERROR_VALUE;
	};

	int Info::getCompareValue(char* name){
		std::vector<Element>::iterator it = this->compares_.begin();
		for (; it != this->compares_.end(); it++){
			if (!strcmp(*(&it->name_), name)){
				return *(&it->value_);
			};
		};
		return SERVICE::ERROR_VALUE;
	};

	int Info::getOperationValue(char* name){
		std::vector<Element>::iterator it = this->operations_.begin();
		for (; it != this->operations_.end(); it++){
			if (!strcmp(*(&it->name_), name)){
				return *(&it->value_);
			};
		};
		return SERVICE::ERROR_VALUE;
	};

	bool Info::isNewFunction(char* name){
		return getFuncValue(name) == SERVICE::ERROR_VALUE ? true : false;
	};

	void Info::pushToFuncList(char* name, int type){
		this->functions_.push_back(Element(name, type));
	};

	Element::Element(){
		this->dataType_ = SERVICE::ERROR_VALUE;
		this->type_ = SERVICE::ERROR_VALUE;
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

	int Element::getType(){
		return this->type_;
	};

	int Element::getDataType(){
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

	void Element::setElem(LA::LexAnalyser* la, char* funcName,
		char** arrOfLines, int& i, int counter){
		char lexeme = la->getElemLt(la->getLtSize() - 1)->getLex();
		this->ltIndex_ = la->getLtSize() - 1;
		this->dataType_ = la->getDataType(arrOfLines, i);
		this->type_ = lexeme == LEX_BEGIN ? TYPE::F : la->getElemTypeValue(arrOfLines[i]);
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
			this->setNumVal(la->getOperationValue(arrOfLines[i]));
			break;

		case  LEX_COMPARE:
			_itoa_s(counter++, this->name_, 10);
			addPrefix(this->name_, AT_PREFIX_COMPARE);
			this->setNumVal(la->getCompareValue(arrOfLines[i]));
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

		default: break;
		};
  	};

	void Element::reset(){
		memset(this->name_, LT_NULL_LEX, AT_NAME_MAXSIZE);
		this->dataType_ = SERVICE::ERROR_VALUE;
		this->type_ = SERVICE::ERROR_VALUE;
		this->ltIndex_ = AT_NULL_INDEX;
		this->setNumVal(AT_NUM_DEFAULT);
		this->setLineVal(AT_LINE_DEFAULT);
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

	int Table::getFuncIndex(char* name){
		for (int i = 0; i < this->size_; i++){
			if (!strcmp(this->table_[i].getName(), name)){
				return i;
			};
		};
		return AT_NULL_INDEX;
	};

	Info* Table::getInfo(){
		return this->info_;
	};

	Element* Table::getElem(int i){
		return (table_ + i);
	};

	int Table::getSize(){
		return this->size_;
	};

	int Table::getLiteralDataType(char* line){
		return line[0] == '‘' ? AT::DATATYPE::LINE : AT::DATATYPE::NUM;
	};

	char Table::getOperation(int value){
		return this->getInfo()->getOperationName(value);
	};

	void Table::addElem(Element& elem){
		this->table_[this->size_++] = elem;
	};

	bool Table::isIncluded(char* name, char* funcName, char lexeme){
		char tempName[AT_NAME_MAXSIZE];
		char tempFuncName[AT_NAME_MAXSIZE];

		if (this->size_ == NULL) return false;

		strncpy_s(tempName, name, AT_NAME_MAXSIZE - 1);
		strncpy_s(tempFuncName, funcName, AT_NAME_MAXSIZE - 1);

		if (lexeme == LEX_LITERAL){
			if (this->getLiteralDataType(tempName) == DATATYPE::LINE){
				name = createStrVal(tempName);
			};
			for (int i = 0; i < this->size_; i++){
				if (this->table_[i].getType() == TYPE::L
					&& !strcmp(this->table_[i].getLineVal(), name)
					&& !strcmp(this->table_[i].getFuncName(), tempFuncName)){
						return true;
				};
			};
		}
		else{
			for (int i = 0; i < size_; i++){
				if ((!strcmp(this->table_[i].getName(), tempName)
					&& !strcmp(this->table_[i].getFuncName(), tempFuncName))
					|| (!strcmp(this->table_[i].getFuncName(), tempName))){
						return true;
				};
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
		return AT_NULL_INDEX;
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
		strncpy(rc, line + 1, static_cast<int> (strlen(line)) - 2); // -2 due to 2	quotes
		rc[static_cast<int> (strlen(line)) - 2] = NULL_STR;
		return rc;
	};
};