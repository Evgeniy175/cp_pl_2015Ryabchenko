#pragma once

#include "errors.h"
#include "log.h"
#include "lexAnalyser.h"
#include "in.h"
#include <cwchar>
#include <iostream>
#include <string>
#include <list>
#include <stack>

#define CP_MAX_SIZE 256
#define CP_OUT L".out"
#define CP_LOG L".log"

namespace LA{ class LexAnalyser; };
namespace LT{ class Element; };
namespace IN{ class In; };
namespace LOG{ class Log; };
namespace PN{ class PolishNotation; };

namespace CP{
	class Compiler{
	public:
		Compiler(int argc, _TCHAR* argv[]);

		wchar_t*			getInName();
		wchar_t*			getOutName();
		wchar_t*			getLogName();
 		LA::LexAnalyser*	getLA();
 		IN::In*				getIn();
 		LOG::Log*			getLog();

		void				polishNotation();
		bool				checkPn(int& position);
		int					getPnPriority(char symbol);

	private:
		wchar_t inStr_[CP_MAX_SIZE];
		wchar_t outStr_[CP_MAX_SIZE];
		wchar_t logStr_[CP_MAX_SIZE];

		LA::LexAnalyser*	la_;
		IN::In*				in_;
		LOG::Log*			log_;
	};
};