#include "stdafx.h" 
#include "in.h"
#include "log.h"

namespace IN{
	In::In(){
		this->size_ = NULL;
		this->lines_ = NULL;
		this->ignor_ = NULL;
		this->numberOfChains_ = NULL;
	};

	char** In::getArr(){
		return this->arrOfLines_;
	};

	char* In::getLine(int i){
		return this->arrOfLines_[i];
	};

	int In::getNumOfChains(){
		return this->numberOfChains_;
	};

	int In::getLinesCounter(){
		return this->lines_;
	};

	int In::getSizeCounter(){
		return this->size_;
	};

	int In::getIgnorCounter(){
		return this->ignor_;
	};

	int In::getCode(char symbol){
		return this->code_[unsigned char(symbol)];
	};

	void In::setCode(int* code){
		this->code_ = code;
	};

	void In::setArr(char** arrOfLines){
		this->arrOfLines_ = arrOfLines;
	};

	void In::setLine(char* line){
		this->arrOfLines_[this->numberOfChains_] = line;
	};

	void In::setChar(int& currPos, char symbol){
		this->arrOfLines_[this->numberOfChains_][currPos++] = symbol;
	};

	void In::setLineEnd(int& currChainPosition){
		this->arrOfLines_[this->numberOfChains_++][currChainPosition] = NULL_STR;
		this->arrOfLines_[this->numberOfChains_] = new char[IN_MAX_LEN_TEXT];
		currChainPosition = 0;
	};

	void In::increaseIgnor(){
		this->ignor_++;
	};

	void In::increaseLines(){
		this->lines_++;
	};

	void In::increaseSize(){
		this->size_++;
	};

	void In::setText(char symbol){
		this->text_[this->size_] = symbol;
	};

	In::In(LOG::Log* log, wchar_t* infile){
		std::ifstream file(infile);
		ERROR::Error* errorVar;

		int clearedValue[] = IN_CODE_TABLE;
		int currChainPosition = 0;				// позиция в цепочке
		int positionCounter = 0;				// позиция в строке файла
		char tempChar;							// считываемый символ из файла
		char previousChar = 0;					// предыдущий символ, нужен для удаления пробелов

		this->setCode(clearedValue);

		if (file.good()){
			log->writeLine("---Начало проверки допустимости символов---", "");

			this->setArr(new char*[IN_MAX_LINE_NUMBER]);	// выделение памяти под кол-во строк In_MAX_LInE_NUMBER
			this->setLine(new char[IN_MAX_LEN_TEXT]);		// выделение памяти для нулевой строки, In_MAX_LEN_TEXT символов

			tempChar = file.get();

			while (!file.eof())	{
				switch (this->getCode(tempChar)){
				case IN::A:
					this->setChar(currChainPosition, tempChar);
					positionCounter++;
					break;

				case IN::D:
					this->setChar(currChainPosition, tempChar);
					errorVar = ERROR_THROW_FULL(103, NULL, this->getLinesCounter(), positionCounter);
					log->writeError(errorVar);
					positionCounter++;
					break;

				case IN::I:
					positionCounter++;
					this->increaseIgnor();
					break;

				case IN::S:
					if (currChainPosition != NULL){
						this->setLineEnd(currChainPosition);
						positionCounter = 0;
					};
					if (tempChar == IN_CODE_ENDL){
						this->setChar(currChainPosition, LEX_NEWLINE);
						this->increaseLines();
					}
					else{
						this->setChar(currChainPosition, tempChar);
					};
					this->setLineEnd(currChainPosition);
					positionCounter = 0;
					break;

				case IN::B:
					if (this->getCode(previousChar) == IN::A){
						this->setLineEnd(currChainPosition);
						positionCounter = 0;
					};
					break;

				case IN::Q:
					this->createLine(tempChar, file, positionCounter);
					positionCounter = 0;
					currChainPosition = 0;
					break;

				default:
					this->setChar(currChainPosition, tempChar);
					positionCounter++;
					break;
				};

				this->setText(tempChar);
				this->increaseSize();
				positionCounter++;
				previousChar = tempChar;
				tempChar = file.get();
			};

			log->writeLine("---Конец проверки допустимости символов---", "");
		}
		else
			throw ERROR_THROW(102);
		
		this->setText(NULL_STR);
		file.close();
	};

	void In::createLine(char firstSymbol, std::ifstream& file, int& positionCounter){
		char tempChar = firstSymbol;
		int i = 0;

		for (; static_cast<unsigned char>(tempChar) != 0x92; i++){
			this->arrOfLines_[this->numberOfChains_][i] = tempChar;
			this->text_[this->size_++] = tempChar;
			tempChar = file.get();
		};
		this->arrOfLines_[this->numberOfChains_][i++] = tempChar;
		this->text_[this->size_++] = tempChar;
		setLineEnd(i);
	};
};