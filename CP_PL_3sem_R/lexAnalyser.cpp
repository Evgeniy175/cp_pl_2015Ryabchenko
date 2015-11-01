#include "stdafx.h"
#include "lexAnalyser.h"

namespace LA
{
	LexAnalyser::LexAnalyser(LT::LexTable* lT, IT::IdTable* aT){
		this->lexTable = lT;
		this->auxTable = aT;
	}


	LexAnalyser create(Log::LOG log, In::IN in){
		LexAnalyser rc(LT::create(in.getNumOfChains()), IT::create(in.getNumOfChains()));
		LT::Element elemLt;								// lexTable element
		IT::Element elemAt;								// auxTable element
		FST::FST* fst = new FST::FST[NUMBER_OF_GRAPHS];	// конечный автомат
		bool isCorrect = false;							// распознана ли строка
		int  lineNumber = 0;							// счётчик строк
		int	 literalCounter = 0;						// счетчик литералов
		char funcName[TI_ID_MAXSIZE + 1];				// имя функции

		fst->createFst();								// формирование массива fst

		Log::writeLine(log, "---Начало работы КА---", "");

		for (int chainNumber = 0; chainNumber < in.getNumOfChains(); chainNumber++){

			isCorrect = false;

			for (int i = 0; !isCorrect && i < NUMBER_OF_GRAPHS; i++){

				fst[i].setString(in.getLine(chainNumber));

				if (isCorrect = fst[i].execute()){
					//Log::writeLine(log, "  Цепочка ", in.getLine(chainNumber), "\t\tраспознана", "");
					elemLt.setElem(i, lineNumber, TI_NULLIDX);
					rc.lexTable->addElem(elemLt);

					switch (LT::getLexem(i)){
					case LEX_BEGIN:
						strncpy_s(funcName, in.getLine(chainNumber), 
								  static_cast<int> (strlen(in.getLine(chainNumber))));
						break;

					case LEX_NEWLINE:	lineNumber++; break;

					case LEX_ID:
						if (IT::getType(rc.lexTable) == IT::TYPE::F)
							strncpy_s(funcName, in.getLine(chainNumber), strlen(in.getLine(chainNumber)));

						if (!rc.auxTable->isIncluded(in.getLine(chainNumber), funcName)){
								IT::setEntry(elemAt, rc.lexTable, funcName, in.getArr(), chainNumber);
								IT::addElement(rc.auxTable, elemAt);
								//if (elemAt.idType_ == IT::TYPE::U)
									//throw ERROR_THROW_LINE(203, in.getLine(chainNumber), lineNumber, -1);
						};
						rc.lexTable->getElem(chainNumber)->setIdx(rc.auxTable->getIndex(in.getLine(chainNumber), funcName));
						break;

					case  LEX_LITERAL:
						IT::setEntry(elemAt, rc.lexTable, funcName, in.getArr(), chainNumber, IT::TYPE::L, literalCounter++);
						IT::addElement(rc.auxTable, elemAt);
						rc.lexTable->getElem(chainNumber)->setIdx(rc.auxTable->size_ - 1);
						break;

					default: break;
					};
					
					IT::reset(elemAt);
				};
			};

			if (!isCorrect){	// если не распознана
				Error::ERROR errorVar = ERROR_THROW_LINE(202, in.getLine(chainNumber), lineNumber, -1);
				Log::writeError(log, errorVar);
			};
		};
		Log::writeLine(log, "---Конец работы КА---", "");
		return rc;
	};


	void deleteLa(LexAnalyser la){
		LT::del(la.lexTable);
		IT::del(la.auxTable);
	};
};