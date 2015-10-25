#pragma once

#include "log.h"
#include "in.h"
#include "fst.h"
#include "lexTable.h"
#include "idTable.h"
#include "errors.h"

namespace LA
{
	struct lexAnalyser
	{
		LT::LexTable* lexTable;
		IT::IdTable* itTable;

		lexAnalyser(LT::LexTable* lt, IT::IdTable* it);
	};

	lexAnalyser create(Log::LOG log, In::IN in);
	void deleteLa(lexAnalyser la);
};