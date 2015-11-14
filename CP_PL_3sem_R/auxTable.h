#pragma once
#include "lexAnalyser.h"
#include "errors.h"
#include <vector>

#define AT_NAME_MAXSIZE		12				// max size of element name
#define AT_MAX_SIZE			4096			// max number of lines at auxiliary table
#define AT_LINE_MAXSIZE		255				// max size of line
#define AT_ARR_MAXSIZE		256				// max size of array
#define AT_NUM_DEFAULT		0xffffffff		// default value for num
#define AT_LINE_DEFAULT		0x00			// default value for line
#define AT_NULL_INDEX		0xffffffff		// default value for index
#define AT_PREFIX_LITERAL	"L"				// prefix of literal
#define AT_PREFIX_OPERATION	"O"				// prefix of operation
#define AT_PREFIX_COMPARE	"C"				// prefix of compare

#define AT_STL_FUNCTIONS_SIZE	13
#define AT_STL_FUNCTIONS		{ "begin", "print", "start", "get_type", "get_load", "get_temp", "get_time", "get_rpm", "set_type", "set_load", "set_temp", "set_time", "set_rpm" }
#define AT_STL_FUNCTIONS_TYPES	{ AT::DATATYPE::NIL,	AT::DATATYPE::NIL,	AT::DATATYPE::BOOL, AT::DATATYPE::LINE, AT::DATATYPE::NUM, AT::DATATYPE::NUM, AT::DATATYPE::NUM, AT::DATATYPE::NUM, AT::DATATYPE::NIL, AT::DATATYPE::NIL, AT::DATATYPE::NIL, AT::DATATYPE::NIL, AT::DATATYPE::NIL }

#define AT_PRIMITIVE_TYPES_SIZE		5
#define AT_PRIMITIVE_TYPES_NAMES	{ "num", "line", "wash", "bool", "nil" }
#define AT_PRIMITIVE_TYPES_VALUES	{ AT::DATATYPE::NUM, AT::DATATYPE::LINE, AT::DATATYPE::WASH, AT::DATATYPE::BOOL, AT::DATATYPE::NIL }

#define AT_ELEMENT_TYPES_SIZE	7
#define AT_ELEMENT_TYPES_NAMES	{ "variable", "parameter", "literal ", "function", "operation", "compare " }

#define AT_COMPARES_SIZE	6
#define AT_COMPARES_NAMES	{ "==", ">=","<=", "!=", ">", "<" }

#define AT_OPERATIONS_SIZE	4
#define AT_OPERATIONS_NAMES	{ "+", "-", "*", "/" }

namespace LA { class LexAnalyser; };

namespace AT{		// auxiliary table namespace
	enum SERVICE{		// service enum for universal values
		ERROR_VALUE = -1	// unknown or error value
	};

	enum DATATYPE{
		NUM = 0,			// integer
		LINE,				// string
		WASH,				// wash
		BOOL,				// bool
		NIL					// like a void in C++
	};

	enum TYPE{
		V = 0,				// variable
		P,					// parameter
		L,					// literal
		F,					// function
		O,					// operation
		C					// compare
	};

	class Info{		// types of data
	public:
		Info();
					/*return name of function*/
		char*		getFuncName(int);

					/*return name of type*/
		char*		getElemTypeName(int);

					/*return name of compare*/
		char*		getCompareName(int);

					/*return name of operation*/
		char		getOperationName(int);

					/*return name of primitive data type*/
		char*		getPrimTypeName(int);

					/*return DATATYPE value for function*/
		int			getFuncValue(char* name);

					/*return TYPE value for type name*/
		int			getElemTypeValue(char* name);

					/*return COMPARE value for compare name*/
		int			getCompareValue(char* name);

					/*return OPERATION value for operation name*/
		int			getOperationValue(char* name);

					/*return DATATYPE value for primitive data type name*/
		int			getPrimTypeValue(char* name);

					/*push this name & type to function list*/
		void		pushToFuncList(char* name, int type);

					/*name is a new function?*/
		bool		isNewFunction(char* name);

	private:
		struct Element{							// vector elements
			Element(char* name, int value);

			char*				name_;			// name of element
			int					value_;			// element number in some sequence
		};
		
		std::vector<Element>	functions_;		// contains STL functions names & types
		std::vector<Element>	elementsTypes_;	// contains elements types names & types
		std::vector<Element>	compares_;		// contains compares values
		std::vector<Element>	operations_;	// contains arithmetical operations
		std::vector<Element>	primTypes_;		// contains primitive data types names
												// & values
	};

	class Element{		// element of the auxTable
	public:
		Element();
		
		int		getType();						// return element type
		int		getDataType();					// return element data type
		char*	getName();						// return element name
		char*	getFuncName();					// return function name
		int		getIndex();						// return lexTable index
		int		getNumVal();					// return element num value
		char*	getLineVal();					// return element line value

		void	setIndex(int value);			// set lexTable index
		void	setName(char* name);			// set element name
		void	setFuncName(char* name);		// set element function name
		void	setValue(						// set element value
					char lexeme,
					char* line = NULL_STR);
		void	setNumVal(int value);			// set num element value
		void	setLineVal(char* value);		// set line element value
		void	setElem(						// set element
					LA::LexAnalyser*	la,				// lexTable
					char*				funcName,		// function name
					char**				arrOfLines,		// array of lines
					int&				i,				// chain index number
					int					counter = -1	// counter
					);
		
		void	reset();						/* set element fields to
												   default values */

	private:
		int		ltIndex_;						/* index of first occurrence of
												   this element in lexTable */
		char	name_[AT_NAME_MAXSIZE];			// element name
		char	funcName_[AT_NAME_MAXSIZE];		// element function name
		int		type_;							// element type
		int		dataType_;						// element data type
		struct {
			int		numValue_;							// field for numerical values
			char	lineValue_[AT_ARR_MAXSIZE];			// field for line values
		} value_;								// element value
	};

	class Table{		// auxiliary table
	public:
		Table();
		Table(int size);

		Info*					getInfo();									// return all names and types of functions, primitive types etc.
		Element*				getElem(int index);							// return element by index
		int						getSize();									// return size of auxiliary table
		int						getFuncIndex(char* name);					// return function index in auxTable
		int						getLiteralDataType(char* line);

		char					getOperation(int value);

		void					addElem(Element& elem);						// add element to auxiliary table

		bool					isIncluded(char* name, char* funcName, char lexeme);		// is name&funcName element included into auxiliary table?
		int						getIndex(char* name, char* funcName);		// return index of element from auxiliary table

		~Table();

	private:
		int						maxSize_;									// max size of auxiliary table
		int						size_;										// current size of auxiliary table
		Info*					info_;										// consists all names and types of functions, primitive types etc.
		Element*				table_;										// auxiliary table that consist elements
	};

	void						addPrefix(char* dest, char* prefix);		// add prefix to the line
	char*						createStrVal(char* line);					// delete quotes in line literal
};