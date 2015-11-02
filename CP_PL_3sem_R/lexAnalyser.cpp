#include "stdafx.h"
#include "lexAnalyser.h"

namespace LA
{
	LexAnalyser::LexAnalyser(int size, LOG::Log* log, In::IN in){
		this->lexTable = new LT::Table(size);
		this->auxTable = new AT::Table(size);
		LT::Element elemLt;								// lexTable element
		AT::Element elemAt;								// auxTable element
		FST::FST* fst = new FST::FST[NUMBER_OF_GRAPHS];	// �������� �������
		bool isCorrect = false;							// ���������� �� ������
		int  lineNumber = 0;							// ������� �����
		int	 literalCounter = 0;						// ������� ���������
		char funcName[AT_NAME_MAXSIZE + 1];			// ��� �������

		fst->createFst();								// ������������ ������� fst

		log->writeLine("---������ ������ ��---", "");

		for (int chainNumber = 0; chainNumber < in.getNumOfChains(); chainNumber++){

			isCorrect = false;

			for (int i = 0; !isCorrect && i < NUMBER_OF_GRAPHS; i++){
				fst[i].setString(in.getLine(chainNumber));

				if (isCorrect = fst[i].execute()){
					//log->writeLine(log, "  ������� ", in.getLine(chainNumber), "\t\t����������", "");
					elemLt.setElem(i, lineNumber);
					this->getLT()->addElem(elemLt);

					switch (LT::getLex(i)){
					case LEX_BEGIN:
						strncpy_s(funcName, in.getLine(chainNumber), 
								  static_cast<int> (strlen(in.getLine(chainNumber))));
						break;

					case LEX_NEWLINE:	lineNumber++; break;

					case LEX_ID:
						if (this->getLT()->getType() == AT::TYPE::F)
							strncpy_s(funcName, in.getLine(chainNumber), strlen(in.getLine(chainNumber)));

						if (!this->getAT()->isIncluded(in.getLine(chainNumber), funcName)){
							 elemAt.setElem(this, funcName, in.getArr(), chainNumber);
								 this->getAT()->addElem(elemAt);
								//if (elemAt.idType_ == AT::TYPE::U)
									//throw ERROR_THROW_LINE(203, in.getLine(chainNumber), lineNumber, -1);
						};
						this->getLT()->getElem(chainNumber)->setIdx(this->getAT()->getIdx(in.getLine(chainNumber), funcName));
						break;

					case  LEX_LITERAL:
						elemAt.setElem(this, funcName, in.getArr(), chainNumber, AT::TYPE::L, literalCounter++);
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
				log->writeError(errorVar);
			};
		};
		log->writeLine("---����� ������ ��---", "");
	};

	LT::Table* LexAnalyser::getLT(){
		return this->lexTable;
	};

	AT::Table* LexAnalyser::getAT(){
		return this->auxTable;
	};

	AT::DATATYPE LexAnalyser::getDataType(char** arrOfLines, int i){
		AT::DATATYPE rc = AT::DATATYPE::UNKNOWN;
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;

		switch (this->getLT()->getElem(this->getLT()->getSize() - 2)->getLex())
		{
		case LEX_COLON:
			for (firstIt = this->getAT()->getDataStruct()->getStructName().begin(),
				secondIt = this->getAT()->getDataStruct()->getStructType().begin();
				firstIt != this->getAT()->getDataStruct()->getStructName().end();
				firstIt++, secondIt++){
					if (strcmp(*firstIt, arrOfLines[i]) == NULL) rc = *secondIt;
			};
			break;

		case LEX_EQUALLY: case LEX_COMMA: case LEX_SQBRACEOPEN: case LEX_RETURN:
			rc = arrOfLines[i][0] == '�' ? AT::DATATYPE::LINE : AT::DATATYPE::NUM;
			break;

		default:
			for (firstIt = this->getAT()->getDataStruct()->getName().begin(),
				secondIt = this->getAT()->getDataStruct()->getType().begin();
				firstIt != this->getAT()->getDataStruct()->getName().end();
				firstIt++, secondIt++){
					if (strcmp(*firstIt, arrOfLines[i - 1]) == NULL) rc = *secondIt;
			};
			break;
		};

		return rc;
	};

	char* LexAnalyser::getDataName(AT::DATATYPE type){
		char* rc = "unknown";
		std::vector<char*>::iterator firstIt = this->getAT()->getDataStruct()->getName().begin();
		std::vector<AT::DATATYPE>::iterator secondIt = this->getAT()->getDataStruct()->getType().begin();

		for (; secondIt != this->getAT()->getDataStruct()->getType().end(); firstIt++, secondIt++)
			if (*secondIt == type) rc = *firstIt;

		return rc;
	};


	LexAnalyser::~LexAnalyser(){
		delete this->lexTable;
		delete this->auxTable;
	};
};