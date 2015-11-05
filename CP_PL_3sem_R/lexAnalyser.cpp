#include "stdafx.h"
#include "lexAnalyser.h"

namespace LA{
	LexAnalyser::LexAnalyser(){}

	LT::Table* LexAnalyser::getLT(){
		return this->lexTable;
	};

	AT::Table* LexAnalyser::getAT(){
		return this->auxTable;
	};

	LT::Element* LexAnalyser::getElemLT(int i){
		return this->getLT()->getElem(i);
	};

	AT::Element* LexAnalyser::getElemAT(int i){
		return this->getAT()->getElem(i);
	};

	int LexAnalyser::_getFuncIdx(char* name){
		for (int i = 0; i < this->getATsize(); i++){
			if (!strcmp(this->getElemAT(i)->getName(), name)){
				return i;
			};
		};
		return AT_NULLIDX;
	};

	bool LexAnalyser::_isIncludedFunction(char* name){
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;
		for (firstIt = this->getFuncName().begin(),
			secondIt = this->getFuncType().begin();
			firstIt != this->getFuncName().end();
			firstIt++, secondIt++){
				if (!strcmp(*firstIt, name)) return true;
		};
		return false;
	};

	int LexAnalyser::getATidx(char* name, char* funcName){
		int rc = this->getAT()->getIdx(name, funcName);

		if (rc == AT_NULLIDX && _isIncludedFunction(name)){
			rc = _getFuncIdx(name);
		};

		return rc;
	};

	int LexAnalyser::getATsize(){
		return this->getAT()->getSize();
	};

	int LexAnalyser::getLTsize(){
		return this->getLT()->getSize();
	};

	std::vector<char*>&	LexAnalyser::getName(){
		return this->getAT()->getDataInfo()->getName();
	};

	std::vector<char*>&	LexAnalyser::getStructName(){
		return this->getAT()->getDataInfo()->getStructName();
	};

	std::vector<char*>&	LexAnalyser::getFuncName(){
		return this->getAT()->getDataInfo()->getFuncName();
	};

	std::vector<AT::DATATYPE>& LexAnalyser::getType(){
		return this->getAT()->getDataInfo()->getType();
	};

	std::vector<AT::DATATYPE>& LexAnalyser::getStructType(){
		return this->getAT()->getDataInfo()->getStructType();
	};

	std::vector<AT::DATATYPE>& LexAnalyser::getFuncType(){
		return this->getAT()->getDataInfo()->getFuncType();
	};

	void LexAnalyser::addElemLT(LT::Element& elem){
		this->getLT()->addElem(elem);
	};

	AT::TYPE LexAnalyser::getElemType(){
		return this->getLT()->getType();
	};

	void LexAnalyser::addElemAT(AT::Element& elem){
		this->getAT()->addElem(elem);
	};

	bool LexAnalyser::isIncludedInAT(char* name, char* funcName){
		return this->getAT()->isIncluded(name, funcName);
	};

	char* LexAnalyser::getDataName(AT::DATATYPE dataType){
		char* rc = "unknown";
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;

		for (firstIt = this->getName().begin(),
			secondIt = this->getType().begin();
			secondIt != this->getType().end();
		firstIt++, secondIt++){
			if (*secondIt == dataType) rc = *firstIt;
		};
		return rc;
	};

	AT::DATATYPE LexAnalyser::getDataType(char** arrOfLines, int i){
		AT::DATATYPE rc = AT::DATATYPE::UNKNOWN;
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;

		switch (this->getElemLT(this->getLTsize() - 2)->getLex()){
		case LEX_COLON:																			// для структурированного типа
			for (firstIt = this->getStructName().begin(),
				secondIt = this->getStructType().begin();
				firstIt != this->getStructName().end();
				firstIt++, secondIt++){
					if (!strcmp(*firstIt, arrOfLines[i])) rc = *secondIt;
			};
			break;

		case LEX_OPERATION: case LEX_EQUALLY: case LEX_COMMA: case LEX_SQBRACEOPEN: case LEX_RETURN:				// для литералов
			rc = arrOfLines[i][0] == '‘' ? AT::DATATYPE::LINE : AT::DATATYPE::NUM;
			break;

		case LEX_TYPE:
			for (firstIt = this->getName().begin(),
				secondIt = this->getType().begin();
				firstIt != this->getName().end();
				firstIt++, secondIt++){
					if (!strcmp(*firstIt, arrOfLines[i - 1])) rc = *secondIt;
			};
			break;

		default:
			for (firstIt = this->getFuncName().begin(),
				secondIt = this->getFuncType().begin();
				firstIt != this->getFuncName().end();
				firstIt++, secondIt++){
					if (!strcmp(*firstIt, arrOfLines[i])) rc = *secondIt;
			};
			break;
		};
		return rc;
	};

	void LexAnalyser::execute(int size, LOG::Log* log, IN::In* in){
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
					this->addElemLT(elemLt);

					switch (LT::getLex(i)){
					case LEX_BEGIN:
						strncpy_s(funcName, in->getLine(chainNumber), 
								  static_cast<int> (strlen(in->getLine(chainNumber))));
						break;

					case LEX_NEWLINE:	lineNumber++; break;

					case LEX_ID:
						if (this->getElemType() == AT::TYPE::F){
							strncpy_s(funcName, in->getLine(chainNumber), strlen(in->getLine(chainNumber)));
							this->getFuncName().push_back(in->getLine(chainNumber));
							this->getFuncType().push_back(this->getDataType(in->getArr(), chainNumber));
						}
						if (!this->isIncludedInAT(in->getLine(chainNumber), funcName)){
							elemAt.setElem(this, funcName, in->getArr(), chainNumber);
							this->addElemAT(elemAt);
							if (elemAt.getType() == AT::TYPE::U)
								throw ERROR_THROW_FULL(203, in->getLine(chainNumber), lineNumber, -1);
						};
						this->getElemLT(chainNumber)->setIdx(this->getATidx(in->getLine(chainNumber), funcName));
						break;

					case LEX_LITERAL:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, literalCounter++);
						this->addElemAT(elemAt);
						this->getElemLT(chainNumber)->setIdx(this->getATsize() - 1);
						break;

					case LEX_OPERATION:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, operationCounter);
						this->addElemAT(elemAt);
						this->getElemLT(chainNumber)->setIdx(this->getATsize() - 1);

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

	LexAnalyser::~LexAnalyser(){
		delete this->lexTable;
		delete this->auxTable;
	};
};