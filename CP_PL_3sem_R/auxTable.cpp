#include "stdafx.h"
#include "auxTable.h"

namespace AT{
	DataInfo::DataInfo(){
		int i;
		char* nameTemp[] = AT_PRIMITIVE_TYPES_NAMES;
		char* funcNameTemp[] = AT_STL_FUNCTIONS;
		DATATYPE typeTemp[AT_PRIMITIVE_TYPES_SIZE] = AT_PRIMITIVE_TYPES_TYPES;
		DATATYPE funcTypeTemp[AT_STL_FUNCSIZE] = AT_STL_FUNCTYPES;

		for (i = 0; i < AT_PRIMITIVE_TYPES_SIZE; i++){			// заполнение векторов для встроенных типов
			this->name_.push_back(nameTemp[i]);
			this->type_.push_back(typeTemp[i]);
		};
		for (i = 0; i < AT_STL_FUNCSIZE; i++){
			this->funcName_.push_back(funcNameTemp[i]);
			this->funcType_.push_back(funcTypeTemp[i]);
		};
	};

	std::vector<char*>& DataInfo::getName(){
		return this->name_;
	};

	std::vector<char*>& DataInfo::getFuncName(){
		return this->funcName_;
	};

	std::vector<DATATYPE>& DataInfo::getType(){
		return this->type_;
	};

	std::vector<DATATYPE>&	DataInfo::getFuncType(){
		return this->funcType_;
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
  		if (lexeme == LEX_LITERAL){
  			if (this->dataType_ == AT::DATATYPE::NUM){
  				this->setNumVal(std::atoi(line));
  			}
  			else if (this->dataType_ == AT::DATATYPE::LINE){
  				line = createStrVal(line);
  				this->setLineVal(line);
  			};
  		}
  		else if (lexeme == LEX_IDENTIFIER){
  			if (this->dataType_ == AT::DATATYPE::NUM){
  				this->setNumVal(AT_NUM_DEFAULT);
  			}
  			else if (this->dataType_ == AT::DATATYPE::LINE){
  				this->setLineVal(AT_LINE_DEFAULT);
  			};
  		}
  		else if (lexeme == LEX_OPERATION){
  			this->setOtherVal(line);
  		}
		else if (lexeme == LEX_COMPARE){
			this->setOtherVal(line);
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
		this->dataInfo_ = new DataInfo();
	}

	Table::Table(int maxSize){
		this->dataInfo_ = new DataInfo();
		this->size_ = NULL;

		if (maxSize < AT_MAX_SIZE)
			this->maxSize_ = maxSize;

		else
			throw ERROR_THROW(201);

		this->table_ = new Element[maxSize];
	}

	DataInfo* Table::getDataInfo(){
		return this->dataInfo_;
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
		int rc = -1;
		int i;
		if (this->size_ == NULL)	return rc;
		else{
			for (i = 0; i < this->size_; i++){
				if (!strcmp(this->table_[i].getName(), name)
					&& !strcmp(this->table_[i].getFuncName(), funcName)){
						return i;
				};
			};
		};
		return rc;
	};

	Table::~Table(){
		delete[] this->table_;
		delete this->dataInfo_;
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