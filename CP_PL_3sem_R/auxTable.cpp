#include "stdafx.h"
#include "auxTable.h"

namespace AT{
	Info::Info(){
		int i;
		char* nameTemp[] = AT_PRIMITIVE_TYPES_NAMES;
		char* funcNameTemp[] = AT_STL_FUNCTIONS;
		char* typeNameTemp[] = AT_ELEMENT_TYPES_NAMES;
		DATATYPE typeTemp[AT_PRIMITIVE_TYPES_SIZE] = AT_PRIMITIVE_TYPES;
		DATATYPE funcTypeTemp[AT_STL_FUNCSIZE] = AT_STL_FUNCTIONS_TYPES;
		TYPE elemTypeTemp[AT_ELEMENT_TYPES_SIZE] = AT_ELEMENT_TYPES;

		for (i = 0; i < AT_PRIMITIVE_TYPES_SIZE; i++){			// заполнение векторов для встроенных типов
			this->name_.push_back(nameTemp[i]);
			this->type_.push_back(typeTemp[i]);
		};
		for (i = 0; i < AT_STL_FUNCSIZE; i++){
			this->funcName_.push_back(funcNameTemp[i]);
			this->funcType_.push_back(funcTypeTemp[i]);
		};
		for (i = 0; i < AT_ELEMENT_TYPES_SIZE; i++){
			this->typeName_.push_back(typeNameTemp[i]);
			this->elemType_.push_back(elemTypeTemp[i]);
		};
	};

	char* Info::getName(AT::DATATYPE dataType){
		std::vector<AT::DATATYPE>::iterator firstIt = this->type_.begin();
		std::vector<char*>::iterator secondIt = this->name_.begin();
		for (; secondIt != this->name_.end(); firstIt++, secondIt++){
			if (*firstIt == dataType) return *secondIt;
		};
		return "unknown";
	};

	char* Info::getFuncName(AT::DATATYPE type){
		std::vector<AT::DATATYPE>::iterator firstIt = this->funcType_.begin();
		std::vector<char*>::iterator secondIt = this->funcName_.begin();
		for (; firstIt != this->funcType_.end(); firstIt++, secondIt++){
			if (*firstIt == type) return *secondIt;
		};
		return NULL_STR;
	};

	char* Info::getTypeName(AT::TYPE type){
		std::vector<AT::TYPE>::iterator firstIt = this->elemType_.begin();
		std::vector<char*>::iterator secondIt = this->typeName_.begin();
		for (; firstIt != this->elemType_.end(); firstIt++, secondIt++){
			if (*firstIt == type) return *secondIt;
		};
		return NULL_STR;
	};

	DATATYPE Info::getType(char* line){
		std::vector<char*>::iterator firstIt = this->name_.begin();
		std::vector<DATATYPE>::iterator secondIt = this->type_.begin();
		for (; firstIt != this->name_.end(); firstIt++, secondIt++){
			if (!strcmp(*firstIt, line)) return *secondIt;
		};
		return AT::DATATYPE::UNKNOWN;
	};

	DATATYPE Info::getFuncType(char* line){
		std::vector<char*>::iterator firstIt = this->funcName_.begin();
		std::vector<DATATYPE>::iterator secondIt = this->funcType_.begin();
		for (; firstIt != this->funcName_.end(); firstIt++, secondIt++){
			if (!strcmp(*firstIt, line)) return *secondIt;
		};
		return AT::DATATYPE::UNKNOWN;
	};

	TYPE Info::getElemType(char* line){
		std::vector<char*>::iterator firstIt = this->typeName_.begin();
		std::vector<TYPE>::iterator secondIt = this->elemType_.begin();
		for (; firstIt != this->typeName_.end(); firstIt++, secondIt++){
			if (!strcmp(*firstIt, line)) return *secondIt;
		};
		return AT::TYPE::U;
	};

	bool Info::isNewFunction(char* name){
		std::vector<char*>::iterator firstIt = this->funcName_.begin();
		std::vector<AT::DATATYPE>::iterator secondIt = this->funcType_.begin();
		for (; firstIt != this->funcName_.end(); firstIt++, secondIt++){
			if (!strcmp(*firstIt, name)) return false;
		};
		return true;
	};

	void Info::pushFuncName(char* line){
		this->funcName_.push_back(line);
	};

	void Info::pushFuncType(AT::DATATYPE type){
		this->funcType_.push_back(type);
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
		char lexeme = la->getElemLT(la->getLTsize() - 1)->getLex();
		this->ltIndex_ = la->getLTsize() - 1;
		this->dataType_ = la->getDataType(arrOfLines, i);
		this->type_ = la->getElemType(arrOfLines[i]);
		this->setFuncName(funcName);

		if (lexeme == LEX_LITERAL){
			_itoa_s(counter++, this->name_, 10);
			addPrefix(this->name_, AT_PREFIX_LITERAL);
			setValue(lexeme, arrOfLines[i]);
		}
		else if (lexeme == LEX_IDENTIFIER){
			this->setName(arrOfLines[i]);
			setValue(lexeme);
		}
		else if (lexeme == LEX_OPERATION){
			_itoa_s(counter++, this->name_, 10);
			addPrefix(this->name_, AT_PREFIX_OPERATION);
			setValue(lexeme, arrOfLines[i]);
		}
		else if (lexeme == LEX_COMPARE){
			_itoa_s(counter++, this->name_, 10);
			addPrefix(this->name_, AT_PREFIX_COMPARE);
			setValue(lexeme, arrOfLines[i]);
		};
	};

  	void Element::setValue(char lexeme, char* line){
		switch (lexeme){
		case LEX_LITERAL:
			if (this->dataType_ == AT::DATATYPE::NUM){
				this->setNumVal(std::atoi(line));
			}
			else if (this->dataType_ == AT::DATATYPE::LINE){
				line = createStrVal(line);
				this->setLineVal(line);
			};
			break;

		case LEX_IDENTIFIER:
			if (this->dataType_ == AT::DATATYPE::NUM){
				this->setNumVal(AT_NUM_DEFAULT);
			}
			else if (this->dataType_ == AT::DATATYPE::LINE){
				this->setLineVal(AT_LINE_DEFAULT);
			};
			break;

		case LEX_OPERATION: case LEX_COMPARE: this->setOtherVal(line); break;
		default: break;
		};
  	};

	void Element::reset(){
		memset(this->name_, -52, AT_NAME_MAXSIZE);
		this->dataType_ = AT::DATATYPE::UNKNOWN;
		this->type_ = AT::TYPE::U;
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
		if (this->size_ == NULL) return false;
		else{
			for (int i = 0; i < size_; i++){
				if ((!strcmp(this->table_[i].getName(), name)
					&& !strcmp(this->table_[i].getFuncName(), funcName))
					|| (!strcmp(this->table_[i].getFuncName(), name))){
						return true;
				};
			};
		};
		return false;
	};

	int Table::getIndex(char* name, char* funcName){
		if (this->size_ == NULL)	return -1;
		else{
			for (int i = 0; i < this->size_; i++){
				if (!strcmp(this->table_[i].getName(), name)
					&& !strcmp(this->table_[i].getFuncName(), funcName)){
						return i;
				};
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
		strncat(dest, temp, AT_NAME_MAXSIZE - strlen(prefix) - 1);
	};

	char* createStrVal(char* line){
		char* rc = new char;
		strncpy(rc, line + 1, (strlen(line) - 2));
		rc[static_cast<int> (strlen(line)) - 2] = NULL_STR;
		return rc;
	};
};