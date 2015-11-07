#pragma once

#include "errors.h"
#include "log.h"
#include "lexAnalyser.h"
#include "in.h"
#include "path.h"
#include <cwchar>
#include <iostream>
#include <string>
#include <list>
#include <stack>

namespace LA{ class LexAnalyser; };
namespace LT{ class Table; class Element; };
namespace IN{ class In; };
namespace LOG{ class Log; };
namespace PN{ class PolishNotation; };
namespace AT{ enum TYPE; enum DATATYPE; class Table; class Element; };

namespace CP{
	class Compiler{
	public:
		Compiler(PATH::Path* filesPath);

		wchar_t*			getInName();
		wchar_t*			getOutName();
		wchar_t*			getLogName();
		LA::LexAnalyser*	getLA();							// return LexAnalyser
		IN::In*				getIn();
		LOG::Log*			getLog();
		int					getLTsize();						// return lexTable size
		LT::Element*		getElemLT(int i);					// return lexTable element(i)
		int					getATsize();						// return auxTable size
		AT::Element*		getElemAT(int i);					// return auxTable element(i)

		void				writeLine(char* line, ...);			// write char* line
		void				writeLine(wchar_t* line, ...);		// write wchar_t* line
		void 				writeLog();							// write log
		void 				writeCP();							// write compiler
		void 				writeIn();
		void 				writeError(ERROR::Error* error);
		void 				writeLt();							// write lexTable
		void 				writeAt();							// write auxTable

		void 				closeLog();

		void				executeIn();
		void				executeLa();

		void				polishNotation();
		bool				create(int& position);				// create Polish Notation
		bool				funcCreate(							// create Polish Notation for functions
								std::stack<LT::Element*>& stack,
								std::list<LT::Element>& exitArr,
								int& position);
		int					getPriority(char symbol);			// get symbol priority
		void				modifyAT();							// modify auxTable (ltIndex field) after Polish Notation

		~Compiler();

	private:
		PATH::Path*			path_;								// field to store path of files
		LOG::Log*			log_;								// write log into file
		IN::In*				in_;								// in for read a file
		LA::LexAnalyser*	la_;								// lexAnalyser
	};
};