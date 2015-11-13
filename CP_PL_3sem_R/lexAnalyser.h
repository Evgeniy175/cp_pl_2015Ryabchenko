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
		
		LT::Element*	getElemLt(int i);											// return lexTable element(i)
		AT::Element*	getElemAt(int i);											// return auxTable element(i)
		int				getDataType(char** arrOfLines, int chainNumber);			// return data type of arrOfLines[chainNumber]
		int				getAtSize();												// return auxTable size
		int				getLtSize();												// return lexTable size
		int				getAtIndex(char* name, char* funcName);						// return index from auxTable for name & funcName
		int				getFuncIndex(char* name);									// return function index in auxTable
		
		char*			getPrimTypeName(int primitiveTypeValue);					// return name of primitive data type
		char*			getElemTypeName(int elemTypeValue);							// return name of element type
		char*			getFuncName(int funcValue);									// return name of function
		int				getTypeValue(char* name);									// return type value of type name
		int				getFuncTypeValue(char* name);								// return function type value of function name
		int				getElemTypeValue(char* name);								// return element type value of element name

		void			addElemLt(LT::Element& elem);								// add element into lexTable
		void			addElemAt(AT::Element& elem);								// add element into auxTable

		bool			isNewFunc(char* name);										// is new function?
		bool			isIncludedInAt(char* name, char* funcName);					// element is included into auxTable?
		void			pushToFuncList(char* line, int type);						// add function to functions vector
		void			execute(int size, LOG::Log* log, IN::In* in);

		~LexAnalyser();

	private:
		LT::Table*		lexTable_;
		AT::Table*		auxTable_;
	};
};