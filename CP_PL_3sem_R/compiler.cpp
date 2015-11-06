#include "stdafx.h"
#include "compiler.h"

namespace CP{
	Compiler::Compiler(PATH::Path* filesPath){
		this->path_ = new PATH::Path(*filesPath);
		this->log_ = new LOG::Log(this->path_->getLog());
		this->in_ = new IN::In();
		this->la_ = new LA::LexAnalyser();
	};

	void Compiler::executeIn(){
		this->in_->execute(this->log_, this->path_->getIn());
	};

	void Compiler::executeLa(){
		this->la_->execute(this->in_->getNumOfChains(), this->log_, this->in_);
	};

	wchar_t* Compiler::getInName(){
		return this->path_->getIn();
	};

	wchar_t* Compiler::getOutName(){
		return this->path_->getOut();
	};

	wchar_t* Compiler::getLogName(){
		return this->path_->getLog();
	};
	
	LT::Element* Compiler::getElemLT(int i){
		return this->getLA()->getLT()->getElem(i);
	};

	AT::Element* Compiler::getElemAT(int i){
		return this->getLA()->getAT()->getElem(i);
	};
	
	int Compiler::getLTsize(){
		return this->getLA()->getLT()->getSize();
	};

	int Compiler::getATsize(){
		return this->getLA()->getAT()->getSize();
	};

	LA::LexAnalyser* Compiler::getLA(){
		return this->la_;
	};

	IN::In*	Compiler::getIn(){
		return this->in_;
	};

	LOG::Log* Compiler::getLog(){
		return this->log_;
	};

	int Compiler::getPriority(char symbol){
		switch (symbol){
		case '*': case '/': return 3;
		case '+': case '-': return 2;
		case '(': return 1;
		default: break;
		};
		return 0;
	};

	void Compiler::writeLine(char* c, ...){
		char **p = &c;

		for (int i = 0; *(p + i)[0] != NULL_STR; i++)
			this->log_->line(*(p + i));

		this->log_->line("\n");
	};

	void Compiler::writeLine(wchar_t* line, ...){
		char temp[SIZE_CHAR_ARRAY];
		char *p;
		wchar_t **cyclePointer = &line;

		for (int i = 0; *(cyclePointer + i)[0] != NULL_STR; i++){
			wcstombs_s(0, temp, *(cyclePointer + i), sizeof(temp));
			p = temp;
			this->log_->line(p);
		};
		this->log_->writeLine("\n");
	};

	void Compiler::writeLog(){
		this->log_->writeLog();
	};

	void Compiler::writeCP(){
		this->log_->writeCP(this);
	};

	void Compiler::writeIn(){
		this->log_->writeIn(this->in_);
	};

	void Compiler::writeError(ERROR::Error* error){
		this->log_->writeError(error);
	};

	void Compiler::writeLt(){
		this->log_->writeLt(this->la_);
	};

	void Compiler::writeAt(){
		this->log_->writeAt(this->la_);
	};

	void Compiler::closeLog(){
		this->log_->close();
	};

	bool Compiler::buildPnFunc(std::stack<LT::Element*>& stack, std::list<LT::Element>& exitArr, int& position){
		int parmCounter = 0;
		LT::Element temp = (exitArr.back());
		exitArr.pop_back();
		stack.push(this->getElemLT(position++));

		while (this->getElemLT(position)->getLex() != LEX_SQBRACECLOSE){
			switch (this->getElemLT(position)->getLex()){
			case LEX_ID: case LEX_LITERAL:
				exitArr.push_back(*(this->getElemLT(position)));
				parmCounter++;
				break;

			case LEX_COMMA: break;

			case LEX_SQBRACEOPEN:
				if (!buildPnFunc(stack, exitArr, position))
					return false;
				break;
			default: return false;
			};
			position++;
		};
		stack.pop();
		temp.setParmCount(parmCounter);
		exitArr.push_back(temp);
		parmCounter = 0;

		return true;
	};

	bool Compiler::checkPn(int& position){
		int startPos = position;
		std::stack<LT::Element*> stack;
		std::list<LT::Element> exitArr;

		if (this->getElemLT(position)->getLex() == LEX_EQUALLY) startPos++;

		for (; this->getElemLT(position)->getLex() != LEX_SEMICOLON
			&& this->getElemLT(position)->getLex() != LEX_RIGHTHESIS;
			position++){
			switch (this->getElemLT(position)->getLex()){
			case LEX_ID: case LEX_LITERAL: case LEX_PRINT:
				exitArr.push_back(*(this->getElemLT(position)));
				break;

			case LEX_OPERATION:
				if (stack.size() == NULL){
					stack.push(this->getElemLT(position));
				}
				else{
					while (stack.size() != NULL
						&& getPriority(this->getElemAT(this->getElemLT(position)->getIndex())->getOperation())
						<= getPriority(this->getElemAT(stack.top()->getIndex())->getOperation())){
							exitArr.push_back(*(stack.top()));
							stack.pop();
					};
					if (stack.size() == NULL
						|| (getPriority(this->getElemAT(this->getElemLT(position)->getIndex())->getOperation())
							> getPriority(this->getElemAT(stack.top()->getIndex())->getOperation()))){
								stack.push(this->getElemLT(position));
					};
				};
				break;

			case LEX_SQBRACEOPEN:
				if (!buildPnFunc(stack, exitArr, position))
					return false;
				break;

			case LEX_EQUALLY: case LEX_COMMA: break;
			default: return false;
			};
		};
		while (stack.size() != NULL){			// выталкиваем из стека в выходную строку
			exitArr.push_back(*(stack.top()));
			stack.pop();
		};
		while (exitArr.size() != NULL){			// помещаем из строки в таблицы
			this->getElemLT(startPos++)->setElem(exitArr.front());
			exitArr.pop_front();
		};
		while (startPos != position){			// обнуляем лишние элементы
			this->getElemLT(startPos++)->setElem();
		};
		return true;
	};

	void Compiler::polishNotation(){
		this->writeLine("\n\n", "");
		for (int i = 0; i < this->getLTsize(); i++){
			if (this->getElemLT(i)->getLex() == LEX_ID
				|| this->getElemLT(i)->getLex() == LEX_LITERAL
				|| this->getElemLT(i)->getLex() == LEX_EQUALLY
				|| this->getElemLT(i)->getLex() == LEX_PRINT){
				if (checkPn(i)){
					this->writeLine("Польская запись построена", "");
				}
				else{
					this->writeLine("Не удалось построить польскую запись", "");
					while (this->getElemLT(i)->getLex() != LEX_SEMICOLON
						&& this->getElemLT(i)->getLex() != LEX_SQBRACECLOSE){
							i++;
					};
				};
			};
		};
	};

	void Compiler::modifyAT(){
		bool isModified;
		for (int auxIt = 0; auxIt < this->getATsize(); auxIt++){
			isModified = false;
			for (int lexIt = 0; !isModified && lexIt < this->getLTsize(); lexIt++){
				if (this->getElemLT(lexIt)->getIndex() == auxIt){
					this->getElemAT(auxIt)->setIndex(lexIt);
					isModified = true;
				};
			};
		};
	};

	Compiler::~Compiler(){
		delete this->path_;
		delete this->log_;
		delete this->in_;
		delete this->la_;
	}
};