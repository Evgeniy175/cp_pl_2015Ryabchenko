#pragma once

#include "log.h"
#include "in.h"
#include "fst.h"
#include "lexTable.h"
#include "auxTable.h"
#include "errors.h"

namespace LOG{ class Log; };
namespace In { class IN; };
namespace LT{ class Table; };
namespace AT{ enum TYPE; enum DATATYPE; class Table; };

namespace LA{

	class LexAnalyser{
	public:
		LexAnalyser(int size, LOG::Log* log, In::IN in);
		
		LT::Table* getLT();
		AT::Table* getAT();

		char* getDataName(AT::DATATYPE type);
		AT::DATATYPE getDataType(char** arrOfLines, int chainNumber);

		~LexAnalyser();

	private:
		LT::Table* lexTable;
		AT::Table* auxTable;
	};
};