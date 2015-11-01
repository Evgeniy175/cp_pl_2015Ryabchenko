#pragma once
#include "lexTable.h"
#include "errors.h"
#include <vector>

#define AUX_NAME_MAXSIZE	12					// ������������ ����� �������� � ����� ����������
#define AUX_MAXSIZE			4096				// ������������ ���-�� ����� � ������� ���������������
#define AUX_INT_DEFAULT		0x00000000			// �������� �� ��������� ��� integer
#define AUX_STR_DEFAULT		0x00				// �������� �� ��������� ��� string
#define AUX_NULLIDX			0xffffffff			// ��� ��-�� ������� ���������������
#define AUX_STR_MAXSIZE		255					// 
#define AUX_ARR_MAXSIZE		256					//
#define AUX_LITERAL_PREFIX	"L"

#define TI_TYPES_SIZE	5				// ������ ������� �����
#define TI_TYPES { "num", "line", "wash", "bool", "nil" }
#define TI_ID_TYPES	{ IT::DATATYPE::NUM, IT::DATATYPE::LINE, IT::DATATYPE::WASH, IT::DATATYPE::BOOL, IT::DATATYPE::NIL }

#define TI_STRUCT_TYPES_SIZE	5		// ������ ������� ����� ��� ���������
#define TI_STRUCT_NAMES { "type", "load", "temperature", "time", "rpm" }
#define TI_STRUCT_ID_TYPES { IT::DATATYPE::LINE, IT::DATATYPE::NUM, IT::DATATYPE::NUM, IT::DATATYPE::NUM, IT::DATATYPE::NUM }

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
		UNKNOWN = 0,		// unknown
		NUM  = 1,			// integer
		LINE = 2,			// string
		WASH = 3,			// wash
		BOOL = 4,			// bool
		NIL  = 5			// ������ void � C++
	};

	struct	IdDataType		// ���� ������ ���������������
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

		void reset();

		int			getIdx();				// get lexTable index
		int			getIntVal();
		char*		getName();
		char*		getFuncName();			// get name of function
		char*		getStrVal();
		TYPE		getType();
		DATATYPE	getDataType();

		void		setIdx(int value);		// set lexTable index
		void		setIntVal(int value);
		void		setName(char* name);
		void		setFuncName(char* name);
		void		setStrVal(char* value);
		void		setValue(char lexeme, char* line = NULL_STR);
		void		setElem(						// ���������� �������� ��� ��������������
						LT::LexTable*	lexTable,				// ������� ������
						char*			funcName,				// ��� �������
						char**			arrOfLines,				// ������ �������
						int&			i,						// ����� ������� �������
						TYPE			idType = TYPE::U,		// ���
						int				literalCounter = -1		// ������� ���������
					);

	private:
		int			ltIndex_;					// ������ ������ ������ � ������� ������
		char		name_[AUX_NAME_MAXSIZE];		// ��� (������������� ��������� �� ID_MAXSIZE)
		char		funcName_[AUX_NAME_MAXSIZE];
		TYPE		type_;						// ��� ��������������
		DATATYPE	dataType_;					// ��� ������
		struct{
			int		intValue_;
			char	strValue_[AUX_ARR_MAXSIZE];
		} value_;
	};

	class AuxTable					// ��������� ������� ���������������
	{
	public:
		AuxTable();
		AuxTable(int size);

		Element*	getElem(int i);
		int			getSize();

		void		addElem(Element elem);

		bool		isIncluded(char* line, char* funcName);
		int			getIdx(char* name, char* funcName);			// get index [i] in table_ for current name&funcName

		~AuxTable();
	private:
		int maxSize_;								// ������������ ������ ������� ��������������� < TI_MAXSIZE
		int size_;									// ������� ������ ������� ��������������� < maxsize
		Element* table_;							// ������ ����� ������� ���������������
	};



	DATATYPE getDataType(LT::LexTable* lt, char** arrOfLines, int chainNumber);

	TYPE getType(LT::LexTable* lexTable);	// ���������� ��� ��������������
	char* getDataName(IT::DATATYPE type);
	void addPrefix(char* dest, char* prefix);
	void createFuncName(char* funcName, char* line);
	bool isFunction(char* first, char* second);
	char* createStrVal(char* line);
};