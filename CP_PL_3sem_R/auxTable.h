#pragma once
#include "lexTable.h"
#include "errors.h"
#include <vector>

#define TI_ID_MAXSIZE		12				// ������������ ����� �������� � ����� ����������
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
#define TI_ID_TYPES	{ IT::DATATYPE::NUM, IT::DATATYPE::LINE, IT::DATATYPE::WASH, IT::DATATYPE::BOOL, IT::DATATYPE::NIL }

#define TI_STRUCT_TYPES_SIZE	5		// ������ ������� ����� ��� ���������
#define TI_STRUCT_NAMES { "type", "load", "temperature", "time", "rpm" }
#define TI_STRUCT_ID_TYPES { IT::DATATYPE::LINE, IT::DATATYPE::NUM, IT::DATATYPE::NUM, IT::DATATYPE::NUM, IT::DATATYPE::NUM, }

namespace IT		// �������������� �������
{
	enum TYPE{			// ����
		U = 0,				// unknown
		V = 1,				// ����������
		F = 2,				// ����
		P = 3,				// ��������
		L = 4,				// �������
		E = 5,				// extern function
		S = 6				// ������� ���������
	};

	enum DATATYPE{		// ���� ������
		NOTDEFINED = 0,		// unknown
		NUM  = 1,			// integer
		LINE = 2,			// string
		WASH = 3,			// wash
		BOOL = 4,			// bool
		NIL  = 5			// ������ void � C++
	};

	struct IdDataType			// ���� ������ ���������������
	{
		std::vector<char*>		name_;			// ��� ������� �����
		std::vector<char*>		structName_;	// ��� ���������
		std::vector<DATATYPE>	type_;			// ��� ������� �����
		std::vector<DATATYPE>	structType_;	// ��� ���������

		IdDataType();
	};

	class Element					// ������ �������������� �������
	{
	public:
		Element();

		int			getIdx();
		int			getIntVal();
		char*		getName();
		char*		getFuncName();
		char*		getStrVal();
		TYPE		getType();
		DATATYPE	getDataType();

		void		setIdx(int value);
		void		setIntVal(int value);
		void		setName(char* name);
		void		setFuncName(char* name);
		void		setStrVal(char* value);
		void		setType(TYPE type);
		void		setDataType(DATATYPE dataType);

	private:
		int			ltIndex_;					// ������ ������ ������ � ������� ������
		char		name_[TI_ID_MAXSIZE];		// ��� (������������� ��������� �� ID_MAXSIZE)
		char		funcName_[TI_ID_MAXSIZE];
		TYPE		type_;						// ��� ��������������
		DATATYPE	dataType_;					// ��� ������
		struct{
			int intValue_;
			char strValue_[TI_ARR_MAXSIZE];
		} value_;
	};

	struct IdTable					// ��������� ������� ���������������
	{
		int maxSize_;								// ������� ������� ��������������� < TI_MAXSIZE
		int size_;									// ������� ������ ������� ��������������� < maxsize
		Element* table_;							// ������ ����� ������� ���������������

		IdTable();
		IdTable(int size);

		bool isIncluded(char* line, char* funcName);				// ��� �� �������� ������������� �����
		int getIndex(char* name, char* funcName);
	};

	IdTable* create(				// ������� ������� ���������������
		int size									//������� ������� ��������������� < TI_MAXSIZE
		);

	void addElement(				// �������� ������ � ������� ���������������
		IdTable* idTable,							// ��������� ������� ���������������
		Element elem									// ������ ������� ���������������
		);

	Element getEntry(					// �������� ������ ������� ���������������
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

	DATATYPE getDataType(			// ���������� ��� ������ ��������������
		LT::LexTable* lexTable,						// ������� ������
		char** arrOfLines,							// ������ �������
		int i										// ����� ������� �������
		);

	TYPE getType(					// ���������� ��� ��������������
		LT::LexTable* lexTable						// ������� ������
		);

	char* getDataName(IT::DATATYPE type);

	void setEntry(					// ���������� �������� ��� ��������������
		IT::Element& elem,							// ������� ��� ����������
		LT::LexTable* lexTable,						// ������� ������
		char* funcName,
		char** arrOfLines,							// ������ �������
		int& i,										// ����� ������� �������
		TYPE idType = TYPE::U,						// ���
		int literalCounter = -1						// ������� ���������
		);

	void setValue(
		IT::Element& elem,							// �������
		char lexeme,
		char* line = NULL_STR
		);

	void addPrefix(					// ���������� ��������(����� �������) � ��������������
		char* dest,									// destination
		char* prefix								// prefix
		);

	void createFuncName(char* funcName, char* line);
	void reset(IT::Element& elem);
	bool isFunction(char* first, char* second);
	char* createStrVal(char* line);
};