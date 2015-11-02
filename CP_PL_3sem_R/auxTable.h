#pragma once
#include "lexAnalyser.h"
#include "errors.h"
#include <vector>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 0 ��� ��������� �������� ���

#define AT_NAME_MAXSIZE	12					// ������������ ����� �������� � ����� ����������
#define AT_MAXSIZE			4096				// ������������ ���-�� ����� � ������� ���������������
#define AT_NUM_DEFAULT		0x00000000			// �������� �� ��������� ��� num
#define AT_LINE_DEFAULT	0x00				// �������� �� ��������� ��� line
#define AT_NULLIDX			0xffffffff			// ��� ��-�� ������� ���������������
#define AT_LINE_MAXSIZE	255					// 
#define AT_ARR_MAXSIZE		256					//
#define AT_LITERAL_PREFIX	"L"

#define AT_DATA_SIZE	5				// ������ ������� �����
#define AT_DATA_NAMES { "num", "line", "wash", "bool", "nil" }
#define AT_DATA_TYPES	{ AT::DATATYPE::NUM, AT::DATATYPE::LINE, AT::DATATYPE::WASH, AT::DATATYPE::BOOL, AT::DATATYPE::NIL }

#define AT_DATASTRUCT_SIZE	5			// ������ ������� ����� ��� ���������
#define AT_DATASTRUCT_NAMES { "type", "load", "temperature", "time", "rpm" }
#define AT_DATASTRUCT_TYPES { AT::DATATYPE::LINE, AT::DATATYPE::NUM, AT::DATATYPE::NUM, AT::DATATYPE::NUM, AT::DATATYPE::NUM }

namespace LA { class LexAnalyser; };

namespace AT{		// �������������� �������

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

	class DataStruct{		// ���� ������ ���������������
	public:
		DataStruct();

		std::vector<char*>&		getName();
		std::vector<char*>&		getStructName();
		std::vector<DATATYPE>&	getType();
		std::vector<DATATYPE>&	getStructType();

	private:
		std::vector<char*>		name_;			// ��� ������� �����
		std::vector<char*>		structName_;	// ��� ���������
		std::vector<DATATYPE>	type_;			// ��� ������� �����
		std::vector<DATATYPE>	structType_;	// ��� ���������
	};

	class Element{					// ������ �������������� �������
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
						LA::LexAnalyser* la,				// ������� ������
						char*			funcName,				// ��� �������
						char**			arrOfLines,				// ������ �������
						int&			i,						// ����� ������� �������
						TYPE			idType = TYPE::U,		// ���
						int				literalCounter = -1		// ������� ���������
					);

	private:
		int			ltIndex_;					// ������ ������ ������ � ������� ������
		char		name_[AT_NAME_MAXSIZE];		// ��� (������������� ��������� �� ID_MAXSIZE)
		char		funcName_[AT_NAME_MAXSIZE];
		TYPE		type_;						// ��� ��������������
		DATATYPE	dataType_;					// ��� ������
		struct{
			int		intValue_;
			char	strValue_[AT_ARR_MAXSIZE];
		} value_;
	};

	class Table{					// ��������� ������� ���������������
	public:
		Table();
		Table(int size);

		Element*	getElem(int i);
		int			getSize();
		DataStruct*	getDataStruct();

		void		addElem(Element elem);

		bool		isIncluded(char* line, char* funcName);
		int			getIdx(char* name, char* funcName);			// get index [i] in table_ for current name&funcName

		~Table();

	private:
		int maxSize_;								// ������������ ������ ������� ��������������� < TI_MAXSIZE
		int size_;									// ������� ������ ������� ��������������� < maxsize
		Element* table_;							// ������ ����� ������� ���������������
		DataStruct*	dataStruct_;
	};



	void addPrefix(char* dest, char* prefix);
	void createFuncName(char* funcName, char* line);
	bool isFunction(char* first, char* second);
	char* createStrVal(char* line);
};