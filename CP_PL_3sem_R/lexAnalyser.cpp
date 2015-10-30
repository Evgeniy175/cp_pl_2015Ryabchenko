#include "stdafx.h"
#include "lexAnalyser.h"

namespace LA
{
	lexAnalyser::lexAnalyser(LT::LexTable* lt, IT::IdTable* it)
	{
		this->lexTable = lt;
		this->itTable = it;
	}


	lexAnalyser create(Log::LOG log, In::IN in)
	{
		lexAnalyser rc(LT::create(in.getNumOfChains()), IT::create(in.getNumOfChains()));
		LT::Entry entryLt;
		IT::Entry entryIt;
		FST::FST* fst = new FST::FST[NUMBER_OF_GRAPHS];	// конечный автомат
		bool isCorrect = false;							// распознана ли строка
		int  lineNumber = 0;							// счётчик строк
		int	 literalCounter = 0;						// счетчик литералов
		int ti_index = 0;								// индекс идентификатора/литерала в таблице идентификаторов
		char funcName[TI_ID_MAXSIZE + 1];				// имя функции
		char idFull[TI_ID_FULL_MAXSIZE];				// переменная для хранения ID и префикса(имени функции)

		for (int i = 0; i < NUMBER_OF_GRAPHS; i++)		fst[i] = fst->createFst(i, "");		// формирование массива fst

		Log::writeLine(log, "---Начало работы КА---", "");

		for (int chainNumber = 0; chainNumber < in.getNumOfChains(); chainNumber++)
		{
			isCorrect = false;

			for (int i = 0; !isCorrect && i < NUMBER_OF_GRAPHS; i++)
			{
				fst[i].setString(in.getLine(chainNumber));

				if (isCorrect = fst->execute(fst[i]))
				{
					//Log::writeLine(log, "  Цепочка ", in.getLine(chainNumber), "\t\tраспознана", "");

					switch (LT::getLexem(i))
					{
					case LEX_BEGIN:
						IT::createFuncName(funcName, in.getLine(chainNumber));
						//IT::setEntry(entryIt, rc.lexTable, rc.lexTable->size_, in.getArr(), chainNumber);
						//IT::addElement(rc.itTable, entryIt);
						break;

					case LEX_NEWLINE:	lineNumber++; break;

					case LEX_ID:
						if (IT::getType(rc.lexTable) == IT::IDTYPE::F)
							IT::createFuncName(funcName, in.getLine(chainNumber));

						strcpy_s(idFull, funcName);
						strncat_s(idFull, in.getLine(chainNumber), TI_ID_FULL_MAXSIZE - strlen(idFull) - 1);

						if (!rc.itTable->isIncluded(in.getLine(chainNumber)) && !rc.itTable->isIncluded(idFull))
						{
							if (IT::isFunction(in.getLine(chainNumber), idFull))
								IT::setEntry(entryIt, rc.lexTable, in.getArr(), chainNumber);
							else
							{
								IT::setEntry(entryIt, rc.lexTable, in.getArr(), chainNumber);
								IT::addPrefix(entryIt.id_, funcName);
							};
							IT::addElement(rc.itTable, entryIt);
							if (entryIt.idType_ == IT::IDTYPE::U) throw ERROR_THROW_FULL(203, in.getLine(chainNumber), -1, -1);
						};						
						break;

					case  LEX_LITERAL:
						IT::setEntry(entryIt, rc.lexTable, in.getArr(), chainNumber, IT::IDTYPE::L, literalCounter++);
						IT::addPrefix(entryIt.id_, funcName);
						IT::addElement(rc.itTable, entryIt);
						break;

					default: break;
					};
					
					if ((ti_index = rc.itTable->getIndex(entryIt.id_)) == -1)
						ti_index = rc.itTable->getIndex(idFull);

					LT::setEntry(entryLt, i, lineNumber, ti_index);
					LT::addElement(rc.lexTable, entryLt);
					IT::reset(entryIt);
				};
			};

			if (!isCorrect)	// если не распознана
			{
				Error::ERROR errorVar = ERROR_THROW_FULL(202, in.getLine(chainNumber), -1, -1);
				Log::writeError(log, errorVar);
			};
		};

		Log::writeLine(log, "---Конец работы КА---", "");
		return rc;
	};


	void deleteLa(lexAnalyser la)
	{
		LT::del(la.lexTable);
		IT::del(la.itTable);
	};
};