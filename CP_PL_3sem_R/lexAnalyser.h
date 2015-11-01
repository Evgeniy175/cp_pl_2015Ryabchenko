#pragma once

#include "log.h"
#include "in.h"
#include "fst.h"
#include "lexTable.h"
#include "auxTable.h"
#include "errors.h"

namespace LA
{
	class LexAnalyser
	{
	public:
		LexAnalyser(int size);
		
		LT::LexTable* getLT();
		IT::AuxTable* getAT();

		void create(Log::LOG log, In::IN in);

		~LexAnalyser();

	private:
		LT::LexTable* lexTable;
		IT::AuxTable* auxTable;
	};

	
};