#include "stdafx.h"
#include "auxTable.h"

namespace IT
{	
	Element::Element(){
		this->dataType_ = DATATYPE::UNKNOWN;
		this->type_ = TYPE::U;
		setIntVal(AUX_INT_DEFAULT);
		setStrVal(AUX_STR_DEFAULT);
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
		strncpy_s(this->name_, name, static_cast<int> (strlen(name)));
	};

	void Element::setFuncName(char* name){
		strncpy_s(this->funcName_, name, static_cast<int> (strlen(name)));
	};
	
	void Element::setStrVal(char* value){
		if (value != NULL)
			strcpy(this->value_.strValue_, value);

		else
			memset(this->value_.strValue_, AUX_ARR_MAXSIZE, 'M');
	};

	void Element::setElem(LT::LexTable* lt, char* funcName,
		char** arrOfLines, int& i, TYPE idType, int literalCounter){
		char lexeme = lt->getElem(lt->getSize() - 1)->getLex();
		this->ltIndex_ = lt->getSize();
		this->dataType_ = IT::getDataType(lt, arrOfLines, i);
		this->type_ = IT::getType(lt);
		this->setFuncName(funcName);

		if (lexeme == LEX_LITERAL) {
			_itoa(literalCounter++, this->name_, 10);
			addPrefix(this->name_, AUX_LITERAL_PREFIX);
			setValue(lexeme, arrOfLines[i]);
		}
		else if (lexeme == LEX_ID) {
			this->setName(arrOfLines[i]);
			setValue(lexeme);
		};
	};

	void Element::setValue(char lexeme, char* line){
		if (lexeme == LEX_LITERAL) {
			if (this->dataType_ == IT::DATATYPE::NUM)
				this->setIntVal(std::atoi(line));

			else if (this->dataType_ == IT::DATATYPE::LINE){
				line = createStrVal(line);
				this->setStrVal(line);
			};
		}
		else if (lexeme == LEX_ID){
			if (this->dataType_ == IT::DATATYPE::NUM)
				this->setIntVal(AUX_INT_DEFAULT);

			else if (this->dataType_ == IT::DATATYPE::LINE) {
				this->setStrVal(AUX_STR_DEFAULT);
			};
		};
	};

	void Element::reset(){
		memset(this->name_, -52, AUX_NAME_MAXSIZE);
		this->dataType_ = IT::DATATYPE::UNKNOWN;
		this->type_ = IT::TYPE::U;
		this->ltIndex_ = AUX_NULLIDX;
		this->setIntVal(AUX_INT_DEFAULT);
		this->setStrVal(AUX_STR_DEFAULT);
	};

	AuxTable::AuxTable() { }

	AuxTable::AuxTable(int maxSize){
		this->size_ = NULL;

		if (maxSize < AUX_MAXSIZE) this->maxSize_ = maxSize;
		else throw ERROR_THROW(201);

		this->table_ = new Element[maxSize];
	}

	Element* AuxTable::getElem(int i){
		return (table_ + i);
	};

	int AuxTable::getSize(){
		return this->size_;
	};

	void AuxTable::addElem(Element elem){
		this->table_[this->size_++] = elem;
	};

	IdDataType::IdDataType(){
		char* firstTempName[] = TI_TYPES;
		char* secondTempName[] = TI_STRUCT_NAMES;
		DATATYPE firstTempType[TI_TYPES_SIZE] = TI_ID_TYPES;
		DATATYPE secondTempType[TI_TYPES_SIZE] = TI_STRUCT_ID_TYPES;

		for (int i = 0; i < TI_TYPES_SIZE; i++){			// заполнение векторов для встроенных типов
			name_.push_back(firstTempName[i]);
			type_.push_back(firstTempType[i]);
		};

		for (int i = 0; i < TI_STRUCT_TYPES_SIZE; i++){	// заполнение векторов для структурированного типа
			structName_.push_back(secondTempName[i]);
			structType_.push_back(secondTempType[i]);
		};
	}

	

	

	bool AuxTable::isIncluded(char* line, char* funcName){
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

	int AuxTable::getIdx(char* name, char* funcName){
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

	AuxTable::~AuxTable(){
		delete[] this->table_;
	};

	DATATYPE getDataType(LT::LexTable* lt, char** arrOfLines, int i){
		DATATYPE rc = DATATYPE::UNKNOWN;
		IdDataType* temp = new IdDataType();
		std::vector<char*>::iterator firstIt;
		std::vector<DATATYPE>::iterator secondIt;

		if (lt->getElem(lt->getSize() - 2)->getLex() == LEX_COLON){
			for (	firstIt = temp->structName_.begin(), secondIt = temp->structType_.begin();
					firstIt != temp->structName_.end(); firstIt++, secondIt++){
						if (strcmp(*firstIt, arrOfLines[i]) == NULL) rc = *secondIt;
			};
		}
		else if (lt->getElem(lt->getSize() - 2)->getLex() == LEX_EQUALLY
					|| lt->getElem(lt->getSize() - 2)->getLex() == LEX_COMMA
					|| lt->getElem(lt->getSize() - 2)->getLex() == LEX_SQBRACEOPEN
					|| lt->getElem(lt->getSize() - 2)->getLex() == LEX_RETURN) {
							rc = arrOfLines[i][0] == '‘' ? DATATYPE::LINE : DATATYPE::NUM;
		}
		else {
			for (firstIt = temp->name_.begin(), secondIt = temp->type_.begin();
					firstIt != temp->name_.end(); firstIt++, secondIt++) {
						if (strcmp(*firstIt, arrOfLines[i - 1]) == NULL) rc = *secondIt;
			};
		};

		delete temp;
		return rc;
	};

	char* getDataName(IT::DATATYPE type){
		char* rc = "unknown";
		IdDataType* temp = new IdDataType();
		std::vector<char*>::iterator firstIt = temp->name_.begin();
		std::vector<DATATYPE>::iterator secondIt = temp->type_.begin();

		for (; secondIt != temp->type_.end(); firstIt++, secondIt++)
			if (*secondIt == type) rc = *firstIt;

		delete temp;
		return rc;
	};

	TYPE getType(LT::LexTable* lt){
		TYPE rc = IT::TYPE::U;

		if (lt->getElem(lt->getSize() - 3)->getLex() == LEX_FUNCTION)
			rc = lt->getElem(lt->getSize() - 4)->getLex() == LEX_EXTERN ? TYPE::E : TYPE::F;

		else {
			switch (lt->getElem(lt->getSize() - 2)->getLex())
			{
			case LEX_COMMA: case LEX_SQBRACEOPEN: rc = TYPE::P; break;
			case LEX_TYPE: rc = TYPE::V; break;
			case LEX_COLON: rc = TYPE::S; break;
			case LEX_RETURN: case LEX_EQUALLY: rc = TYPE::L; break;
			default: break;
			};
		}

		return rc;
	};

	

	void addPrefix(char* dest, char* prefix){
		char temp[AUX_ARR_MAXSIZE];
		strcpy(temp, dest);
		strncpy(dest, prefix, AUX_NAME_MAXSIZE - 1);
		strncat(dest, temp, AUX_NAME_MAXSIZE - strlen(prefix) - 1);
	};

	bool isFunction(char* first, char* second){
		for (int i = 0; i < static_cast<int> (strlen(first))
			&& i < static_cast<int> (strlen(second)); i++){
			if (first[i] != second[i]) return false;
		};
		return true;
	};

	char* createStrVal(char* line){
		char* rc = new char;
		strncpy(rc, line + 1, static_cast<int> (strlen(line)) - 2);
		rc[static_cast<int> (strlen(line)) - 2] = NULL_STR;
		return rc;
	};
};