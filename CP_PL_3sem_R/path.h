#pragma once

#include "errors.h"
#include <cwchar>
#include <iostream>
#include <string>

#define PATH_MAX_NAMESIZE	256
#define PATH_IN				L"in"
#define PATH_LOG			L"log"
#define PATH_OUT			L"out"
#define PATH_OUT_POSTFIX	L".out"
#define PATH_LOG_POSTFIX	L".log"
#define PATH_SEPARATOR		L":"

struct FilesPath;

namespace PATH
{
	class Path{
	public:
		Path();
		Path(const Path& initial);							// copy initial path fields into this object

		void initialExecute(int argc, _TCHAR* argv[]);		// create initial path's
		void pushPath(wchar_t* dest, wchar_t* source);		// copy source into dest

		wchar_t* getIn();									// return path of a in file
		wchar_t* getOut();									// return path of a out file
		wchar_t* getLog();									// return path of a out file

	protected:
		wchar_t in_[PATH_MAX_NAMESIZE];						// path of a in file
		wchar_t out_[PATH_MAX_NAMESIZE];					// path of a out file
		wchar_t log_[PATH_MAX_NAMESIZE];					// path of a out file
	};

};