#pragma once

#include "fst.h"
#include "errors.h"
#include "lexTable.h"
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>

#define IN_MAX_LEN_TEXT 4096
#define IN_MAX_LINE_NUMBER 1024
#define IN_CODE_ENDL '\n'
#define IN_CODE_NEW_LINE '@'
#define IN_CODE_TABLE_SIZE 4096

#define IN_CODE_TABLE {\
	/*0-15*/    IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::I, IN::S, IN::D, IN::D, IN::D, IN::D, IN::D, \
	IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, \
	/*32-47*/   IN::B, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::S, IN::S, IN::S, IN::S, IN::S, IN::S, IN::A, IN::S, \
	IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::S, IN::S, IN::A, IN::S, IN::A, IN::A, \
	/*64-79*/   IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, \
	IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::S, IN::A, IN::S, IN::A, IN::A, \
	/*96-111*/  IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, \
	IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::S, IN::A, IN::S, IN::A, IN::D, \
	\
	/*128-143*/	IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, \
	IN::D, IN::Q, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, IN::D, \
	/*160-175*/	IN::D, IN::A, IN::A, IN::D, IN::D, IN::D, IN::D, IN::A, IN::A, IN::A, IN::A, IN::A, IN::D, IN::D, IN::A, IN::A, \
	IN::D, IN::D, IN::A, IN::A, IN::D, IN::A, IN::D, IN::D, IN::A, IN::A, IN::D, IN::A, IN::A, IN::A, IN::A, IN::A, \
	/*192-207*/ IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, \
	IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, \
	/*224-239*/ IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, \
	IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, \
}

namespace Log{ struct LOG; };

namespace In
{
	class IN
	{
	public:
		enum
		{
			A = 1024,		// ALLOWED,		допустимые
			D = 2048,		// DISALLOWED,	запрещенные
			I = 4096,		// IGNORE,		игнорируемые
			S = 8192,		// SEPARATOR,	сепаратор
			B = 16384,		// SPACE,		пробел
			Q = 32768		// QUOTE,		кавычка
		};

		IN();

		int getCode(char symbol);			// возвращает значение из таблицы допустимости символов для текущего символа строки
		void createLine(char firstSymbol, std::ifstream& file, int& positionCounter);	// для корректного считывания текста в кавычках

		char** getArr();
		char* getLine(int i);
		int getNumOfChains();
		int getLinesCounter();
		int getSizeCounter();
		int getIgnorCounter();

		void setArr(char** arrOfLines);
		void setLine(char* line);
		void setChar(int& currPos, char symbol);	// в текущую строку добавляем symbol
		void setCode(int* code);
		void setLineEnd(int& currChainPosition);
		void setText(char symbol);
		
		void increaseIgnor();
		void increaseLines();
		void increaseSize();

	private:
		int		size_;						// кол-во символов в файле
		int		lines_;						// кол-во строк в файле
		int		ignor_;						// кол-во игнорируемых символов
		int*	code_;						// массив значений таблицы допустимых символов
		int		numberOfChains_;			// количество цепочек
		char	text_[IN_CODE_TABLE_SIZE];	// обработанный текст
		char**	arrOfLines_;				// массив цепочек
	};

	IN getIn(Log::LOG log, wchar_t* infile);
	char*	createLine(char firstSymbol, std::ifstream& file, char* text, int& textSize, int& positionCounter);	// для корректного считывания текста в кавычках
};