#include "stdafx.h"
#include "idTable.h"

namespace IT
{
	IdDataType::IdDataType()
	{
		char* tempName[] = TI_TYPES;
		IDDATATYPE tempType[TI_TYPES_SIZE] = TI_ID_TYPES;

		for (int i = 0; i < TI_TYPES_SIZE; i++)
		{
			name_.push_back(tempName[i]);
			type_.push_back(tempType[i]);
		};
	}

	Entry::Entry()
	{

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
		if (
			static_cast<int> (strlen(firstLine)) < TI_ID_MAXSIZE &&
			static_cast<int> (strlen(secondLine)) < TI_ID_MAXSIZE &&
			static_cast<int> (strlen(firstLine)) != static_cast<int> (strlen(secondLine))
			)
			return false;

		for (int i = 0; i < static_cast<int> (strlen(firstLine)) && i < static_cast<int> (strlen(secondLine)); i++)
		if (firstLine[i] != secondLine[i])	return false;

		return true;
	};

	IDDATATYPE getIdDataType(LT::LexTable* lexTable, char** arrOfLines, int i)
	{
		IDDATATYPE rc = IDDATATYPE::UNKNOWN;
		IdDataType* temp = new IdDataType();
		std::vector<char*>::iterator firstIt = temp->name_.begin();
		std::vector<IDDATATYPE>::iterator secondIt = temp->type_.begin();

		for (; firstIt != temp->name_.end(); firstIt++, secondIt++)
			if (isSimilar(*firstIt, arrOfLines[i - 1])) rc = *secondIt;

		delete temp;
		return rc;
	};

	char* getIdDataName(IDDATATYPE type)
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

	IDTYPE getIdType(LT::LexTable* lexTable, char* line)
	{
		IDTYPE rc = IT::IDTYPE::F;
		
		if (lexTable->table_[lexTable->size_ - 2].lexema_ != LEX_FUNCTION) rc = IDTYPE::V;

		return rc;
	};

	void setEntry(IT::Entry& entry, LT::LexTable* lexTable, int& ltIndex, char** arrOfLines, int& chainNumber)
	{

		entry.idxFirstLE_ = ltIndex;
		strncpy_s(entry.id_, arrOfLines[chainNumber], TI_ID_MAXSIZE - 1);

		entry.idDataType_ = IT::getIdDataType(lexTable, arrOfLines, chainNumber);
		entry.idType_ = IT::getIdType(lexTable, arrOfLines[chainNumber]);

		if (entry.idDataType_ == IT::IDDATATYPE::NUM) entry.value_.intValue_ = TI_INT_DEFAULT;
		else if (entry.idDataType_ == IT::IDDATATYPE::LINE)
		{
			entry.value_.vstr_.len_ = TI_STR_DEFAULT;
			entry.value_.vstr_.str_[0] = TI_STR_DEFAULT;
		};
	};

	void addPrefix(char* id, char* prefix)
	{
		char temp[TI_ARR_MAXSIZE];
		strcpy(temp, id);
		strncpy(id, prefix, TI_ID_FULL_MAXSIZE - 1);
		strncat(id, temp, TI_ID_FULL_MAXSIZE - strlen(prefix) - 1);
	};


	void createFuncName(char* funcName, char* line)
	{
		int size = static_cast<int> (strlen(line));
		strcpy(funcName, line);
		funcName[size] = TI_FUNC_SEP;
		funcName[size + 1] = NULL_STR;
	};
};