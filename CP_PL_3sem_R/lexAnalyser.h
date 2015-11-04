#pragma once

#include "log.h"
#include "in.h"
#include "fst.h"
#include "lexTable.h"
#include "auxTable.h"
#include "errors.h"

namespace LOG{ class Log; };
namespace IN { class In; };
namespace LT{ class Table; };
namespace AT{ enum TYPE; enum DATATYPE; class Table; };

namespace LA{
	class LexAnalyser{
	public:
		LexAnalyser(int size, LOG::Log* log, IN::In* in);
		
		LT::Table*		getLT();
		AT::Table*		getAT();
		AT::DATATYPE	getDataType(char** arrOfLines, int chainNumber);
		char*			getDataName(AT::DATATYPE dataType);

		~LexAnalyser();

	private:
		LT::Table* lexTable;
		AT::Table* auxTable;
	};
};