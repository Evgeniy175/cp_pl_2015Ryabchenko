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
#define AT_STL_FUNCTIONS {	"print",	"start",\
							"get_type", "get_load",	"get_temp", "get_time", "get_rpm",\
							"set_type", "set_load", "set_temp", "set_time", "set_rpm"}
#define AT_STL_FUNCTYPES {	AT::DATATYPE::NIL, AT::DATATYPE::BOOL,\
							AT::DATATYPE::LINE, AT::DATATYPE::NUM, AT::DATATYPE::NUM, AT::DATATYPE::NUM, AT::DATATYPE::NUM, \
							AT::DATATYPE::NIL,  AT::DATATYPE::NIL, AT::DATATYPE::NIL, AT::DATATYPE::NIL, AT::DATATYPE::NIL}

#define AT_PRIMITIVE_TYPES_SIZE	5
#define AT_PRIMITIVE_TYPES_NAMES { "num", "line", "wash", "bool", "nil" }
#define AT_PRIMITIVE_TYPES_TYPES	{ AT::DATATYPE::NUM, AT::DATATYPE::LINE,AT::DATATYPE::WASH, AT::DATATYPE::BOOL, AT::DATATYPE::NIL }

namespace LA { class LexAnalyser; };

namespace AT{			// auxiliary table namespace
	enum TYPE{
		U,					// unknown
		V,					// variable
		P,					// parameter
		L,					// literal
		F,					// function
		O,					// operation
		C					// compare
	};

	enum DATATYPE{
		UNKNOWN,			// unknown
		NUM,				// integer
		LINE,				// string
		WASH,				// wash
		BOOL,				// bool
		NIL					// like a void in C++
	};

	enum COMPARE{
		EQUAL,				// == equal
		EQUAL_OR_GREATER,	// >= equal or greater than
		EQUAL_OR_LESS,		// <= equal or less than
		NOT_EQUAL,			// != not equal
		LESS,				// <  less than
		GREATER				// >  greater than
	};

	class DataInfo{		// types of data
	public:
		DataInfo();

		std::vector<char*>&		getName();						// return names of all included primitive data types
		std::vector<char*>&		getFuncName();					// return names of all STL functions
		std::vector<DATATYPE>&	getType();						// return DATATYPE's of all included primitive data types
		std::vector<DATATYPE>&	getFuncType();					// return DATATYPE's of all STL functions

	private:
		std::vector<char*>		name_;							// names of all primitive types
		std::vector<char*>		funcName_;						// names of all STL functions
		std::vector<DATATYPE>	type_;							// data types of all primitive types
		std::vector<DATATYPE>	funcType_;						// data types of STL functions
	};

	class Element{		// element of the auxTable
	public:
		Element();
		
		int			getIndex();									// return lexTable index
		char*		getName();									// return element name
		char*		getFuncName();								// return function name
		int			getNumVal();								// return element num value
		char*		getLineVal();								// return element line value
		char*		getOtherVal();								// return element other value
		char		getOperation();								// return element operation value
		TYPE		getType();									// return element type
		DATATYPE	getDataType();								// return element data type

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

		Element*	getElem(int index);							// return element by index
		int			getSize();									// return size of auxiliary table
		DataInfo*	getDataInfo();								// return all names and types of functions and primitive types

		void		addElem(Element& elem);						// add element to auxiliary table

		bool		isIncluded(char* name, char* funcName);		// is name&funcName element included into auxiliary table?
		int			getIndex(char* name, char* funcName);		// return index of element from auxiliary table

		~Table();

	private:
		int			maxSize_;									// max size of auxiliary table
		int			size_;										// current size of auxiliary table
		DataInfo*	dataInfo_;									// consists all names and types of functions and primitive types
		Element*	table_;										// auxiliary table that consist elements
	};

	void  addPrefix(char* dest, char* prefix);					// add prefix to the line
	char* createStrVal(char* line);								// delete quotes in line literal
};