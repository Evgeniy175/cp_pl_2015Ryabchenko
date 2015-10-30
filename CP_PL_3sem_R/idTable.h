#pragma once
#include "lexTable.h"
#include "errors.h"
#include <vector>

#define TI_ID_MAXSIZE		12				// ������������ ���-�� �������� � ��������������
#define TI_ID_FULL_MAXSIZE	16				// ������������ ������ ��������������+����� ����
#define TI_MAXSIZE		4096				// ������������ ���-�� ����� � ������� ���������������
#define TI_INT_DEFAULT	0x00000000			// �������� �� ��������� ��� integer
#define TI_STR_DEFAULT	0x00				// �������� �� ��������� ��� string
#define TI_NULLIDX		0xffffffff			// ��� ��-�� ������� ���������������
#define TI_STR_MAXSIZE	255					// 
#define TI_ARR_MAXSIZE	256					// 
#define TI_FUNC_SEP		'$'					// ���������, ����������� ����� �������� � ������ ����������
#define TI_LIT_PREFIX	"L"

#define TI_TYPES_SIZE	5				// ������ ������� �����
#define TI_TYPES { "num", "line", "wash", "bool", "nil" }
#define TI_ID_TYPES	{ IT::IDDATATYPE::NUM, IT::IDDATATYPE::LINE, IT::IDDATATYPE::WASH, IT::IDDATATYPE::BOOL, IT::IDDATATYPE::NIL }

#define TI_STRUCT_TYPES_SIZE	5		// ������ ������� ����� ��� ���������
#define TI_STRUCT_NAMES { "type", "load", "temperature", "time", "rpm" }
#define TI_STRUCT_ID_TYPES { IT::IDDATATYPE::LINE, IT::IDDATATYPE::NUM, IT::IDDATATYPE::NUM, IT::IDDATATYPE::NUM, IT::IDDATATYPE::NUM, }

namespace IT		// ������� ���������������
{
	enum IDDATATYPE	// ���� ������ ���������������
	{
		NOTDEFINED = 0,	// uknown
		NUM	 = 1,		// integer
		LINE = 2,		// string
		WASH = 3,		// wash
		BOOL = 4,		// bool
		NIL  = 5		// ������ void � C++
	};

	enum IDTYPE		// ���� ���������������
	{
		U = 0,			// unknown
		V = 1,			// ����������
		F = 2,			// ����
		P = 3,			// ��������
		L = 4,			// �������
		E = 5,			// extern function
		S = 6			// ������� ���������
	};

	enum ELEMENTTYPE
	{
		UNKNOWN = 0,	// unknown
		ID = 1,			// �������������
		LITERAL = 2,	// �������
		ACTION = 3		// �������������� ��������
	};

	struct IdDataType			// ���� ������ ���������������
	{
		std::vector<char*>		name_;			// ��� ������� �����
		std::vector<char*>		structName_;	// ��� ���������
		std::vector<IDDATATYPE> type_;			// ��� ������� �����
		std::vector<IDDATATYPE>	structType_;	// ��� ���������
		
		IdDataType();
	};

	struct Entry					// ������ ������� ���������������
	{
		int			idxFirstLE_;					// ������ ������ ������ � ������� ������
		char		id_[TI_ID_FULL_MAXSIZE];		// ��� (������������� ��������� �� ID_MAXSIZE)
		IDTYPE		idType_;						// ��� ��������������
		IDDATATYPE	idDataType_;					// ��� ������
		ELEMENTTYPE elementType_;					// ��� ��������

		union
		{
			int intValue_;							// �������� integer

			struct
			{
				int len_;							// ���������� �������� � string
				char str_[TI_STR_MAXSIZE];			// ������� string
			} vstr_;								// �������� string
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

		bool isIncluded(char* line);				// ��� �� �������� ������������� �����
		int getIndex(char* line);
	};

	IdTable* create(				// ������� ������� ���������������
		int size									//������� ������� ��������������� < TI_MAXSIZE
		);

	void addElement(				// �������� ������ � ������� ���������������
		IdTable* idTable,							// ��������� ������� ���������������
		Entry entry									// ������ ������� ���������������
		);

	Entry getEntry(					// �������� ������ ������� ���������������
		IdTable& idTable,							// ��������� ������� ���������������
		int n										// ����� ���������� ������
		);

	int isId(						// �������: ����� ������(���� ����), TI_NULLIDX(���� ���)
		IdTable& idTable,							// ��������� ������� ���������������
		char id[TI_ID_MAXSIZE]						// �������������
		);

	void del(IdTable* idTable);		// ������� ������� ���������������(���������� ������)

	bool isSimilar(					// ���������� �� �������
		char* firstLine,
		char* secondLine
		);		

	IDDATATYPE getDataType(			// ���������� ��� ������ ��������������
		LT::LexTable* lexTable,						// ������� ������
		char** arrOfLines,							// ������ �������
		int i										// ����� ������� �������
		);

	IDTYPE getType(					// ���������� ��� ��������������
		LT::LexTable* lexTable						// ������� ������
		);

	char* getDataName(IT::IDDATATYPE type);
	
	void setEntry(					// ���������� �������� ��� ��������������
		IT::Entry& entry,							// ������� ��� ����������
		LT::LexTable* lexTable,						// ������� ������
		char** arrOfLines,							// ������ �������
		int& chainNumber,							// ����� ������� �������
		IDTYPE idType = IDTYPE::U,					// ���
		int literalCounter = -1						// ������� ���������
		);

	void setValue(
		IT::Entry& entry,							// �������
		char* line = NULL_STR
		);

	void addPrefix(					// ���������� ��������(����� �������) � ��������������
		char* dest,									// destination
		char* prefix								// prefix
		);
	
	void createFuncName(char* funcName, char* line);
	void reset(IT::Entry& entry);
	bool isFunction(char* first, char* second);
};