#pragma once

#include "log.h"
#include "in.h"
#include "fst.h"
#include "lexTable.h"
#include "auxTable.h"
#include "errors.h"
#include <vector>

#define LA_NULL_LEX 'M'

namespace LOG	{	class Log;	};
namespace IN	{	class In;	};
namespace LT	{	class Table; class Element; };
namespace AT	{
	enum TYPE; enum DATATYPE; class Info; class Table; class Element;
};

namespace LA{
	class LexAnalyzer{
	public:
		LexAnalyzer();
		
						/*return lexeme table element(i)*/
		LT::Element*	getElemLt(int i);

						/*return auxiliary table element(i)*/
		AT::Element*	getElemAt(int i);

						/*return data type of arrOfLines[chainNumber]*/
		int				getDataType(char** arrOfLines, int chainNumber);

						/*return auxiliary table size*/
		int				getAtSize();

						/*return lexeme table size*/
		int				getLtSize();

						/*return index from auxiliary table
						for element name in function funcName
						belonging to the function is important*/
		int				getAtIndex(char* name, char* funcName);

						/*return index from auxiliary table;
						belonging to the function is not important*/
		int				getAtIndex(char* value, AT::TYPE type);

						/*return function index from auxiliary table*/
		int				getFunctionIndex(char* name);
		
						/*return name of primitive data type value*/
		char*			getPrimTypeName(int primitiveTypeValue);

						/*return name of element type value*/
		char*			getElemTypeName(int elemTypeValue);

						/*return name of function value*/
		char*			getFuncName(int funcValue);

						/*return name of compare value*/
		char*			getCompareName(int compareValue);

						/*return name of operation value*/
		char			getOperationName(int operationValue);

						/*return value of type name*/
		int				getTypeValue(char* name);

						/*return value of function name*/
		int				getFuncTypeValue(char* name);

						/*return value of element type name*/
		int				getElemTypeValue(char* name);

						/*return value of compare name*/
		int				getCompareValue(char* name);
		
						/*return value of operation name*/
		int				getOperationValue(char* name);

						/*add element to lexeme table*/
		void			addElemLt(LT::Element& elem);
		
						/*add element to auxiliary table*/
		void			addElemAt(AT::Element& elem);

						/*is new function?*/
		bool			isNewFunc(char* name);

						/*is element included into auxiliary table?
						belonging to the function ignored*/
		bool			isIncluded(char* name, AT::TYPE type);
						
						/*is element included into auxiliary table?
						belonging to the function is important*/
		bool			isIncluded(char* name, char* funcName);
		
						/*add function to functions vector;
						line - function name
						type - function data type*/
		void			pushToFuncList(char* line, int type);

						/*execute lexeme analyzer*/
		void			execute(int size, LOG::Log* log, IN::In* in);

		~LexAnalyzer();

	private:
		LT::Table*		lexTable_;
		AT::Table*		auxTable_;
	};
};