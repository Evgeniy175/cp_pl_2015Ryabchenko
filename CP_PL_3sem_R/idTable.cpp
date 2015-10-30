#include "stdafx.h"
#include "idTable.h"

namespace IT
{
	IdDataType::IdDataType()
	{
		char* firstTempName[] = TI_TYPES;
		char* secondTempName[] = TI_STRUCT_NAMES;
		IDDATATYPE firstTempType[TI_TYPES_SIZE] = TI_ID_TYPES;
		IDDATATYPE secondTempType[TI_TYPES_SIZE] = TI_STRUCT_ID_TYPES;

		for (int i = 0; i < TI_TYPES_SIZE; i++)			// заполнение векторов для встроенных типов
		{
			name_.push_back(firstTempName[i]);
			type_.push_back(firstTempType[i]);
		};

		for (int i = 0; i < TI_STRUCT_TYPES_SIZE; i++)	// заполнение векторов для структурированного типа
		{
			structName_.push_back(secondTempName[i]);
			structType_.push_back(secondTempType[i]);
		};
	}

	Entry::Entry()
	{
		this->elementType_ = ELEMENTTYPE::UNKNOWN;
		this->idDataType_ = IDDATATYPE::NOTDEFINED;
		this->idType_ = IDTYPE::U;
	}

	IdTable::IdTable()
	{

	}

	IdTable::IdTable(int maxSize)
	{
		this->size_ = NULL;

		if (maxSize < TI_MAXSIZE) this->maxSize_ = maxSize;
		else throw ERROR_THROW(201);

		this->table_ = new Entry[maxSize];
	}

	IdTable* create(int size)
	{
		return new IdTable(size);
	};

	void del(IdTable* idTable)
	{
		delete[] idTable->table_;
		delete idTable;
	};

	bool IdTable::isIncluded(char* line)
	{
		if (this->size_ == NULL) return false;
		else
			for (int i = 0; i < size_; i++)
				if (isSimilar(this->table_[i].id_, line)) return true;

		return false;
	};

	void addElement(IdTable* idTable, Entry entry)
	{
		idTable->table_[idTable->size_++] = entry;
	};

	bool isSimilar(char* firstLine, char* secondLine)
	{
		if (static_cast<int> (strlen(firstLine)) != static_cast<int> (strlen(secondLine)))
			return false;

		for (int i = 0; i < static_cast<int> (strlen(firstLine)) && i < static_cast<int> (strlen(secondLine)); i++)
			if (firstLine[i] != secondLine[i])	return false;

		return true;
	};

	IDDATATYPE getDataType(LT::LexTable* lexTable, char** arrOfLines, int i)
	{
		IDDATATYPE rc = IDDATATYPE::NOTDEFINED;
		IdDataType* temp = new IdDataType();
		std::vector<char*>::iterator firstIt;
		std::vector<IDDATATYPE>::iterator secondIt;

		if (lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_COLON)
		{
			for (	firstIt = temp->structName_.begin(), secondIt = temp->structType_.begin();
					firstIt != temp->structName_.end(); firstIt++, secondIt++
				)
				if (isSimilar(*firstIt, arrOfLines[i])) rc = *secondIt;
		}
		else if (	lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_EQUALLY		||
					lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_COMMA			||
					lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_SQBRACEOPEN	||
					lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_RETURN
				)
				rc = arrOfLines[i][0] == '‘' ? IDDATATYPE::LINE : IDDATATYPE::NUM;
		else
		{
			for (	firstIt = temp->name_.begin(), secondIt = temp->type_.begin();
					firstIt != temp->name_.end(); firstIt++, secondIt++
				)
				if (isSimilar(*firstIt, arrOfLines[i - 1])) rc = *secondIt;
		}

		delete temp;
		return rc;
	};

	char* getDataName(IT::IDDATATYPE type)
	{
		char* rc = "unknown";
		IdDataType* temp = new IdDataType();
		std::vector<char*>::iterator firstIt = temp->name_.begin();
		std::vector<IDDATATYPE>::iterator secondIt = temp->type_.begin();

		for (; secondIt != temp->type_.end(); firstIt++, secondIt++)
			if (*secondIt == type) rc = *firstIt;

		delete temp;
		return rc;
	};

	IDTYPE getType(LT::LexTable* lexTable)
	{
		IDTYPE rc = IT::IDTYPE::U;
		
		if (lexTable->table_[lexTable->size_ - 2].lexema_ == LEX_FUNCTION)
			rc = lexTable->table_[lexTable->size_ - 3].lexema_ == LEX_EXTERN ? IDTYPE::E : IDTYPE::F;

		else if (	lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_SQBRACEOPEN ||
					lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_COMMA
				) rc = IDTYPE::P;
		else if (lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_TYPE) rc = IDTYPE::V;
		else if (lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_COLON) rc = IDTYPE::S;
		else if (	lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_EQUALLY	||
					lexTable->table_[lexTable->size_ - 1].lexema_ == LEX_RETURN
				) rc = IDTYPE::L;

		return rc;
	};

	void setEntry(IT::Entry& entry, LT::LexTable* lexTable, char** arrOfLines, int& chainNumber, IDTYPE idType, int literalCounter)
	{
		entry.idxFirstLE_ = lexTable->size_;
		entry.idDataType_ = IT::getDataType(lexTable, arrOfLines, chainNumber);
		entry.elementType_ = (idType == IDTYPE::L ? ELEMENTTYPE::LITERAL : ELEMENTTYPE::ID);
		entry.idType_ = IT::getType(lexTable);

		if (entry.elementType_ == IT::ELEMENTTYPE::LITERAL)
		{
			_itoa(literalCounter++, entry.id_, 10);
			addPrefix(entry.id_, TI_LIT_PREFIX);
			setValue(entry, arrOfLines[chainNumber]);
		}
		else if (entry.elementType_ == IT::ELEMENTTYPE::ID)
		{
			strncpy_s(entry.id_, arrOfLines[chainNumber], TI_ID_MAXSIZE - 1);
			setValue(entry);
		};

		
	};

	void setValue(IT::Entry& entry, char* line)
	{
		if (entry.elementType_ == IT::ELEMENTTYPE::LITERAL)
		{
			if (entry.idDataType_ == IT::IDDATATYPE::NUM)
				entry.value_.intValue_ = std::atoi(line);

			else if (entry.idDataType_ == IT::IDDATATYPE::LINE)
			{
				entry.value_.vstr_.len_ = strlen(line) - 2;
				strncpy_s(entry.value_.vstr_.str_, line + 1, entry.value_.vstr_.len_);
			};
		} 
		else if (entry.elementType_ == IT::ELEMENTTYPE::ID)
		{
			if (entry.idDataType_ == IT::IDDATATYPE::NUM)
				entry.value_.intValue_ = TI_INT_DEFAULT;

			else if (entry.idDataType_ == IT::IDDATATYPE::LINE)
			{
				entry.value_.vstr_.len_ = TI_STR_DEFAULT;
				entry.value_.vstr_.str_[0] = TI_STR_DEFAULT;
			};
		};
	};

	void addPrefix(char* dest, char* prefix)
	{
		char temp[TI_ARR_MAXSIZE];
		strcpy(temp, dest);
		strncpy(dest, prefix, TI_ID_FULL_MAXSIZE - 1);
		strncat(dest, temp, TI_ID_FULL_MAXSIZE - strlen(prefix) - 1);
	};


	void createFuncName(char* funcName, char* line)
	{
		int size = static_cast<int> (strlen(line));
		strcpy(funcName, line);
		funcName[size] = TI_FUNC_SEP;
		funcName[size + 1] = NULL_STR;
	};

	int IdTable::getIndex(char* line)
	{
		int rc = -1;

		if (this->size_ == NULL)	return rc;
		else 
			for (int i = 0; i < this->size_; i++)
				if (isSimilar(this->table_[i].id_, line))  return i;

		return rc;
	};

	void reset(IT::Entry& entry)
	{
		memset(entry.id_, -52, strlen(entry.id_));
		memset(entry.value_.vstr_.str_, -52, strlen(entry.value_.vstr_.str_));
		entry.idDataType_ = IT::IDDATATYPE::NOTDEFINED;
		entry.idType_ = IT::IDTYPE::U;
		entry.elementType_ = IT::ELEMENTTYPE::UNKNOWN;
		entry.idxFirstLE_ = -1;
		entry.value_.intValue_ = -1;
		entry.value_.vstr_.len_ = -1;
	};

	bool isFunction(char* first, char* second)
	{
		for (int i = 0; i < static_cast<int> (strlen(first)) && i < static_cast<int> (strlen(second)); i++)
			if (first[i] != second[i]) return false;

		return true;
	};
};