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
		return this->getAT()->getInfo()->isNewFunction(name);
	};

	int LexAnalyser::getATidx(char* name, char* funcName){
		int rc = this->getAT()->getIndex(name, funcName);

		if (rc == AT_NULL_INDEX && !isNewFunc(name)) rc = _getFuncIdx(name);

		return rc;
	};

	int LexAnalyser::getATsize(){
		return this->getAT()->getSize();
	};

	int LexAnalyser::getLTsize(){
		return this->getLT()->getSize();
	};

	char* LexAnalyser::getFuncName(AT::DATATYPE type){
		return this->getAT()->getInfo()->getFuncName(type);
	};

	AT::DATATYPE LexAnalyser::getType(char* line){
		return this->getAT()->getInfo()->getType(line);
	};

	AT::DATATYPE LexAnalyser::getFuncType(char* line){
		return this->getAT()->getInfo()->getFuncType(line);
	};

	void LexAnalyser::pushToFuncList(char* line, AT::DATATYPE type){
		this->getAT()->getInfo()->pushFuncName(line);
		this->getAT()->getInfo()->pushFuncType(type);
	};

	void LexAnalyser::addElemLT(LT::Element& elem){
		this->getLT()->addElem(elem);
	};

	AT::TYPE LexAnalyser::getElemType(char* line){
		AT::TYPE rc = AT::TYPE::U;
		std::vector<char*>::iterator firstIt;

		switch (this->getElemLT(this->getLTsize() - 1)->getLex())
		{
		case LEX_LITERAL:	return AT::TYPE::L;
		case LEX_OPERATION: return AT::TYPE::O;
		case LEX_COMPARE:	return AT::TYPE::C;

		case LEX_IDENTIFIER:
			switch (this->getElemLT(this->getLTsize() - 3)->getLex()){
			case LEX_FUNCTION:						return AT::TYPE::F;
			case LEX_OPEN_SQBRACE: case LEX_COMMA:	return AT::TYPE::P;

			default:
				if (this->getElemLT(this->getLTsize() - 2)->getLex() == LEX_TYPE){
					return AT::TYPE::V;
				}
				else if (this->getAT()->getInfo()->getFuncType(line) != AT::DATATYPE::UNKNOWN){
					return AT::TYPE::F;
				};
				break;
			};
			break;

		default: break;
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
		return this->getAT()->getInfo()->getName(dataType);
	};

	char* LexAnalyser::getTypeName(AT::TYPE type){
		return this->getAT()->getInfo()->getTypeName(type);
	};

	AT::DATATYPE LexAnalyser::getDataType(char** arrOfLines, int i){
		AT::DATATYPE rc = AT::DATATYPE::UNKNOWN;
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;

		if (this->getElemLT(this->getLTsize() - 1)->getLex() == LEX_OPERATION
			|| this->getElemLT(this->getLTsize() - 1)->getLex() == LEX_COMPARE){
			return AT::DATATYPE::UNKNOWN;
		};

		switch (this->getElemLT(this->getLTsize() - 2)->getLex()){
		case LEX_COMPARE: case LEX_OPERATION: case LEX_EQUAL: case LEX_COMMA: case LEX_OPEN_SQBRACE: case LEX_RETURN:				// для литералов
			return arrOfLines[i][0] == '‘' ? AT::DATATYPE::LINE : AT::DATATYPE::NUM;
			break;

		case LEX_TYPE: return this->getType(arrOfLines[i - 1]);

		default:
			return this->getFuncType(arrOfLines[i]);
			break;
		};
		return rc;
	};

	void LexAnalyser::execute(int size, LOG::Log* log, IN::In* in){
		this->lexTable = new LT::Table(size);
		this->auxTable = new AT::Table(size);
		LT::Element elemLt;								// lexTable element
		AT::Element elemAt;								// auxTable element
		FST::FST* fst = new FST::FST[NUMBER_OF_GRAPHS];	// final state machine
		bool isCorrect = false;							// is this string executed by graphs?
		int  lineNumber = 0;							// number of lines in input file
		int	 literalCounter = 0;
		int  operationCounter = 0;
		int  compareCounter = 0;
		char funcName[AT_NAME_MAXSIZE + 1];				// function name

		fst->createFst();

		log->writeLine("---Начало работы КА---", "");

		for (int chainNumber = 0; chainNumber < in->getNumOfChains(); chainNumber++){

			isCorrect = false;

			for (int i = 0; !isCorrect && i < NUMBER_OF_GRAPHS; i++){
				fst[i].setString(in->getLine(chainNumber));

				if (isCorrect = fst[i].execute()){
					log->writeLine("  Цепочка ", in->getLine(chainNumber), "\t\tраспознана", "");
					elemLt.setElem(i, lineNumber);
					this->addElemLT(elemLt);

					switch (LT::getLex(i)){
					case LEX_BEGIN:
						strncpy_s(funcName, in->getLine(chainNumber), 
								  static_cast<int> (strlen(in->getLine(chainNumber))));
						break;

					case LEX_NEW_LINE:	lineNumber++; break;

					case LEX_IDENTIFIER:
						if (this->getElemType(in->getLine(chainNumber)) == AT::TYPE::F
							&& this->isNewFunc(in->getLine(chainNumber))){
								strncpy_s(funcName, in->getLine(chainNumber), strlen(in->getLine(chainNumber)));
								this->pushToFuncList(in->getLine(chainNumber), this->getDataType(in->getArr(), chainNumber));
						}
						if (!this->isIncludedInAT(in->getLine(chainNumber), funcName)){		// isincludedfunc?
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
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, operationCounter++);
						this->addElemAT(elemAt);
						this->getElemLT(chainNumber)->setIndex(this->getATsize() - 1);
						break;

					case LEX_COMPARE:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, compareCounter++);
						this->addElemAT(elemAt);
						this->getElemLT(chainNumber)->setIndex(this->getATsize() - 1);
						break;

					default: break;
					};
					elemAt.reset();
				};
			};
			if (!isCorrect){
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