#pragma once

#include "log.h"
#include "in.h"
#include "fst.h"
#include "lexTable.h"
#include "auxTable.h"
#include "errors.h"

namespace LOG{ class Log; };
namespace In { class IN; };
namespace LEX{ class Table; };
namespace AUX{ enum TYPE; enum DATATYPE; class Table; };

namespace LA{

	class LexAnalyser{
	public:
		LexAnalyser(int size, LOG::Log* log, In::IN in);
		
		LEX::Table* getLT();
		AUX::Table* getAT();

		char* getDataName(AUX::DATATYPE type);
		AUX::DATATYPE getDataType(char** arrOfLines, int chainNumber);

		~LexAnalyser();

	private:
		LEX::Table* lexTable;
		AUX::Table* auxTable;
	};
};