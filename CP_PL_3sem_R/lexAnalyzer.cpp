#include "stdafx.h"
#include "lexAnalyzer.h"

namespace LA{
	LexAnalyzer::LexAnalyzer(){}

	LT::Element* LexAnalyzer::getElemLt(int i){
		return this->lexTable_->getElem(i);
	};

	AT::Element* LexAnalyzer::getElemAt(int i){
		return this->auxTable_->getElem(i);
	};

	int LexAnalyzer::getFunctionIndex(char* name){
		return this->auxTable_->getFunctionIndex(name);
	};

	int LexAnalyzer::getAtIndex(char* value, AT::TYPE type){
		switch (type){
		case AT::TYPE::L: return this->auxTable_->getLiteralIndex(value);
		case AT::TYPE::O: return this->auxTable_->getOperationIndex(value);
		case AT::TYPE::C: return this->auxTable_->getCompareIndex(value);
		default: break;
		};
		return AT_NULL_INDEX;
	};

	int LexAnalyzer::getAtIndex(char* name, char* funcName){
		int rc = this->auxTable_->getIndex(name, funcName);
		if (rc == AT_NULL_INDEX && !isNewFunc(name)) rc = getFunctionIndex(name);
		return rc;
	};

	int LexAnalyzer::getAtSize(){
		return this->auxTable_->getSize();
	};

	int LexAnalyzer::getLtSize(){
		return this->lexTable_->getSize();
	};

	int LexAnalyzer::getDataType(char** arrOfLines, int i){
		int rc = AT::SERVICE::ERROR_VALUE;
		std::vector<char*>::iterator firstIt;
		std::vector<AT::DATATYPE>::iterator secondIt;

		if (this->getElemLt(this->getLtSize() - 1)->getLex() == LEX_OPERATION
			|| this->getElemLt(this->getLtSize() - 1)->getLex() == LEX_COMPARE){
				return rc;
		};

		switch (this->getElemLt(this->getLtSize() - 2)->getLex()){
		case LEX_COMPARE: case LEX_OPERATION: case LEX_EQUAL:
		case LEX_COMMA: case LEX_OPEN_SQBRACE: case LEX_RETURN:
			return this->auxTable_->getLiteralDataType(arrOfLines[i]);				// for literals
			break;

		case LEX_TYPE: return this->getTypeValue(arrOfLines[i - 1]);

		default:
			return this->getFuncTypeValue(arrOfLines[i]);
			break;
		};
		return rc;
	};

	char* LexAnalyzer::getPrimTypeName(int primitiveTypeValue){
		return this->auxTable_->getInfo()->getPrimTypeName(primitiveTypeValue);
	};

	char* LexAnalyzer::getElemTypeName(int elemTypeValue){
		return this->auxTable_->getInfo()->getElemTypeName(elemTypeValue);
	};

	char* LexAnalyzer::getFuncName(int type){
		return this->auxTable_->getInfo()->getFunctionName(type);
	};

	char* LexAnalyzer::getCompareName(int compareValue){
		return this->auxTable_->getInfo()->getCompareName(compareValue);
	};

	char LexAnalyzer::getOperationName(int operationValue){
		return this->auxTable_->getInfo()->getOperationName(operationValue);
	};

	int LexAnalyzer::getTypeValue(char* line){
		return this->auxTable_->getInfo()->getPrimTypeValue(line);
	};

	int LexAnalyzer::getFuncTypeValue(char* line){
		return this->auxTable_->getInfo()->getFunctionValue(line);
	};

	int LexAnalyzer::getElemTypeValue(char* line){
		int rc = AT::SERVICE::ERROR_VALUE;
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
				else if (this->auxTable_->getInfo()->getFunctionValue(line) != AT::SERVICE::ERROR_VALUE){
					return AT::TYPE::F;
				};
				break;
			};
			break;

		default: break;
		};
		return rc;
	};

	int LexAnalyzer::getCompareValue(char* name){
		return this->auxTable_->getInfo()->getCompareValue(name);
	};

	int LexAnalyzer::getOperationValue(char* name){
		return this->auxTable_->getInfo()->getOperationValue(name);
	};

	void LexAnalyzer::pushToFuncList(char* line, int type){
		this->auxTable_->getInfo()->pushToFunctionList(line, type);
	};

	void LexAnalyzer::addElemLt(LT::Element& elem){
		this->lexTable_->addElem(elem);
	};

	void LexAnalyzer::addElemAt(AT::Element& elem){
		this->auxTable_->addElem(elem);
	};

	bool LexAnalyzer::isIncluded(char* name, AT::TYPE type){
		switch (type){
		case AT::TYPE::L: return this->auxTable_->getLiteralIndex(name)
									!= AT_NULL_INDEX ? true : false;
		case AT::TYPE::O: return this->auxTable_->getOperationIndex(name)
									!= AT_NULL_INDEX ? true : false;
		case AT::TYPE::C: return this->auxTable_->getCompareIndex(name)
									!= AT_NULL_INDEX ? true : false;
		default: break;
		};
		return false;
	};

	bool LexAnalyzer::isIncluded(char* name, char* funcName){
		return this->auxTable_->getIndex(name, funcName) != AT_NULL_INDEX ? true : false;
	};

	bool LexAnalyzer::isNewFunc(char* name){
		return this->auxTable_->getInfo()->isNewFunction(name);
	};

	void LexAnalyzer::execute(int size, LOG::Log* log, IN::In* in){
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
						if (this->getElemTypeValue(in->getLine(chainNumber)) == AT::TYPE::F
							&& this->isNewFunc(in->getLine(chainNumber))){
								strncpy_s(funcName, in->getLine(chainNumber), AT_NAME_MAXSIZE - 1);
								this->pushToFuncList(in->getLine(chainNumber), this->getDataType(in->getArr(), chainNumber));
						}
						if (!this->isIncluded(in->getLine(chainNumber), funcName)){		// isincludedfunc?
							elemAt.setElem(this, funcName, in->getArr(), chainNumber);
							this->addElemAt(elemAt);
							if (elemAt.getType() == AT::SERVICE::ERROR_VALUE)
								throw ERROR_THROW_FULL(203, in->getLine(chainNumber), lineNumber, -1);
						};
						this->getElemLt(chainNumber)->setIndex(this->getAtIndex(in->getLine(chainNumber), funcName));
						break;

					case LEX_LITERAL:
						if (!this->isIncluded(in->getLine(chainNumber), AT::TYPE::L)){
							elemAt.setElem(this, funcName, in->getArr(), chainNumber, literalCounter++);
							this->addElemAt(elemAt);
						};
						this->getElemLt(chainNumber)->setIndex(this->getAtIndex(in->getLine(chainNumber), AT::TYPE::L));
						break;

					case LEX_OPERATION:
						if (!this->isIncluded(in->getLine(chainNumber), AT::TYPE::O)){
							elemAt.setElem(this, funcName, in->getArr(), chainNumber, operationCounter++);
							this->addElemAt(elemAt);
						};
						this->getElemLt(chainNumber)->setIndex(this->getAtIndex(in->getLine(chainNumber), AT::TYPE::O));
						break;

					case LEX_COMPARE:
						if (!this->isIncluded(in->getLine(chainNumber), AT::TYPE::C)){
							elemAt.setElem(this, funcName, in->getArr(), chainNumber, compareCounter++);
							this->addElemAt(elemAt);
						};
						this->getElemLt(chainNumber)->setIndex(this->getAtIndex(in->getLine(chainNumber), AT::TYPE::C));
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

	LexAnalyzer::~LexAnalyzer(){
		delete this->lexTable_;
		delete this->auxTable_;
	};
};