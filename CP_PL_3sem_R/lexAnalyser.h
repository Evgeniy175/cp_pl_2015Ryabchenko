#pragma once

#include "log.h"
#include "in.h"
#include "fst.h"
#include "lexTable.h"
#include "auxTable.h"
#include "errors.h"

namespace LA
{
	struct LexAnalyser
	{
		LT::LexTable* lexTable;
		IT::IdTable*  auxTable;

		LexAnalyser(LT::LexTable* lT, IT::IdTable* aT);
	};

	LexAnalyser create(Log::LOG log, In::IN in);
	void deleteLa(LexAnalyser la);
};