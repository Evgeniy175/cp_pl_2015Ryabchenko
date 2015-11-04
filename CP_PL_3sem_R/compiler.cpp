#include "stdafx.h"
#include "compiler.h"

namespace CP{
	Compiler::Compiler(int argc, _TCHAR* argv[]){
		this->log_ = new LOG::Log();

		wchar_t* temp;
		wchar_t temp2[CP_MAX_SIZE];
		int i = 1;

		if (argc < 2)
			throw ERROR_THROW(100);

		while (i < argc){
			if (wcslen(argv[i++]) > CP_MAX_SIZE)
				throw ERROR_THROW(101);
		};
		for (i = 1; i < argc; i++){
			temp = wcsstr(argv[i], L":");
			wcscpy_s(temp2, temp + 1);

			if (i == 1) wcscpy_s(this->inStr_, temp2);
			else{
				if (strstr((char*)(argv[i][1]), "out"))
					wcscpy_s(this->outStr_, temp2);

				else if (strstr((char*)(argv[i][1]), "log"))
					wcscpy_s(this->logStr_, temp2);
			};
		};

		if (argc < 4){
			if (argc == 3){
				if (strstr((char*)(argv[2][1]), "log")){
					wcscpy_s(this->outStr_, this->inStr_);
					wcsncat_s(this->outStr_, CP_OUT, 4);
				}
				else if (strstr((char*)(argv[2][1]), "out")){
					wcscpy_s(this->logStr_, this->inStr_);
					wcsncat_s(this->logStr_, CP_LOG, 4);
				};
			}
			else if (argc == 2){
				wcscpy_s(this->outStr_, this->inStr_);
				wcsncat_s(this->outStr_, CP_OUT, 4);
				wcscpy_s(this->logStr_, this->inStr_);
				wcsncat_s(this->logStr_, CP_LOG, 4);
			};
		};

		this->log_->getLog(this->logStr_);
		this->in_ = new IN::In(this->log_, this->inStr_);
		this->la_ = new LA::LexAnalyser(this->in_->getNumOfChains(), this->log_, this->in_);
	};

	wchar_t* Compiler::getInName(){
		return this->inStr_;
	};

	wchar_t* Compiler::getOutName(){
		return this->outStr_;
	};

	wchar_t* Compiler::getLogName(){
		return this->logStr_;
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

	int Compiler::getPnPriority(char symbol){
		switch (symbol)
		{
		case '*': case '/': return 3;
		case '+': case '-': return 2;
		case '(': return 1;
		default: break;
		};
		return 0;
	};

	bool Compiler::checkPn(int& position){
		int startPos = position;
		int parmCounter = 0;
		bool parmCounterStart = false;
		LT::Element temp;
		std::stack<LT::Element*> stack;
		std::list<LT::Element> exitArr;

		if (this->getLA()->getLT()->getElem(position)->getLex() == LEX_EQUALLY) startPos++;

		for (; this->getLA()->getLT()->getElem(position)->getLex() != LEX_SEMICOLON; position++){
			switch (this->getLA()->getLT()->getElem(position)->getLex()){
			case LEX_ID: case LEX_LITERAL: case LEX_PRINT: case LEX_COLON:
				exitArr.push_back(*(this->getLA()->getLT()->getElem(position)));
				if (parmCounterStart) parmCounter++;
				break;

			case LEX_OPERATION:
				if (stack.size() == NULL){
					stack.push(this->getLA()->getLT()->getElem(position));
				}
				else{
					while (getPnPriority(this->getLA()->getAT()->getElem(this->getLA()->getLT()->getElem(position)->getIdx())->getOperation())
						<= getPnPriority(this->getLA()->getAT()->getElem(stack.top()->getIdx())->getOperation())){
						exitArr.push_back(*(stack.top()));
						stack.pop();
					};
					if (getPnPriority(this->getLA()->getAT()->getElem(this->getLA()->getLT()->getElem(position)->getIdx())->getOperation())
						> getPnPriority(this->getLA()->getAT()->getElem(stack.top()->getIdx())->getOperation())){
						stack.push(this->getLA()->getLT()->getElem(position));
					};
				};
				break;

			case LEX_SQBRACEOPEN:
				temp = (exitArr.back());
				exitArr.pop_back();
				stack.push(this->getLA()->getLT()->getElem(position));
				parmCounterStart = true;
				break;

			case LEX_SQBRACECLOSE:
				if (stack.size() != NULL){
					while (stack.top()->getLex() != LEX_SQBRACEOPEN){
						exitArr.push_back(*(stack.top()));
						stack.pop();
					};
					stack.pop();
					temp.setParmCount(parmCounter);
					exitArr.push_back(temp);
					parmCounter = 0;
					parmCounterStart = false;
				};
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
			this->getLA()->getLT()->getElem(startPos)->setElem(exitArr.front());
			exitArr.pop_front();
			this->getLA()->getAT()->getElem(this->getLA()->getLT()->getElem(startPos)->getIdx())->setIdx(startPos++);
		};
		while (startPos != position){			// обнуляем лишние элементы
			this->getLA()->getLT()->getElem(startPos++)->setElem();
		};
		return true;
	};

	void Compiler::polishNotation(){
		this->getLog()->writeLine("\n\n", "");
		for (int i = 0; i < this->getLA()->getLT()->getSize(); i++){
			if (this->getLA()->getAT()->getElem(this->getLA()->getLT()->getElem(i)->getIdx())->getType() == AT::TYPE::F
				|| this->getLA()->getAT()->getElem(this->getLA()->getLT()->getElem(i)->getIdx())->getType() == AT::TYPE::E
				|| this->getLA()->getAT()->getElem(this->getLA()->getLT()->getElem(i)->getIdx())->getDataType() == AT::DATATYPE::WASH
				|| this->getLA()->getLT()->getElem(i)->getLex() == LEX_EQUALLY
				|| this->getLA()->getLT()->getElem(i)->getLex() == LEX_PRINT){
				if (checkPn(i)){
					this->getLog()->writeLine("Польская запись построена", "");
				}
				else{
					this->getLog()->writeLine("Не удалось построить польскую запись", "");
					while (this->getLA()->getLT()->getElem(i)->getLex() != LEX_SEMICOLON
						&& this->getLA()->getLT()->getElem(i)->getLex() != LEX_SQBRACECLOSE){
						i++;
					};
				};
			};
		};
	};
};