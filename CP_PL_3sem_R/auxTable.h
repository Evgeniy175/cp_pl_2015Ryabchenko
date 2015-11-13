#pragma once
#include "lexAnalyser.h"
#include "errors.h"
#include <vector>

#define AT_NAME_MAXSIZE		12					// max size of element name
#define AT_MAX_SIZE			4096				// max number of lines at auxiliary table
#define AT_LINE_MAXSIZE		255					// max size of line
#define AT_ARR_MAXSIZE		256					// max size of array
#define AT_NUM_DEFAULT		0xffffffff			// default value for num
#define AT_LINE_DEFAULT		0x00				// default value for line
#define AT_NULL_INDEX		0xffffffff			// default value for index
#define AT_NULL_OTHER_VALUE	"M"					// default value for operation
#define AT_PREFIX_LITERAL	"L"					// prefix of literal
#define AT_PREFIX_OPERATION	"O"					// prefix of operation
#define AT_PREFIX_COMPARE	"C"					// prefix of compare

#define AT_STL_FUNCSIZE	12
#define AT_STL_FUNCTIONS		{	"print",	"start",\
									"get_type", "get_load",	"get_temp", "get_time", "get_rpm",\
									"set_type", "set_load", "set_temp", "set_time", "set_rpm"}
#define AT_STL_FUNCTIONS_TYPES	{	AT::DATATYPE::NIL,	AT::DATATYPE::BOOL,\
	AT::DATATYPE::LINE, AT::DATATYPE::NUM, AT::DATATYPE::NUM, AT::DATATYPE::NUM, AT::DATATYPE::NUM, \
	AT::DATATYPE::NIL, AT::DATATYPE::NIL, AT::DATATYPE::NIL, AT::DATATYPE::NIL, AT::DATATYPE::NIL}

#define AT_PRIMITIVE_TYPES_SIZE	6
#define AT_PRIMITIVE_TYPES_NAMES { "unknown", "num", "line", "wash", "bool", "nil" }
#define AT_PRIMITIVE_TYPES	{ AT::DATATYPE::UNKNOWN, AT::DATATYPE::NUM, AT::DATATYPE::LINE,AT::DATATYPE::WASH, AT::DATATYPE::BOOL, AT::DATATYPE::NIL }

#define AT_ELEMENT_TYPES_SIZE 7
#define AT_ELEMENT_TYPES_NAMES { "unknown ", "variable", "parameter", "literal ", "function", "operation", "compare " }
#define AT_ELEMENT_TYPES { AT::TYPE::U, AT::TYPE::V, AT::TYPE::P, AT::TYPE::L, AT::TYPE::F, AT::TYPE::O, AT::TYPE::C }

#define AT_COMPARE_SIZE 6
#define AT_COMPARE_NAME { "==", ">=","<=", "!=", ">", "<" }
#define AT_COMPARE_VALUE {  AT::COMPARE::EQUAL, AT::COMPARE::EQUAL_OR_GREATER,\
	AT::COMPARE::EQUAL_OR_LESS, AT::COMPARE::NOT_EQUAL, \
	AT::COMPARE::GREATER, AT::COMPARE::LESS }

#define AT_OPERATION_SIZE 4
#define AT_OPERATION_NAME { "+", "-", "*", "/" }
#define AT_OPERATION_VALUE { AT::OPERATION::PLUS, AT::OPERATION::MINUS, AT::OPERATION::MULTIPLY, AT::OPERATION::DIVIDE }

namespace LA { class LexAnalyser; };

namespace AT{			// auxiliary table namespace
	enum SERVICE{			// service enum for universal values
		ERROR_VALUE = 0
	};

	enum DATATYPE{
		UNKNOWN = 0,		// unknown
		NUM,				// integer
		LINE,				// string
		WASH,				// wash
		BOOL,				// bool
		NIL					// like a void in C++
	};

	enum TYPE{
		U = 0,				// unknown
		V,					// variable
		P,					// parameter
		L,					// literal
		F,					// function
		O,					// operation
		C					// compare
	};

	enum COMPARE{
		EQUAL = 0,			// == equal
		EQUAL_OR_GREATER,	// >= equal or greater than
		EQUAL_OR_LESS,		// <= equal or less than
		NOT_EQUAL,			// != not equal
		LESS,				// <  less than
		GREATER				// >  greater than
	};

	enum OPERATION{
		PLUS = 0,				// +
		MINUS,				// -
		MULTIPLY,				// *
		DIVIDE				// /
	};

	class Info{		// types of data
	public:
		Info();

		char*					getName(DATATYPE);				// return name of primitive data type
		char*					getFuncName(DATATYPE);			// return name of function
		char*					getTypeName(TYPE);				// return name of type
		char*					getCompareName(COMPARE);		// return name of compare
		char*					getOperationName(OPERATION);	// return name of operation
		DATATYPE				getType(char* name);			// return DATATYPE value for primitive data type name
		DATATYPE				getFuncType(char* name);		// return DATATYPE value for function
		TYPE					getElemType(char* name);		// return TYPE value for type name
		COMPARE					getCompareValue(char* name);	// return COMPARE value for compare name
		OPERATION				getOperationValue(char* name);	// return OPERATION value for operation name

		void pushFuncName(char*);
		void pushFuncType(AT::DATATYPE);

		bool isNewFunction(char*);

	private:
		std::vector<char*>		name_;							// names of all primitive types
		std::vector<char*>		funcName_;						// names of all included functions
		std::vector<char*>		typeName_;						// names of all element types
		std::vector<char*>		compareName_;					// names of all compares
		std::vector<char*>		operationName_;					// names of all operations
		std::vector<DATATYPE>	typeValue_;						// data types of all primitive types
		std::vector<DATATYPE>	funcTypeValue_;					// data types of included functions
		std::vector<TYPE>		elemTypeValue_;					// types of elements
		std::vector<COMPARE>	compareValue_;					// compares values
		std::vector<OPERATION>	operationValue_;				// operations values
	};

	class Element{		// element of the auxTable
	public:
		Element();
		
		TYPE		getType();									// return element type
		DATATYPE	getDataType();								// return element data type
		char*		getName();									// return element name
		char*		getFuncName();								// return function name
		int			getIndex();									// return lexTable index
		int			getNumVal();								// return element num value
		char*		getLineVal();								// return element line value
		char*		getOtherVal();								// return element other value
		char		getOperation();								// return element operation value

		void		setIndex(int value);						// set lexTable index
		void		setName(char* name);						// set element name
		void		setFuncName(char* name);					// set element function name
		void		setValue(									// set element value
						char lexeme,
						char* line = NULL_STR);
		void		setNumVal(int value);						// set num element value
		void		setLineVal(char* value);					// set line element value
		void		setOtherVal(char* value);					// set element other value
		void		setElem(									// set element
						LA::LexAnalyser*	la,						// lexTable
						char*				funcName,				// function name
						char**				arrOfLines,				// array of lines
						int&				i,						// chain index number
						int					counter = -1			// counter
					);
		
		void		reset();									// set element fields to default values

	private:
		int			ltIndex_;									// index of first occurrence of this element in lexTable
		char		name_[AT_NAME_MAXSIZE];						// element name
		char		funcName_[AT_NAME_MAXSIZE];					// element function name
		TYPE		type_;										// element type
		DATATYPE	dataType_;									// element data type
		struct{
			int		numValue_;										// field for numerical values
			char	lineValue_[AT_ARR_MAXSIZE];						// field for line values
			char	otherValue_[AT_ARR_MAXSIZE];					// field for other values
		} value_;												// element value
	};

	class Table{		// auxiliary table
	public:
		Table();
		Table(int size);

		Info*		getInfo();									// return all names and types of functions and primitive types etc.
		Element*	getElem(int index);							// return element by index
		int			getSize();									// return size of auxiliary table

		void		addElem(Element& elem);						// add element to auxiliary table

		bool		isIncluded(char* name, char* funcName);		// is name&funcName element included into auxiliary table?
		int			getIndex(char* name, char* funcName);		// return index of element from auxiliary table

		~Table();

	private:

		int			maxSize_;									// max size of auxiliary table
		int			size_;										// current size of auxiliary table
		Info*		info_;										// consists all names and types of functions and primitive types etc.
		Element*	table_;										// auxiliary table that consist elements
	};

	void  addPrefix(char* dest, char* prefix);					// add prefix to the line
	char* createStrVal(char* line);								// delete quotes in line literal
};