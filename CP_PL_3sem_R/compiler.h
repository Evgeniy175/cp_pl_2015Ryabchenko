#pragma once

#include "errors.h"
#include "log.h"
#include "lexAnalyzer.h"
#include "in.h"
#include "path.h"
#include <cwchar>
#include <iostream>
#include <string>
#include <list>
#include <stack>

namespace LA{ class LexAnalyzer; };
namespace LT{ class Table; class Element; };
namespace IN{ class In; };
namespace LOG{ class Log; };
namespace PN{ class PolishNotation; };
namespace AT{ enum TYPE; enum DATATYPE; class Table; class Element; };

namespace CP{
	class Compiler{
	public:
		void				execute(PATH::Path& filesPath);
		void				execute(wchar_t* in);
		void				execute(char* in);

		wchar_t*			getInName();
		wchar_t*			getOutName();
		wchar_t*			getLogName();
		LA::LexAnalyzer*	getLa();							// return LexAnalyser
		IN::In*				getIn();
		LOG::Log*			getLog();
		int					getLtSize();						// return lexTable size
		LT::Element*		getElemLt(int i);					// return lexTable element(i)
		int					getAtSize();						// return auxTable size
		AT::Element*		getElemAt(int i);					// return auxTable element(i)

		void				writeLine(char* line, ...);			// write char* line
		void				writeLine(wchar_t* line, ...);		// write wchar_t* line
		void 				writeCp();							// write compiler
		void 				writeIn();
		void 				writeError(ERROR::Error* error);
		void 				writeLt();							// write lexTable
		void 				writeAt();							// write auxTable

		void 				closeLog();

		void				executeIn();
		void				executeLa();

		void				polishNotation();
		bool				executePn(int& position);			// create Polish Notation for
		bool				executeFuncPn(						// create Polish Notation for functions
								std::stack<LT::Element*>& stack,
								std::list<LT::Element>& exitArr,
								int& position);
		int					getPriority(char symbol);			// get symbol priority
		void				modifyAt();							// modify auxTable (ltIndex field) after Polish Notation

		~Compiler();

	private:
		void				reset();

		wchar_t				inPath_[PATH_MAX_NAMESIZE];			// path of a in file
		wchar_t				outPath_[PATH_MAX_NAMESIZE];		// path of a out file
		wchar_t				logPath_[PATH_MAX_NAMESIZE];		// path of a out file
		LOG::Log*			log_;								// write log into file
		IN::In*				in_;								// in for read a file
		LA::LexAnalyzer*	la_;								// lexAnalyser
	};
};