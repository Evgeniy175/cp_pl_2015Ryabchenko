#pragma once

#include "errors.h"
#include <cwchar>
#include <iostream>
#include <string>

#define PATH_MAX_NAMESIZE			256
#define PATH_OUT_POSTFIX			L".out"
#define PATH_LOG_POSTFIX			L".log"
#define PATH_SEPARATOR				L":"
#define PATH_IN_WCHAR				L"in"
#define PATH_LOG_WCHAR				L"log"
#define PATH_OUT_WCHAR				L"out"
#define PATH_IN						"in"
#define PATH_LOG					"log"
#define PATH_OUT					"out"

namespace PATH
{
	class Path{
	public:
		void initialExecute(int linesNumber, char* lines[]);		// create initial path's
		void initialExecute(int linesNumber, wchar_t* lines[]);		// create initial path's

		void initialExecute(										// create initial path's
			char* inFile,												// in file path
			char* outFile = NULL_STR,									// out file path
			char* logFile = NULL_STR									// log file path
			);

		void initialExecute(										// create initial path's
			wchar_t* inFile,											// in file path
			wchar_t* outFile = NULL_STR,								// out file path
			wchar_t* logFile = NULL_STR									// log file path
			);

		void pushPath(wchar_t* dest, wchar_t* source);				// copy source into dest

		wchar_t* getIn();											// return path of a in file
		wchar_t* getOut();											// return path of a out file
		wchar_t* getLog();											// return path of a out file

	protected:
		wchar_t in_[PATH_MAX_NAMESIZE];								// path of a in file
		wchar_t out_[PATH_MAX_NAMESIZE];							// path of a out file
		wchar_t log_[PATH_MAX_NAMESIZE];							// path of a out file
	};

};