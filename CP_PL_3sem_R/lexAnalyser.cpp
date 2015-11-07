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
		return AT_NULL_INDEX;
	};

	bool LexAnalyser::isNewFunc(char* name){
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;
		for (firstIt = this->getFuncNameList().begin(),
			secondIt = this->getFuncTypeList().begin();
			firstIt != this->getFuncNameList().end();
			firstIt++, secondIt++){
				if (!strcmp(*firstIt, name)) return false;
		};
		return true;
	};

	int LexAnalyser::getATidx(char* name, char* funcName){
		int rc = this->getAT()->getIndex(name, funcName);

		if (rc == AT_NULL_INDEX && !isNewFunc(name)){
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

	std::vector<char*>&	LexAnalyser::getNameList(){
		return this->getAT()->getDataInfo()->getName();
	};

	std::vector<char*>&	LexAnalyser::getFuncNameList(){
		return this->getAT()->getDataInfo()->getFuncName();
	};

	std::vector<AT::DATATYPE>& LexAnalyser::getTypeList(){
		return this->getAT()->getDataInfo()->getType();
	};

	std::vector<AT::DATATYPE>& LexAnalyser::getFuncTypeList(){
		return this->getAT()->getDataInfo()->getFuncType();
	};

	void LexAnalyser::addElemLT(LT::Element& elem){
		this->getLT()->addElem(elem);
	};

	AT::TYPE LexAnalyser::getElemType(char* line){
		AT::TYPE rc = AT::TYPE::U;
		std::vector<char*>::iterator firstIt;

		if (this->getElemLT(this->getLTsize() - 3)->getLex() == LEX_FUNCTION){
			return AT::TYPE::F;
		}
		else if (this->getElemLT(this->getLTsize() - 1)->getLex() == LEX_OPERATION){
			return AT::TYPE::O;
		}
		else{
			switch (this->getElemLT(this->getLTsize() - 2)->getLex())
			{
			case LEX_COMMA: case LEX_OPEN_SQBRACE:					return AT::TYPE::P;
			case LEX_TYPE:											return AT::TYPE::V;
			case LEX_RETURN: case LEX_EQUALLY: case LEX_OPERATION:	return AT::TYPE::L;
			default:
				for (firstIt = this->getFuncNameList().begin();
					firstIt != this->getFuncNameList().end();
					firstIt++){
						if (!strcmp(*firstIt, line)) return AT::TYPE::F;
				};
				break;
			};
		};
		return rc;
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

		for (firstIt = this->getNameList().begin(),
			secondIt = this->getTypeList().begin();
			secondIt != this->getTypeList().end();
			firstIt++, secondIt++){
				if (*secondIt == dataType) return *firstIt;
		};
		return rc;
	};

	AT::DATATYPE LexAnalyser::getDataType(char** arrOfLines, int i){
		AT::DATATYPE rc = AT::DATATYPE::UNKNOWN;
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;

		switch (this->getElemLT(this->getLTsize() - 2)->getLex()){
		case LEX_OPERATION: case LEX_EQUALLY: case LEX_COMMA: case LEX_OPEN_SQBRACE: case LEX_RETURN:				// для литералов
			return arrOfLines[i][0] == '‘' ? AT::DATATYPE::LINE : AT::DATATYPE::NUM;
			break;

		case LEX_TYPE:
			for (firstIt = this->getNameList().begin(),
				secondIt = this->getTypeList().begin();
				firstIt != this->getNameList().end();
				firstIt++, secondIt++){
					if (!strcmp(*firstIt, arrOfLines[i - 1])) return *secondIt;
			};
			break;

		default:
			for (firstIt = this->getFuncNameList().begin(),
				secondIt = this->getFuncTypeList().begin();
				firstIt != this->getFuncNameList().end();
				firstIt++, secondIt++){
					if (!strcmp(*firstIt, arrOfLines[i])) return *secondIt;
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
					//log->writeLine("  Цепочка ", in->getLine(chainNumber), "\t\tраспознана", "");
					elemLt.setElem(i, lineNumber);
					this->addElemLT(elemLt);

					switch (LT::getLex(i)){
					case LEX_BEGIN:
						strncpy_s(funcName, in->getLine(chainNumber), 
								  static_cast<int> (strlen(in->getLine(chainNumber))));
						break;

					case LEX_NEW_LINE:	lineNumber++; break;

					case LEX_ID:
						if (this->getElemType(in->getLine(chainNumber)) == AT::TYPE::F
							&& this->isNewFunc(in->getLine(chainNumber))){
								strncpy_s(funcName, in->getLine(chainNumber), strlen(in->getLine(chainNumber)));
								this->getFuncNameList().push_back(in->getLine(chainNumber));
								this->getFuncTypeList().push_back(this->getDataType(in->getArr(), chainNumber));
						}
						if (!this->isIncludedInAT(in->getLine(chainNumber), funcName)){		// isincludedfunc
							elemAt.setElem(this, funcName, in->getArr(), chainNumber);
							this->addElemAT(elemAt);
							if (elemAt.getType() == AT::TYPE::U)
								throw ERROR_THROW_FULL(203, in->getLine(chainNumber), lineNumber, -1);
						};
						this->getElemLT(chainNumber)->setIndex(this->getATidx(in->getLine(chainNumber), funcName));
						break;

					case LEX_LITERAL:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, literalCounter++);
						this->addElemAT(elemAt);
						this->getElemLT(chainNumber)->setIndex(this->getATsize() - 1);
						break;

					case LEX_OPERATION:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, operationCounter);
						this->addElemAT(elemAt);
						this->getElemLT(chainNumber)->setIndex(this->getATsize() - 1);

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

		delete[] fst;
	};

	LexAnalyser::~LexAnalyser(){
		delete this->lexTable;
		delete this->auxTable;
	};
};