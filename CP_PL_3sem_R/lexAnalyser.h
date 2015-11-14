#pragma once

#include "log.h"
#include "in.h"
#include "fst.h"
#include "lexTable.h"
#include "auxTable.h"
#include "errors.h"
#include <vector>

namespace LOG{ class Log; };
namespace IN { class In; };
namespace LT{ class Table; class Element; };
namespace AT{ enum TYPE; enum DATATYPE; class Info; class Table; class Element; };

namespace LA{
	class LexAnalyser{
	public:
		LexAnalyser();
		
						/*return lexTable element(i)*/
		LT::Element*	getElemLt(int i);

						/*return auxTable element(i)*/
		AT::Element*	getElemAt(int i);

						/*return data type of arrOfLines[chainNumber]*/
		int				getDataType(char** arrOfLines, int chainNumber);

						/*return auxTable size*/
		int				getAtSize();

						/*return lexTable size*/
		int				getLtSize();

						/*return index from auxTable for name & funcName*/
		int				getAtIndex(char* name, char* funcName);

						/*return function index in auxTable*/
		int				getFuncIndex(char* name);
		
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

						/*add element into lexTable*/
		void			addElemLt(LT::Element& elem);
		
						/*add element into auxTable*/
		void			addElemAt(AT::Element& elem);

						/*is new function?*/
		bool			isNewFunc(char* name);
						
						/*is element included into auxiliary table?*/
		bool			isIncludedInAt(char* name,
							char* funcName,
							char lexeme = 'M');

						/*add function to functions vector;
						line - function name
						type - function data type*/
		void			pushToFuncList(char* line, int type);

						/*execute lexeme analyser*/
		void			execute(int size, LOG::Log* log, IN::In* in);

		~LexAnalyser();

	private:
		LT::Table*		lexTable_;
		AT::Table*		auxTable_;
	};
};