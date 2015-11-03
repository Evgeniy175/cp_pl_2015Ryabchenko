#include "stdafx.h"
#include "auxTable.h"

namespace AT
{	
	Element::Element(){
		this->dataType_ = DATATYPE::UNKNOWN;
		this->type_ = TYPE::U;
		setIntVal(AT_NUM_DEFAULT);
		setStrVal(AT_LINE_DEFAULT);
	}

	int	Element::getIdx(){
		return this->ltIndex_;
	};

	int Element::getIntVal(){
		return this->value_.intValue_;
	};

	char* Element::getName(){
		return this->name_;
	};

	char* Element::getFuncName(){
		return this->funcName_;
	};

	char* Element::getStrVal(){
		return this->value_.strValue_;
	};

	TYPE Element::getType(){
		return this->type_;
	};

	DATATYPE Element::getDataType(){
		return this->dataType_;
	};

	void Element::setIdx(int value){
		this->ltIndex_ = value;
	};

	void Element::setIntVal(int value){
		this->value_.intValue_ = value;
	};

	void Element::setName(char* name){
		strncpy_s(this->name_, name, strlen(name));
	};

	void Element::setFuncName(char* name){
		strncpy_s(this->funcName_, name, strlen(name));
	};
	
	void Element::setStrVal(char* value){
		if (value != NULL)
			strcpy(this->value_.strValue_, value);
		
		else
			memset(this->value_.strValue_, AT_ARR_MAXSIZE, 'M');
	};

	void Element::setElem(LA::LexAnalyser* la, char* funcName,
		char** arrOfLines, int& i, TYPE idType, int literalCounter){
		char lexeme = la->getLT()->getElem(la->getLT()->getSize() - 1)->getLex();
		this->ltIndex_ = la->getLT()->getSize();
		this->dataType_ = la->getDataType(arrOfLines, i);
		this->type_ = la->getLT()->getType();
		this->setFuncName(funcName);

		if (lexeme == LEX_LITERAL) {
			_itoa(literalCounter++, this->name_, 10);
			addPrefix(this->name_, AT_LITERAL_PREFIX);
			setValue(lexeme, arrOfLines[i]);
		}
		else if (lexeme == LEX_ID) {
			this->setName(arrOfLines[i]);
			setValue(lexeme);
		};
	};

	void Element::setValue(char lexeme, char* line){
		if (lexeme == LEX_LITERAL) {
			if (this->dataType_ == AT::DATATYPE::NUM){
				this->setIntVal(std::atoi(line));
			}
			else if (this->dataType_ == AT::DATATYPE::LINE){
				line = createStrVal(line);
				this->setStrVal(line);
			};
		}
		else if (lexeme == LEX_ID){
			if (this->dataType_ == AT::DATATYPE::NUM){
				this->setIntVal(AT_NUM_DEFAULT);
			}
			else if (this->dataType_ == AT::DATATYPE::LINE) {
				this->setStrVal(AT_LINE_DEFAULT);
			};
		};
	};

	void Element::reset(){
		memset(this->name_, -52, AT_NAME_MAXSIZE);
		this->dataType_ = AT::DATATYPE::UNKNOWN;
		this->type_ = AT::TYPE::U;
		this->ltIndex_ = AT_NULLIDX;
		this->setIntVal(AT_NUM_DEFAULT);
		this->setStrVal(AT_LINE_DEFAULT);
	};

	DataStruct::DataStruct(){
		char* firstTempName[] = AT_DATA_NAMES;
		char* secondTempName[] = AT_DATASTRUCT_NAMES;
		DATATYPE firstTempType[AT_DATA_SIZE] = AT_DATA_TYPES;
		DATATYPE secondTempType[AT_DATA_SIZE] = AT_DATASTRUCT_TYPES;

		for (int i = 0; i < AT_DATA_SIZE; i++){			// заполнение векторов для встроенных типов
			this->name_.push_back(firstTempName[i]);
			this->type_.push_back(firstTempType[i]);
		};

		for (int i = 0; i < AT_DATASTRUCT_SIZE; i++){	// заполнение векторов для структурированного типа
			this->structName_.push_back(secondTempName[i]);
			this->structType_.push_back(secondTempType[i]);
		};
	};

	std::vector<char*>& DataStruct::getName(){
		return this->name_;
	};

	std::vector<char*>& DataStruct::getStructName(){
		return this->structName_;
	};

	std::vector<DATATYPE>& DataStruct::getType(){
		return this->type_;
	};

	std::vector<DATATYPE>& DataStruct::getStructType(){
		return this->structType_;
	};

	Table::Table(){
		this->dataStruct_ = new DataStruct();
	}

	Table::Table(int maxSize){
		this->dataStruct_ = new DataStruct();
		this->size_ = NULL;

		if (maxSize < AT_MAXSIZE)
			this->maxSize_ = maxSize;

		else
			throw ERROR_THROW(201);

		this->table_ = new Element[maxSize];
	}

	Element* Table::getElem(int i){
		return (table_ + i);
	};

	int Table::getSize(){
		return this->size_;
	};

	DataStruct* Table::getDataStruct(){
		return this->dataStruct_;
	};

	void Table::addElem(Element& elem){
		this->table_[this->size_++] = elem;
	};

	bool Table::isIncluded(char* line, char* funcName){
		if (this->size_ == NULL) return false;
		else {
			for (int i = 0; i < size_; i++){
				if (strcmp(this->table_[i].getName(), line) == NULL
					&& strcmp(this->table_[i].getFuncName(), funcName) == NULL){
						return true;
				};
			};
		};
		return false;
	};

	int Table::getIdx(char* name, char* funcName){
		int rc = -1;

		if (this->size_ == NULL)	return rc;
		else {
			for (int i = 0; i < this->size_; i++) {
				if (strcmp(this->table_[i].getName(), name) == NULL
					&& strcmp(this->table_[i].getFuncName(), funcName) == NULL) {
						return i;
				};
			};
		};
		return rc;
	};

	Table::~Table(){
		delete[] this->table_;
	};

	void addPrefix(char* dest, char* prefix){
		char temp[AT_ARR_MAXSIZE];
		strcpy(temp, dest);
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