#include "stdafx.h"
#include "lexTable.h"

namespace LT
{
	Entry::Entry()
	{

	}

	LexTable::LexTable()
	{

	}

	LexTable::LexTable(int maxSize)
	{
		this->size_ = NULL;

		if (maxSize < LT_MAXSIZE) this->maxSize_ = maxSize;
		else throw ERROR_THROW(200);

		this->table_ = new Entry[this->maxSize_];
	}

	LexTable* create(int size)
	{
		return new LexTable(size);
	};

	void del(LexTable* lexTable)
	{
		delete[] lexTable->table_;
		delete lexTable;
	};

	void addElement(LexTable* lexTable, Entry entry)
	{
		lexTable->table_[lexTable->size_++] = entry;
	};

	Entry getEntry(LexTable& lexTable, int number)
	{
		return lexTable.table_[number];
	};

	char getLexem(int switchCount)
	{
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
		case 12:	rc = LEX_ACTIONS;				break;
		case 13:	rc = LEX_CONDITION;				break;
		case 14:	rc = LEX_EXTERN;				break;
		case 15:	rc = LEX_FUNCTION;				break;
		case 16:	rc = LEX_PRINT;					break;
		case 17:	rc = LEX_RETURN;				break;
		case 18:	rc = LEX_BEGIN;					break;
		case 19:	rc = LEX_ID;					break;
		case 20:	rc = LEX_LITERAL;			break;
		case 21:	rc = LEX_LITERAL;			break;
		default:	break;
		};

		return rc;
	};

	void setEntry(LT::Entry& entry, int& i, int& lineNumber, int itIndex)
	{
		entry.lexema_ = LT::getLexem(i);
		entry.lineNumber_ = lineNumber;

		if (entry.lexema_ == LEX_ID || entry.lexema_ == LEX_LITERAL) entry.TI_Index_ = itIndex;
		else entry.TI_Index_ = LT_TI_NULL_IDX;
	};
};