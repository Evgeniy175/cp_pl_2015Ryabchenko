#include "stdafx.h" 
#include "in.h"
#include "log.h"

namespace In{
	IN::IN(){
		this->size_ = NULL;
		this->lines_ = NULL;
		this->ignor_ = NULL;
		this->numberOfChains_ = NULL;
	};

	char** IN::getArr(){
		return this->arrOfLines_;
	};

	char* IN::getLine(int i){
		return this->arrOfLines_[i];
	};

	int IN::getNumOfChains(){
		return this->numberOfChains_;
	};

	int IN::getLinesCounter(){
		return this->lines_;
	};

	int IN::getSizeCounter(){
		return this->size_;
	};

	int IN::getIgnorCounter(){
		return this->ignor_;
	};

	int IN::getCode(char symbol){
		return this->code_[unsigned char(symbol)];
	};

	void IN::setCode(int* code){
		this->code_ = code;
	};

	void IN::setArr(char** arrOfLines){
		this->arrOfLines_ = arrOfLines;
	};

	void IN::setLine(char* line){
		this->arrOfLines_[this->numberOfChains_] = line;
	};

	void IN::setChar(int& currPos, char symbol){
		this->arrOfLines_[this->numberOfChains_][currPos++] = symbol;
	};

	void IN::setLineEnd(int& currChainPosition){
		this->arrOfLines_[this->numberOfChains_++][currChainPosition] = NULL_STR;
		this->arrOfLines_[this->numberOfChains_] = new char[IN_MAX_LEN_TEXT];
		currChainPosition = 0;
	};

	void IN::increaseIgnor(){
		this->ignor_++;
	};

	void IN::increaseLines(){
		this->lines_++;
	};

	void IN::increaseSize(){
		this->size_++;
	};

	void IN::setText(char symbol){
		this->text_[this->size_] = symbol;
	};

	IN getIn(Log::LOG log, wchar_t* infile){
		IN rc;
		std::ifstream file(infile);
		Error::ERROR errorVar;

		int clearedValue[] = IN_CODE_TABLE;
		int currChainPosition = 0;				// позиция в цепочке
		int positionCounter = 0;				// позиция в строке файла
		char tempChar;							// считываемый символ из файла
		char previousChar = 0;					// предыдущий символ, нужен для удаления пробелов

		rc.setCode(clearedValue);

		if (file.good()){
			Log::writeLine(log, "---Начало проверки допустимости символов---", "");

			rc.setArr(new char*[IN_MAX_LINE_NUMBER]);			// выделение памяти под кол-во строк IN_MAX_LINE_NUMBER
			rc.setLine(new char[IN_MAX_LEN_TEXT]);		// выделение памяти для нулевой строки, IN_MAX_LEN_TEXT символов

			tempChar = file.get();

			while (!file.eof())	{
				switch (rc.getCode(tempChar)){
				case IN::A:
					rc.setChar(currChainPosition, tempChar);
					positionCounter++;
					break;


				case IN::D:
					rc.setChar(currChainPosition, tempChar);
					errorVar = ERROR_THROW_LINE(103, NULL, rc.getLinesCounter(), positionCounter);
					Log::writeError(log, errorVar);
					positionCounter++;
					break;


				case IN::I:
					positionCounter++;
					rc.increaseIgnor();
					break;


				case IN::S:
					if (currChainPosition != NULL){
						rc.setLineEnd(currChainPosition);
						positionCounter = 0;
					};

					if (tempChar == IN_CODE_ENDL){
						rc.setChar(currChainPosition, LEX_NEWLINE);
						rc.increaseLines();
					}
					else	rc.setChar(currChainPosition, tempChar);

					rc.setLineEnd(currChainPosition);
					positionCounter = 0;

					break;


				case IN::B:
					if (rc.getCode(previousChar) == IN::A){
						rc.setLineEnd(currChainPosition);
						positionCounter = 0;
					};

					break;


				case IN::Q:
					rc.createLine(tempChar, file, positionCounter);
					positionCounter = 0;
					currChainPosition = 0;
					break;


				default:
					rc.setChar(currChainPosition, tempChar);
					positionCounter++;
					break;
				};

				rc.setText(tempChar);
				rc.increaseSize();
				positionCounter++;
				previousChar = tempChar;
				tempChar = file.get();
			};

			Log::writeLine(log, "---Конец проверки допустимости символов---", "");
		}
		else throw ERROR_THROW(102);

		rc.setText(NULL_STR);
		file.close();

		return rc;
	};

	void IN::createLine(char firstSymbol, std::ifstream& file, int& positionCounter){
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