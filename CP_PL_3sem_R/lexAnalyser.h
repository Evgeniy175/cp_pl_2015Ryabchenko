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
		AT::TYPE		getElemType();												// return last in table element type
		LT::Element*	getElemLT(int i);											// return lexTable element(i)
		AT::Element*	getElemAT(int i);											// return auxTable element(i)
		int				getATidx(char* name, char* funcName);						// return index of this element from auxTable
		int				getATsize();												// return auxTable size
		int				getLTsize();												// return lexTable size

		std::vector<char*>&			getName();										// return char* name type from vector
		std::vector<char*>&			getStructName();								// return char* name struct element type from vector
		std::vector<char*>&			getFuncName();									// return char* name of function type from vector
		std::vector<AT::DATATYPE>&	getType();										// return type from vector
		std::vector<AT::DATATYPE>&	getStructType();								// return struct element type from vector
		std::vector<AT::DATATYPE>&	getFuncType();									// return function type from vector

		void addElemLT(LT::Element& elem);										// add element into lexTable
		void addElemAT(AT::Element& elem);										// add element into auxTable
		
		bool isIncludedInAT(char* name, char* funcName);
		void execute(int size, LOG::Log* log, IN::In* in);

		~LexAnalyser();

	private:
		LT::Table* lexTable;
		AT::Table* auxTable;
	};
};