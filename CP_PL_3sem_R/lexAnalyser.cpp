#include "stdafx.h"
#include "lexAnalyser.h"

namespace LA
{
	LexAnalyser::LexAnalyser(int size, Log::LOG log, In::IN in){
		this->lexTable = new LEX::Table(size);
		this->auxTable = new AUX::Table(size);
		LEX::Element elemLt;								// lexTable element
		AUX::Element elemAt;								// auxTable element
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

					switch (LEX::getLex(i)){
					case LEX_BEGIN:
						strncpy_s(funcName, in.getLine(chainNumber), 
								  static_cast<int> (strlen(in.getLine(chainNumber))));
						break;

					case LEX_NEWLINE:	lineNumber++; break;

					case LEX_ID:
						if (this->getLT()->getType() == AUX::TYPE::F)
							strncpy_s(funcName, in.getLine(chainNumber), strlen(in.getLine(chainNumber)));

						if (!this->getAT()->isIncluded(in.getLine(chainNumber), funcName)){
							 elemAt.setElem(this, funcName, in.getArr(), chainNumber);
								 this->getAT()->addElem(elemAt);
								//if (elemAt.idType_ == AUX::TYPE::U)
									//throw ERROR_THROW_LINE(203, in.getLine(chainNumber), lineNumber, -1);
						};
						this->getLT()->getElem(chainNumber)->setIdx(this->getAT()->getIdx(in.getLine(chainNumber), funcName));
						break;

					case  LEX_LITERAL:
						elemAt.setElem(this, funcName, in.getArr(), chainNumber, AUX::TYPE::L, literalCounter++);
						this->getAT()->addElem(elemAt);
						this->getLT()->getElem(chainNumber)->setIdx(this->getAT()->getSize() - 1);
						break;

					case LEX_ACTIONS:
						elemAt.setElem(this, funcName, in.getArr(), chainNumber);
						this->getAT()->addElem(elemAt);
						this->getLT()->getElem(chainNumber)->setIdx(this->getAT()->getSize() - 1);

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

	LEX::Table* LexAnalyser::getLT(){
		return this->lexTable;
	};

	AUX::Table* LexAnalyser::getAT(){
		return this->auxTable;
	};

	AUX::DATATYPE LexAnalyser::getDataType(char** arrOfLines, int i){
		AUX::DATATYPE rc = AUX::DATATYPE::UNKNOWN;
		std::vector<char*>::iterator firstIt;
		std::vector<AUX::DATATYPE>::iterator secondIt;

		if (this->getLT()->getElem(this->getLT()->getSize() - 2)->getLex() == LEX_COLON){
			for (firstIt = this->getAT()->getDataStruct()->getStructName().begin(),
				secondIt = this->getAT()->getDataStruct()->getStructType().begin();
				firstIt != this->getAT()->getDataStruct()->getStructName().end(); firstIt++, secondIt++){
				if (strcmp(*firstIt, arrOfLines[i]) == NULL) rc = *secondIt;
			};
		}
		else if (this->getLT()->getElem(this->getLT()->getSize() - 2)->getLex() == LEX_EQUALLY
			|| this->getLT()->getElem(this->getLT()->getSize() - 2)->getLex() == LEX_COMMA
			|| this->getLT()->getElem(this->getLT()->getSize() - 2)->getLex() == LEX_SQBRACEOPEN
			|| this->getLT()->getElem(this->getLT()->getSize() - 2)->getLex() == LEX_RETURN) {
			rc = arrOfLines[i][0] == '�' ? AUX::DATATYPE::LINE : AUX::DATATYPE::NUM;
		}
		else {
			for (firstIt = this->getAT()->getDataStruct()->getName().begin(), 
				secondIt = this->getAT()->getDataStruct()->getType().begin();
				firstIt != this->getAT()->getDataStruct()->getName().end(); 
				firstIt++, secondIt++){
					if (strcmp(*firstIt, arrOfLines[i - 1]) == NULL) rc = *secondIt;
			};
		};

		return rc;
	};

	char* LexAnalyser::getDataName(AUX::DATATYPE type){
		char* rc = "unknown";
		std::vector<char*>::iterator firstIt = this->getAT()->getDataStruct()->getName().begin();
		std::vector<AUX::DATATYPE>::iterator secondIt = this->getAT()->getDataStruct()->getType().begin();

		for (; secondIt != this->getAT()->getDataStruct()->getType().end(); firstIt++, secondIt++)
			if (*secondIt == type) rc = *firstIt;

		return rc;
	};


	LexAnalyser::~LexAnalyser(){
		delete this->lexTable;
		delete this->auxTable;
	};
};