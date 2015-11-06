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
namespace AT{ enum TYPE; enum DATATYPE; class Table; class Element; };

namespace LA{
	class LexAnalyser{
	public:
		LexAnalyser();
		
		LT::Table*		getLT();													// return lexTable
		AT::Table*		getAT();													// return auxTable
		AT::DATATYPE	getDataType(char** arrOfLines, int chainNumber);			// return DATATYPE for this line
		char*			getDataName(AT::DATATYPE dataType);							// return char* name of this DATATYPE for this line			
		AT::TYPE		getElemType(char* line);									// return last element type
		LT::Element*	getElemLT(int i);											// return lexTable element(i)
		AT::Element*	getElemAT(int i);											// return auxTable element(i)
		int				getATidx(char* name, char* funcName);						// return index of this element from auxTable
		int				getATsize();												// return auxTable size
		int				getLTsize();												// return lexTable size
		int				_getFuncIdx(char* name);									// return function index in auxTable
		bool			isNewFunc(char* name);										// is new function?

		std::vector<char*>&			getNameList();										// return char* name type from vector
		std::vector<char*>&			getFuncNameList();									// return char* name of function type from vector
		std::vector<AT::DATATYPE>&	getTypeList();										// return type from vector
		std::vector<AT::DATATYPE>&	getFuncTypeList();									// return function type from vector

		void addElemLT(LT::Element& elem);											// add element into lexTable
		void addElemAT(AT::Element& elem);											// add element into auxTable
		
		bool isIncludedInAT(char* name, char* funcName);							// element is included into auxTable?
		void execute(int size, LOG::Log* log, IN::In* in);

		~LexAnalyser();

	private:
		LT::Table* lexTable;
		AT::Table* auxTable;
	};
};