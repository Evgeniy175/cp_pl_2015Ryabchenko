#include "stdafx.h"
#include "lexAnalyser.h"

namespace LA{
	LexAnalyser::LexAnalyser(int size, LOG::Log* log, IN::In* in){
		this->lexTable = new LT::Table(size);
		this->auxTable = new AT::Table(size);
		LT::Element elemLt;								// lexTable element
		AT::Element elemAt;								// auxTable element
		FST::FST* fst = new FST::FST[NUMBER_OF_GRAPHS];	// конечный автомат
		bool isCorrect = false;							// распознана ли строка
		int  lineNumber = 0;							// счётчик строк
		int	 literalCounter = 0;						// счётчик литералов
		int  operationCounter = 0;						// счётчик операций
		char funcName[AT_NAME_MAXSIZE + 1];				// имя функции

		fst->createFst();								// формирование массива fst

		log->writeLine("---Начало работы КА---", "");

		for (int chainNumber = 0; chainNumber < in->getNumOfChains(); chainNumber++){

			isCorrect = false;

			for (int i = 0; !isCorrect && i < NUMBER_OF_GRAPHS; i++){
				fst[i].setString(in->getLine(chainNumber));

				if (isCorrect = fst[i].execute()){
					//log->writeLine(log, "  Цепочка ", in->getLine(chainNumber), "\t\tраспознана", "");
					elemLt.setElem(i, lineNumber);
					this->getLT()->addElem(elemLt);

					switch (LT::getLex(i)){
					case LEX_BEGIN:
						strncpy_s(funcName, in->getLine(chainNumber), 
								  static_cast<int> (strlen(in->getLine(chainNumber))));
						break;

					case LEX_NEWLINE:	lineNumber++; break;

					case LEX_ID:
						if (this->getLT()->getType() == AT::TYPE::F){
							strncpy_s(funcName, in->getLine(chainNumber), strlen(in->getLine(chainNumber)));
							this->getAT()->getDataStruct()->getFuncName().push_back(in->getLine(chainNumber));
							this->getAT()->getDataStruct()->getFuncType().push_back(this->getDataType(in->getArr(), chainNumber));
						}
						if (!this->getAT()->isIncluded(in->getLine(chainNumber), funcName)){
							elemAt.setElem(this, funcName, in->getArr(), chainNumber);
							this->getAT()->addElem(elemAt);
							if (elemAt.getType() == AT::TYPE::U)
								throw ERROR_THROW_FULL(203, in->getLine(chainNumber), lineNumber, -1);
						};
						this->getLT()->getElem(chainNumber)->setIdx(this->getAT()->getIdx(in->getLine(chainNumber), funcName));
						break;

					case LEX_LITERAL:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, literalCounter++);
						this->getAT()->addElem(elemAt);
						this->getLT()->getElem(chainNumber)->setIdx(this->getAT()->getSize() - 1);
						break;

					case LEX_OPERATION:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, operationCounter);
						this->getAT()->addElem(elemAt);
						this->getLT()->getElem(chainNumber)->setIdx(this->getAT()->getSize() - 1);

					default: break;
					};
					elemAt.reset();
				};
			};
			if (!isCorrect){	// если не распознана
				ERROR::Error* err = ERROR_THROW_FULL(202, in->getLine(chainNumber), lineNumber, -1);
				log->writeError(err);
			};
		};
		log->writeLine("---Конец работы КА---", "");
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
		case LEX_COLON:																			// для структурированного типа
			for (firstIt = this->getAT()->getDataStruct()->getStructName().begin(),
				secondIt = this->getAT()->getDataStruct()->getStructType().begin();
				firstIt != this->getAT()->getDataStruct()->getStructName().end();
				firstIt++, secondIt++){
					if (strcmp(*firstIt, arrOfLines[i]) == NULL) rc = *secondIt;
			};
			break;

		case LEX_EQUALLY: case LEX_COMMA: case LEX_SQBRACEOPEN: case LEX_RETURN:				// для литералов
			rc = arrOfLines[i][0] == '‘' ? AT::DATATYPE::LINE : AT::DATATYPE::NUM;
			break;

		case LEX_TYPE:
			for (firstIt = this->getAT()->getDataStruct()->getName().begin(),
				secondIt = this->getAT()->getDataStruct()->getType().begin();
				firstIt != this->getAT()->getDataStruct()->getName().end();
				firstIt++, secondIt++){
					if (strcmp(*firstIt, arrOfLines[i - 1]) == NULL) rc = *secondIt;
			};
			break;

		default:
			for (firstIt = this->getAT()->getDataStruct()->getFuncName().begin(),
				secondIt = this->getAT()->getDataStruct()->getFuncType().begin();
				firstIt != this->getAT()->getDataStruct()->getFuncName().end();
				firstIt++, secondIt++){
					if (strcmp(*firstIt, arrOfLines[i]) == NULL) rc = *secondIt;
			};
			break;
		};
		return rc;
	};

	char* LexAnalyser::getDataName(AT::DATATYPE dataType){
		char* rc = "unknown";
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;

		for (firstIt = this->getAT()->getDataStruct()->getName().begin(),
			secondIt = this->getAT()->getDataStruct()->getType().begin();
			secondIt != this->getAT()->getDataStruct()->getType().end();
			firstIt++, secondIt++){
				if (*secondIt == dataType) rc = *firstIt;
		};
		return rc;
	};

	LexAnalyser::~LexAnalyser(){
		delete this->lexTable;
		delete this->auxTable;
	};
};