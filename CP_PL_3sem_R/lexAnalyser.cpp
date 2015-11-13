#include "stdafx.h"
#include "lexAnalyser.h"

namespace LA{
	LexAnalyser::LexAnalyser(){}

	LT::Table* LexAnalyser::getLt(){
		return this->lexTable_;
	};

	AT::Table* LexAnalyser::getAt(){
		return this->auxTable_;
	};

	LT::Element* LexAnalyser::getElemLt(int i){
		return this->getLt()->getElem(i);
	};

	AT::Element* LexAnalyser::getElemAt(int i){
		return this->getAt()->getElem(i);
	};

	int LexAnalyser::getFuncIndex(char* name){
		for (int i = 0; i < this->getAtSize(); i++){
			if (!strcmp(this->getElemAt(i)->getName(), name)){
				return i;
			};
		};
		return AT_NULL_INDEX;
	};

	bool LexAnalyser::isNewFunc(char* name){
		return this->getAt()->getInfo()->isNewFunction(name);
	};

	int LexAnalyser::getAtIndex(char* name, char* funcName){
		int rc = this->getAt()->getIndex(name, funcName);

		if (rc == AT_NULL_INDEX && !isNewFunc(name)) rc = getFuncIndex(name);

		return rc;
	};

	int LexAnalyser::getAtSize(){
		return this->getAt()->getSize();
	};

	int LexAnalyser::getLtSize(){
		return this->getLt()->getSize();
	};

	char* LexAnalyser::getFuncName(AT::DATATYPE type){
		return this->getAt()->getInfo()->getFuncName(type);
	};

	AT::DATATYPE LexAnalyser::getType(char* line){
		return this->getAt()->getInfo()->getType(line);
	};

	AT::DATATYPE LexAnalyser::getFuncType(char* line){
		return this->getAt()->getInfo()->getFuncType(line);
	};

	void LexAnalyser::pushToFuncList(char* line, AT::DATATYPE type){
		this->getAt()->getInfo()->pushFuncName(line);
		this->getAt()->getInfo()->pushFuncType(type);
	};

	void LexAnalyser::addElemLt(LT::Element& elem){
		this->getLt()->addElem(elem);
	};

	void LexAnalyser::addElemAt(AT::Element& elem){
		this->getAt()->addElem(elem);
	};

	bool LexAnalyser::isIncludedInAT(char* name, char* funcName){
		return this->getAt()->isIncluded(name, funcName);
	};

	char* LexAnalyser::getDataName(AT::DATATYPE dataType){
		return this->getAt()->getInfo()->getName(dataType);
	};

	char* LexAnalyser::getTypeName(AT::TYPE type){
		return this->getAt()->getInfo()->getTypeName(type);
	};

	AT::TYPE LexAnalyser::getElemType(char* line){
		AT::TYPE rc = AT::TYPE::U;
		std::vector<char*>::iterator firstIt;

		switch (this->getElemLt(this->getLtSize() - 1)->getLex())
		{
		case LEX_LITERAL:	return AT::TYPE::L;
		case LEX_OPERATION: return AT::TYPE::O;
		case LEX_COMPARE:	return AT::TYPE::C;

		case LEX_IDENTIFIER:
			switch (this->getElemLt(this->getLtSize() - 3)->getLex()){
			case LEX_FUNCTION:						return AT::TYPE::F;
			case LEX_OPEN_SQBRACE: case LEX_COMMA:	return AT::TYPE::P;

			default:
				if (this->getElemLt(this->getLtSize() - 2)->getLex() == LEX_TYPE){
					return AT::TYPE::V;
				}
				else if (this->getAt()->getInfo()->getFuncType(line) != AT::DATATYPE::UNKNOWN){
					return AT::TYPE::F;
				};
				break;
			};
			break;

		default: break;
		};
		return rc;
	};

	AT::DATATYPE LexAnalyser::getDataType(char** arrOfLines, int i){
		AT::DATATYPE rc = AT::DATATYPE::UNKNOWN;
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;

		if (this->getElemLt(this->getLtSize() - 1)->getLex() == LEX_OPERATION
			|| this->getElemLt(this->getLtSize() - 1)->getLex() == LEX_COMPARE){
				return AT::DATATYPE::UNKNOWN;
		};

		switch (this->getElemLt(this->getLtSize() - 2)->getLex()){
		case LEX_COMPARE: case LEX_OPERATION: case LEX_EQUAL:
		case LEX_COMMA: case LEX_OPEN_SQBRACE: case LEX_RETURN:
			return arrOfLines[i][0] == '‘' ? AT::DATATYPE::LINE : AT::DATATYPE::NUM;				// для литералов
			break;

		case LEX_TYPE: return this->getType(arrOfLines[i - 1]);

		default:
			return this->getFuncType(arrOfLines[i]);
			break;
		};
		return rc;
	};

	void LexAnalyser::execute(int size, LOG::Log* log, IN::In* in){
		this->lexTable_ = new LT::Table(size);
		this->auxTable_ = new AT::Table(size);
		FST::FST fst[] = FST_ARRAY;
		LT::Element elemLt;								// lexTable element
		AT::Element elemAt;								// auxTable element
		bool isCorrect = false;							// is this string executed by graphs?
		int  lineNumber = 0;							// number of lines in input file
		int	 literalCounter = 0;
		int  operationCounter = 0;
		int  compareCounter = 0;
		char funcName[AT_NAME_MAXSIZE];					// function name

		log->writeLine("---Начало работы КА---", "");

		for (int chainNumber = 0; chainNumber < in->getNumOfChains(); chainNumber++){

			isCorrect = false;

			for (int i = 0; !isCorrect && i < NUMBER_OF_GRAPHS; i++){
				fst[i].setString(in->getLine(chainNumber));

				if (isCorrect = fst[i].execute()){
					//log->writeLine("  Цепочка ", in->getLine(chainNumber), "\t\tраспознана", "");
					elemLt.setElem(i, lineNumber);
					this->addElemLt(elemLt);

					switch (LT::getLex(i)){
					case LEX_BEGIN:
						strncpy_s(funcName, in->getLine(chainNumber), 
								  static_cast<int> (strlen(in->getLine(chainNumber))));
						elemAt.setElem(this, funcName, in->getArr(), chainNumber);
						this->addElemAt(elemAt);
						break;

					case LEX_NEW_LINE:	lineNumber++; break;

					case LEX_IDENTIFIER:
						if (this->getElemType(in->getLine(chainNumber)) == AT::TYPE::F
							&& this->isNewFunc(in->getLine(chainNumber))){
								strncpy_s(funcName, in->getLine(chainNumber), AT_NAME_MAXSIZE - 1);
								this->pushToFuncList(in->getLine(chainNumber), this->getDataType(in->getArr(), chainNumber));
						}
						if (!this->isIncludedInAT(in->getLine(chainNumber), funcName)){		// isincludedfunc?
							elemAt.setElem(this, funcName, in->getArr(), chainNumber);
							this->addElemAt(elemAt);
							if (elemAt.getType() == AT::TYPE::U)
								throw ERROR_THROW_FULL(203, in->getLine(chainNumber), lineNumber, -1);
						};
						this->getElemLt(chainNumber)->setIndex(this->getAtIndex(in->getLine(chainNumber), funcName));
						break;

					case LEX_LITERAL:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, literalCounter++);
						this->addElemAt(elemAt);
						this->getElemLt(chainNumber)->setIndex(this->getAtSize() - 1);
						break;

					case LEX_OPERATION:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, operationCounter++);
						this->addElemAt(elemAt);
						this->getElemLt(chainNumber)->setIndex(this->getAtSize() - 1);
						break;

					case LEX_COMPARE:
						elemAt.setElem(this, funcName, in->getArr(), chainNumber, compareCounter++);
						this->addElemAt(elemAt);
						this->getElemLt(chainNumber)->setIndex(this->getAtSize() - 1);
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
	};

	LexAnalyser::~LexAnalyser(){
		delete this->lexTable_;
		delete this->auxTable_;
	};
};