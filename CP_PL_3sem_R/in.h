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
	/*32-47*/   IN::B, IN::C, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::S, IN::S, IN::S, IN::S, IN::S, IN::S, IN::A, IN::S, \
	IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::A, IN::D, IN::S, IN::C, IN::C, IN::C, IN::A, \
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

namespace LOG{ class Log; };

namespace IN{
	enum{
		A = 512,		// ALLOWED symbols
		D = 768,		// DISALLOWED symbols
		I = 1024,		// IGNORE symbols
		S = 1280,		// SEPARATOR symbols
		B = 1536,		// SPACE symbols
		Q = 1792,		// QUOTE symbols
		C = 2048		// CONDITION symbols
	};

	class In{
	public:
		In();

		int		getCode(char symbol);						// return IN_CODE_TABLE value for symbol

		char**	getArr();									// return array of lines
		char*	getLine(int index);							// return line from array of lines using index
		int		getNumOfChains();							// return number of chains
		int		getLinesCounter();							// return number of line counter value
		int		getSizeCounter();							// return size counter value
		int		getIgnorCounter();							// return ignor counter value

		void	setArr(char** arrOfLines);					// set array of lines
		void	setCode(int* code);							// set code symbol table
		void	setLineEnd(int& currChainPosition);			// set the end of the line

		void	addChar(int& currPos, char symbol);			// add symbol to current line
		void	addLine(char* line);						// add line to array of lines
		void	setText(char symbol);						// add symbol to exit text

		void	execute(LOG::Log* log, wchar_t* infile);
		void	createLine(									// correct read line literal(with spaces)
					char firstSymbol,
					std::ifstream& file,
					int& positionCounter);

		void	increaseIgnor();							// like ignor++
		void	increaseLines();							// like lines++
		void	increaseSize();								// like size++

	private:
		char	text_[IN_CODE_TABLE_SIZE];					// raw text from input file
		int		size_;										// number of symbols in the input file
		int		lines_;										// number of lines in the input file
		int		ignor_;										// number of ignored symbols
		int*	code_;										// code symbol table
		char**	arrOfLines_;								// array of lines
		int		numberOfChains_;
	};

	char*		createLine(									// correct read text in quotes(with spaces)
					char firstSymbol,
					std::ifstream& file,
					char* text,
					int& textSize,
					int& positionCounter);
};