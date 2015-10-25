#pragma once
#include "lexTable.h"
#include "errors.h"
#include <vector>

#define TI_ID_MAXSIZE		10				// ������������ ���-�� �������� � ��������������
#define TI_ID_FULL_MAXSIZE	32				// ������������ ������ ��������������+����� ����
#define TI_MAXSIZE		4096			// ������������ ���-�� ����� � ������� ���������������
#define TI_INT_DEFAULT	0x00000000		// �������� �� ��������� ��� integer
#define TI_STR_DEFAULT	0x00			// �������� �� ��������� ��� string
#define TI_NULLIDX		0xffffffff		// ��� ��-�� ������� ���������������
#define TI_STR_MAXSIZE	255				// 
#define TI_ARR_MAXSIZE	256
#define TI_FUNC_SEP		'$'

#define TI_TYPES_SIZE	3
#define TI_TYPES		{\
	"num",\
	"line",\
	"wash"\
}

#define TI_ID_TYPES		{\
	IT::IDDATATYPE::NUM, \
	IT::IDDATATYPE::LINE, \
	IT::IDDATATYPE::WASH \
}

namespace IT						// ������� ���������������
{
	enum IDDATATYPE	// ���� ������ ���������������
	{
		UNKNOWN = 0,
		NUM = 1,	// integer
		LINE = 2,	// string
		WASH = 3,		// wash
	};

	enum IDTYPE	// ���� ���������������
	{
		U = 0,  // uknown
		V = 1,	// ����������
		F = 2,	// ����
		P = 3,	// ��������
		L = 4	// �������
	};

	struct IdDataType				// ���� ������ ���������������
	{
		std::vector<char*>		name_;
		std::vector<IDDATATYPE> type_;
		IdDataType();
	};

	struct Entry					// ������ ������� ���������������
	{
		int			idxFirstLE_;					// ������ ������ ������ � ������� ������
		char		id_[TI_ID_FULL_MAXSIZE];		// ������������� (������������� ��������� �� ID_MAXSIZE)
		IDTYPE		idType_;						// ��� ��������������
		IDDATATYPE idDataType_;

		union
		{
			int intValue_;							// �������� integer

			struct
			{
				char len_;							// ���������� �������� � string
				char str_[TI_STR_MAXSIZE - 1];		// ������� string
			} vstr_;			// �������� string
		} value_;									// �������� ��������������

		Entry();
	};

	struct IdTable					// ��������� ������� ���������������
	{
		int maxSize_;								// ������� ������� ��������������� < TI_MAXSIZE
		int size_;									// ������� ������ ������� ��������������� < maxsize
		Entry* table_;								// ������ ����� ������� ���������������

		IdTable();
		IdTable(int size);

		bool isIncluded(char* line);		// ��� �� �������� ������������� �����
	};

	IdTable* create(					// ������� ������� ���������������
		int size									//������� ������� ��������������� < TI_MAXSIZE
		);

	void addElement(						// �������� ������ � ������� ���������������
		IdTable* idTable,							// ��������� ������� ���������������
		Entry entry									// ������ ������� ���������������
		);

	Entry getEntry(					// �������� ������ ������� ���������������
		IdTable& idTable,							// ��������� ������� ���������������
		int n										// ����� ���������� ������
		);

	int isId(						// �������: ����� ������(���� ����), TI_NULLIDX(���� ���)
		IdTable& idTable,							// ��������� ������� ���������������
		char id[TI_ID_MAXSIZE]							// �������������
		);

	void del(IdTable* idTable);								// ������� ������� ���������������(���������� ������)
	bool isSimilar(char* firstLine, char* secondLine);		// ���������� �� �������

	IDDATATYPE getIdDataType(		// ���������� ��� ������ ��������������
		LT::LexTable* lexTable,						// ������� ������
		char** arrOfLines,							// ������ �������
		int i										// ����� ������� �������
		);

	IDDATATYPE getType(
		char* line
		);

	IDTYPE getIdType(				// ���������� ��� ��������������
		LT::LexTable* lexTable,						// ������� ������
		char* line									// ������� �������
		);

	char* getIdDataName(IDDATATYPE type);
	
	void setEntry(					// ���������� �������� �������
		IT::Entry& entry,							// ������� ��� ����������
		LT::LexTable* lexTable,						// ������� ������
		int& ltIndex,							// ������� ������
		char** arrOfLines,							// ������ �������
		int& chainNumber							// ����� ������� �������
		);

	void addPrefix(					// ���������� ��������(����� �������) � ��������������
		char* id,									// �������������
		char* prefix								// ��� �������
		);
	
	void createFuncName(char* funcName, char* line);
};