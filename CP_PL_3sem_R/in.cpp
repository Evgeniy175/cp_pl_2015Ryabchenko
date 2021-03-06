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

	void In::addLine(char* line){
		this->arrOfLines_[this->numberOfChains_] = line;
	};

	void In::addChar(int& currPos, char symbol){
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

	void In::execute(LOG::Log* log, wchar_t* infile){
		std::ifstream file(infile);
		ERROR::Error* errorVar;

		int clearedValue[] = IN_CODE_TABLE;
		int currChainPosition = 0;				// ������� � �������
		int positionCounter = 0;				// ������� � ������ �����
		char tempChar;							// ����������� ������ �� �����
		char previousChar = 0;					// ���������� ������, ����� ��� �������� ��������

		this->setCode(clearedValue);

		if (file.good()){
			log->writeLine("---������ �������� ������������ ��������---", "");

			this->setArr(new char*[IN_MAX_LINE_NUMBER]);	// ��������� ������ ��� ���-�� ����� In_MAX_LInE_NUMBER
			this->addLine(new char[IN_MAX_LEN_TEXT]);		// ��������� ������ ��� ������� ������, In_MAX_LEN_TEXT ��������

			tempChar = file.get();

			while (!file.eof())	{
				switch (this->getCode(tempChar)){
				case IN::A:
					this->addChar(currChainPosition, tempChar);
					positionCounter++;
					break;

				case IN::D:
					this->addChar(currChainPosition, tempChar);
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
 						this->addChar(currChainPosition, LEX_NEW_LINE);
 						this->increaseLines();
 					}
					else{
						this->addChar(currChainPosition, tempChar);
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

				case IN::C:
					if (this->getCode(previousChar) != IN::C
						&& currChainPosition != NULL){
						this->setLineEnd(currChainPosition);
						positionCounter = 0;
					};
					this->addChar(currChainPosition, tempChar);

					if (this->getCode(file.peek()) != IN::C){
						this->setLineEnd(currChainPosition);
						positionCounter = 0;
					};
					break;

				default:
					this->addChar(currChainPosition, tempChar);
					positionCounter++;
					break;
				};

				this->setText(tempChar);
				this->increaseSize();
				positionCounter++;
				previousChar = tempChar;
				tempChar = file.get();
			};

			log->writeLine("---����� �������� ������������ ��������---\n", "");
		}
		else
			throw ERROR_THROW(102);

		this->setText(NULL_STR);
		file.close();
	};
};