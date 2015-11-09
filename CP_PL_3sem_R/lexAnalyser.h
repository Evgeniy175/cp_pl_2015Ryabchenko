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
		
		AT::DATATYPE	getDataType(char** arrOfLines, int chainNumber);			// return DATATYPE for this line
		LT::Element*	getElemLt(int i);											// return lexTable element(i)
		AT::Element*	getElemAt(int i);											// return auxTable element(i)
		int				getAtIndex(char* name, char* funcName);						// return index of this element from auxTable
		int				getAtSize();												// return auxTable size
		int				getLtSize();												// return lexTable size
		int				getFuncIndex(char* name);									// return function index in auxTable
		bool			isNewFunc(char* name);										// is new function?

		char*			getDataName(AT::DATATYPE dataType);							// return char* name of this DATATYPE for this line
		char*			getTypeName(AT::TYPE type);
		char*			getFuncName(AT::DATATYPE);									// return char* name of function type from vector
		AT::DATATYPE	getType(char* line);										// return type from vector
		AT::DATATYPE	getFuncType(char* line);
		AT::TYPE		getElemType(char* line);									// return last element type

		void			pushToFuncList(char* line, AT::DATATYPE type);

		void			addElemLt(LT::Element& elem);								// add element into lexTable
		void			addElemAt(AT::Element& elem);								// add element into auxTable
		
		bool			isIncludedInAT(char* name, char* funcName);					// element is included into auxTable?
		void			execute(int size, LOG::Log* log, IN::In* in);

		~LexAnalyser();

	private:
		LT::Table*		getLt();													// return lexTable
		AT::Table*		getAt();													// return auxTable

		LT::Table* lexTable;
		AT::Table* auxTable;
	};
};