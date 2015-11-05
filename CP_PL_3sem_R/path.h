#pragma once

#include "errors.h"
#include "filesPath.h"
#include <cwchar>
#include <iostream>
#include <string>

#define FILES_MAX_NAMESIZE 256

namespace PATH
{
	class Path{
	public:
		Path(wchar_t* in, wchar_t* out, wchar_t* log);

		wchar_t* getIn();
		wchar_t* getOut();
		wchar_t* getLog();

	protected:
		wchar_t in_[FILES_MAX_NAMESIZE];
		wchar_t out_[FILES_MAX_NAMESIZE];
		wchar_t log_[FILES_MAX_NAMESIZE];
	};

};