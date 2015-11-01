#include "stdafx.h"
#include "lexAnalyser.h"

namespace LA
{
	LexAnalyser::LexAnalyser(int size){
		this->lexTable = new LT::LexTable(size);
		this->auxTable = new IT::AuxTable(size);
	}

	LT::LexTable* LexAnalyser::getLT(){
		return this->lexTable;
	};

	IT::AuxTable* LexAnalyser::getAT(){
		return this->auxTable;
	};

	void LexAnalyser::create(Log::LOG log, In::IN in){
		LT::Element elemLt;								// lexTable element
		IT::Element elemAt;								// auxTable element
		FST::FST* fst = new FST::FST[NUMBER_OF_GRAPHS];	// конечный автомат
		bool isCorrect = false;							// распознана ли строка
		int  lineNumber = 0;							// счётчик строк
		int	 literalCounter = 0;						// счетчик литералов
		char funcName[AUX_NAME_MAXSIZE + 1];			// имя функции

		fst->createFst();								// формирование массива fst

		Log::writeLine(log, "---Начало работы КА---", "");

		for (int chainNumber = 0; chainNumber < in.getNumOfChains(); chainNumber++){

			isCorrect = false;

			for (int i = 0; !isCorrect && i < NUMBER_OF_GRAPHS; i++){

				fst[i].setString(in.getLine(chainNumber));

				if (isCorrect = fst[i].execute()){
					//Log::writeLine(log, "  Цепочка ", in.getLine(chainNumber), "\t\tраспознана", "");
					elemLt.setElem(i, lineNumber);
					this->getLT()->addElem(elemLt);

					switch (LT::getLex(i)){
					case LEX_BEGIN:
						strncpy_s(funcName, in.getLine(chainNumber), 
								  static_cast<int> (strlen(in.getLine(chainNumber))));
						break;

					case LEX_NEWLINE:	lineNumber++; break;

					case LEX_ID:
						if (IT::getType(this->getLT()) == IT::TYPE::F)
							strncpy_s(funcName, in.getLine(chainNumber), strlen(in.getLine(chainNumber)));

						if (!this->getAT()->isIncluded(in.getLine(chainNumber), funcName)){
							 elemAt.setElem(this->getLT(), funcName, in.getArr(), chainNumber);
								 this->getAT()->addElem(elemAt);
								//if (elemAt.idType_ == IT::TYPE::U)
									//throw ERROR_THROW_LINE(203, in.getLine(chainNumber), lineNumber, -1);
						};
						this->getLT()->getElem(chainNumber)->setIdx(this->getAT()->getIdx(in.getLine(chainNumber), funcName));
						break;

					case  LEX_LITERAL:
						elemAt.setElem(this->getLT(), funcName, in.getArr(), chainNumber, IT::TYPE::L, literalCounter++);
						this->getAT()->addElem(elemAt);
						this->getLT()->getElem(chainNumber)->setIdx(this->getAT()->getSize() - 1);
						break;

					default: break;
					};
					
					elemAt.reset();
				};
			};

			if (!isCorrect){	// если не распознана
				Error::ERROR errorVar = ERROR_THROW_LINE(202, in.getLine(chainNumber), lineNumber, -1);
				Log::writeError(log, errorVar);
			};
		};
		Log::writeLine(log, "---Конец работы КА---", "");
	};


	LexAnalyser::~LexAnalyser(){
		delete this->lexTable;
		delete this->auxTable;
	};
};