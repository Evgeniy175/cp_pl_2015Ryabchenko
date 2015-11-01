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
		FST::FST* fst = new FST::FST[NUMBER_OF_GRAPHS];	// �������� �������
		bool isCorrect = false;							// ���������� �� ������
		int  lineNumber = 0;							// ������� �����
		int	 literalCounter = 0;						// ������� ���������
		char funcName[AUX_NAME_MAXSIZE + 1];			// ��� �������

		fst->createFst();								// ������������ ������� fst

		Log::writeLine(log, "---������ ������ ��---", "");

		for (int chainNumber = 0; chainNumber < in.getNumOfChains(); chainNumber++){

			isCorrect = false;

			for (int i = 0; !isCorrect && i < NUMBER_OF_GRAPHS; i++){

				fst[i].setString(in.getLine(chainNumber));

				if (isCorrect = fst[i].execute()){
					//Log::writeLine(log, "  ������� ", in.getLine(chainNumber), "\t\t����������", "");
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

			if (!isCorrect){	// ���� �� ����������
				Error::ERROR errorVar = ERROR_THROW_LINE(202, in.getLine(chainNumber), lineNumber, -1);
				Log::writeError(log, errorVar);
			};
		};
		Log::writeLine(log, "---����� ������ ��---", "");
	};


	LexAnalyser::~LexAnalyser(){
		delete this->lexTable;
		delete this->auxTable;
	};
};