#pragma once
#include "lexAnalyser.h"
#include "errors.h"
#include <vector>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 0 ��� ��������� �������� ���

#define AUX_NAME_MAXSIZE	12					// ������������ ����� �������� � ����� ����������
#define AUX_MAXSIZE			4096				// ������������ ���-�� ����� � ������� ���������������
#define AUX_NUM_DEFAULT		0x00000000			// �������� �� ��������� ��� num
#define AUX_LINE_DEFAULT	0x00				// �������� �� ��������� ��� line
#define AUX_NULLIDX			0xffffffff			// ��� ��-�� ������� ���������������
#define AUX_LINE_MAXSIZE	255					// 
#define AUX_ARR_MAXSIZE		256					//
#define AUX_LITERAL_PREFIX	"L"

#define AUX_DATA_SIZE	5				// ������ ������� �����
#define AUX_DATA_NAMES { "num", "line", "wash", "bool", "nil" }
#define AUX_DATA_TYPES	{ AUX::DATATYPE::NUM, AUX::DATATYPE::LINE, AUX::DATATYPE::WASH, AUX::DATATYPE::BOOL, AUX::DATATYPE::NIL }

#define AUX_DATASTRUCT_SIZE	5			// ������ ������� ����� ��� ���������
#define AUX_DATASTRUCT_NAMES { "type", "load", "temperature", "time", "rpm" }
#define AUX_DATASTRUCT_TYPES { AUX::DATATYPE::LINE, AUX::DATATYPE::NUM, AUX::DATATYPE::NUM, AUX::DATATYPE::NUM, AUX::DATATYPE::NUM }

namespace LA { class LexAnalyser; };

namespace AUX{		// �������������� �������

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
		char		name_[AUX_NAME_MAXSIZE];		// ��� (������������� ��������� �� ID_MAXSIZE)
		char		funcName_[AUX_NAME_MAXSIZE];
		TYPE		type_;						// ��� ��������������
		DATATYPE	dataType_;					// ��� ������
		struct{
			int		intValue_;
			char	strValue_[AUX_ARR_MAXSIZE];
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