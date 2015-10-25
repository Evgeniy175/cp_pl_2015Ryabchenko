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
					Log::writeLine(log, "  Цепочка ", in.getLine(chainNumber), "\t\tраспознана", "");

					switch (LT::getLexem(i))
					{
					case LEX_BEGIN:		IT::createFuncName(funcName, in.getLine(chainNumber));  break;

					case LEX_NEWLINE:	lineNumber++; break;

					case LEX_ID:
						if (IT::getIdType(rc.lexTable, in.getLine(chainNumber)) == IT::IDTYPE::F)
							IT::createFuncName(funcName, in.getLine(chainNumber));

						strcpy_s(idFull, funcName);
						strncat_s(idFull, in.getLine(chainNumber), TI_ID_FULL_MAXSIZE - strlen(idFull) - 1);

						if (!rc.itTable->isIncluded(in.getLine(chainNumber)) && !rc.itTable->isIncluded(idFull))
						{
							if (IT::isSimilar(in.getLine(chainNumber), funcName))
								IT::setEntry(entryIt, rc.lexTable, rc.lexTable->size_, in.getArr(), chainNumber);
							else
							{
								IT::setEntry(entryIt, rc.lexTable, rc.lexTable->size_, in.getArr(), chainNumber);
								IT::addPrefix(entryIt.id_, funcName);
							};

							IT::addElement(rc.itTable, entryIt);
						};

						if (entryIt.idType_ == IT::IDTYPE::U) throw ERROR_THROW_FULL(203, in.getLine(chainNumber), -1, -1)
						break;

					default: break;
					};

					LT::setEntry(entryLt, i, lineNumber, rc.itTable->size_);
					LT::addElement(rc.lexTable, entryLt);
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