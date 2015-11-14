#include "stdafx.h"
#include "compiler.h"

namespace CP{
	void Compiler::execute(PATH::Path& filesPath){
		this->reset();
		wcscpy_s(this->inPath_, filesPath.getIn());
		wcscpy_s(this->outPath_, filesPath.getOut());
		wcscpy_s(this->logPath_, filesPath.getLog());
		this->log_ = new LOG::Log(this->logPath_);
		this->in_ = new IN::In();
		this->la_ = new LA::LexAnalyser();
		this->writeCp();
	};

	void Compiler::execute(wchar_t* in){
		this->reset();
		wcscpy_s(this->inPath_, in);
		wcscpy_s(this->logPath_, this->inPath_);
		wcscat_s(this->logPath_, PATH_LOG_POSTFIX);
		wcscpy_s(this->outPath_, this->inPath_);
		wcscat_s(this->outPath_, PATH_OUT_POSTFIX);
		this->writeCp();

	};

	void Compiler::execute(char* in){
		this->reset();
		wchar_t temp[PATH_MAX_NAMESIZE];
		mbstowcs(temp, in, static_cast<int> (wcslen(temp)) - 1);
		wcscpy_s(this->inPath_, temp);
		wcscpy_s(this->logPath_, this->inPath_);
		wcscat_s(this->logPath_, PATH_LOG_POSTFIX);
		wcscpy_s(this->outPath_, this->inPath_);
		wcscat_s(this->outPath_, PATH_OUT_POSTFIX);
		this->writeCp();
	};

	void Compiler::executeIn(){
		this->in_->execute(this->log_, this->inPath_);
		this->writeIn();
	};

	void Compiler::executeLa(){
		this->la_->execute(this->in_->getNumOfChains(), this->log_, this->in_);
		this->writeLt();
		this->writeAt();
	};

	wchar_t* Compiler::getInName(){
		return this->inPath_;
	};

	wchar_t* Compiler::getOutName(){
		return this->outPath_;
	};

	wchar_t* Compiler::getLogName(){
		return this->logPath_;
	};
	
	LT::Element* Compiler::getElemLt(int i){
		return this->getLa()->getElemLt(i);
	};

	AT::Element* Compiler::getElemAt(int i){
		return this->getLa()->getElemAt(i);
	};
	
	int Compiler::getLtSize(){
		return this->getLa()->getLtSize();
	};

	int Compiler::getAtSize(){
		return this->getLa()->getAtSize();
	};

	LA::LexAnalyser* Compiler::getLa(){
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

		this->log_->newLine();
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
		this->log_->newLine();
	};

	void Compiler::writeCp(){
		this->log_->writeCp(this);
	};

	void Compiler::writeIn(){
		this->log_->writeIn(this->in_);
	};

	void Compiler::writeError(ERROR::Error* error){
		if (this->log_ != nullptr){
			this->log_->writeError(error);
		}
		else{
			this->log_ = new LOG::Log(L"errors.txt");
			this->log_->writeError(error);
		};
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

	bool Compiler::executeFuncPn(std::stack<LT::Element*>& stack,
		std::list<LT::Element>& exitArr, int& position){
		int parmCounter = 0;
		LT::Element temp = (exitArr.back());
		exitArr.pop_back();
		stack.push(this->getElemLt(position++));

		while (this->getElemLt(position)->getLex() != LEX_CLOSE_SQBRACE){
			switch (this->getElemLt(position)->getLex()){
			case LEX_IDENTIFIER: case LEX_LITERAL:
				exitArr.push_back(*(this->getElemLt(position)));
				parmCounter++;
				break;

			case LEX_COMMA: break;

			case LEX_OPEN_SQBRACE:
				if (!executeFuncPn(stack, exitArr, position))
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

	bool Compiler::executePn(int& position){
		int startPos = position;
		std::stack<LT::Element*> stack;
		std::list<LT::Element> exitArr;

		for (; this->getElemLt(position)->getLex() != LEX_SEMICOLON
			&& this->getElemLt(position)->getLex() != LEX_CLOSE_PARENTHESIS
			&& this->getElemLt(position)->getLex() != LEX_COMPARE;
			position++){
			switch (this->getElemLt(position)->getLex()){
			case LEX_IDENTIFIER: case LEX_LITERAL:
				exitArr.push_back(*(this->getElemLt(position)));
				break;

			case LEX_OPERATION:
				if (stack.size() == NULL){
					stack.push(this->getElemLt(position));
				}
				else{
					while (stack.size() != NULL
						&& (getPriority(this->la_->getOperationName(this->getElemAt(this->getElemLt(position)->getIndex())->getNumVal()))
						<= getPriority(this->la_->getOperationName(this->getElemAt(stack.top()->getIndex())->getNumVal())))){
							exitArr.push_back(*(stack.top()));
							stack.pop();
					};
					if (stack.size() == NULL
						|| (getPriority(this->la_->getOperationName(this->getElemAt(this->getElemLt(position)->getIndex())->getNumVal()))
						> getPriority(this->la_->getOperationName(this->getElemAt(stack.top()->getIndex())->getNumVal())))){
							stack.push(this->getElemLt(position));
					};
				};
				break;

			case LEX_OPEN_SQBRACE:
				if (this->getElemAt(this->getElemLt(position - 1)->getIndex())->getType() != AT::TYPE::F
					&& this->getElemAt(this->getElemLt(position - 1)->getIndex())->getDataType() != AT::DATATYPE::WASH){
						stack.push(this->getElemLt(position));
				}
				else{
					if (!executeFuncPn(stack, exitArr, position)){
						return false;
					};
				};
				break;

			case LEX_CLOSE_SQBRACE:
				if (stack.size() != NULL){
					while (stack.top()->getLex() != LEX_OPEN_SQBRACE){
						exitArr.push_back(*(stack.top()));
						stack.pop();
					};
					stack.pop();
				}
				else return false;
				break;

			case LEX_COMMA: break;
			default: return false;
			};
		};
		while (stack.size() != NULL){			// выталкиваем из стека в выходную строку
			exitArr.push_back(*(stack.top()));
			stack.pop();
		};
		while (exitArr.size() != NULL){			// помещаем из строки в таблицы
			this->getElemLt(startPos++)->setElem(exitArr.front());
			exitArr.pop_front();
		};
		while (startPos < position){			// обнуляем лишние элементы
			this->getElemLt(startPos++)->setElem();
		};
		return true;
	};

	void Compiler::polishNotation(){
		this->writeLine("");
		this->writeLine("");

		for (int i = 0; i < this->getLtSize(); i++){
			if (this->getElemLt(i)->getLex() == LEX_IDENTIFIER
				|| this->getElemLt(i)->getLex() == LEX_LITERAL){
					if (executePn(i)){
						this->writeLine("Польская запись построена", "");
					}
					else{
						this->writeLine("Не удалось построить польскую запись", "");
					};
			};
		};

		this->modifyAt();
		this->writeLt();
		this->writeAt();
	};

	void Compiler::modifyAt(){
		bool isModified;
		for (int auxIt = 0; auxIt < this->getAtSize(); auxIt++){
			isModified = false;
			for (int lexIt = 0; !isModified && lexIt < this->getLtSize(); lexIt++){
				if (this->getElemLt(lexIt)->getIndex() == auxIt){
					this->getElemAt(auxIt)->setIndex(lexIt);
					isModified = true;
				};
			};
		};
	};

	void Compiler::reset(){
		if (this->log_ != NULL){
			delete this->log_;
			this->log_ = nullptr;
		};
		if (this->in_ != NULL){
			delete this->in_;
			this->in_ = nullptr;
		};
		if (this->la_ != NULL){
			delete this->la_;
			this->la_ = nullptr;
		};
	};

	Compiler::~Compiler(){
		this->closeLog();
		delete this->log_;
		delete this->in_;
		delete this->la_;
	}
};