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
		Compiler(wchar_t* in, wchar_t* out, wchar_t* log);

		wchar_t*			getInName();
		wchar_t*			getOutName();
		wchar_t*			getLogName();
		LA::LexAnalyser*	getLA();					// get LexAnalyser
		IN::In*				getIn();
		LOG::Log*			getLog();
		int					getLTsize();				// return lexTable size
		LT::Element*		getElemLT(int i);			// return lexTable element(i)
		int					getATsize();				// return auxTable size
		AT::Element*		getElemAT(int i);			// return auxTable element(i)

		void writeLine(char* c, ...);
		void writeLine(wchar_t* c, ...);
		void writeLog();
		void writeCP();
		void writeIn();
		void writeError(ERROR::Error* error);
		void writeLt();
		void writeAt();
		void closeLog();

		void executeIn();
		void executeLa();


		void	polishNotation();
		bool	checkPn(int& position);					// сделать в польскую запись
		bool	buildPnFunc(							// для разбора функций
					std::stack<LT::Element*>& stack,
					std::list<LT::Element>& exitArr,
					int& position);
		int		getPriority(char symbol);				// приоритет символа
		void	modifyAT();								// модифицировать дополнительную таблицу после отработки польской записи

	private:
		PATH::Path*			path_;
		LA::LexAnalyser*	la_;
		IN::In*				in_;
		LOG::Log*			log_;
	};
};