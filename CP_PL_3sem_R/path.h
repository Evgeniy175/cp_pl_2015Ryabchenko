#pragma once

#include "errors.h"
#include <cwchar>
#include <iostream>
#include <string>

#define FILES_MAX_NAMESIZE	256
#define FILES_OUT_POSTFIX	L".out"
#define FILES_LOG_POSTFIX	L".log"

namespace PATH
{
	class Path{
	public:
		Path(wchar_t* in, wchar_t* out, wchar_t* log);

		wchar_t* getIn();
		wchar_t* getOut();
		wchar_t* getLog();

	private:
		wchar_t in_[FILES_MAX_NAMESIZE];
		wchar_t out_[FILES_MAX_NAMESIZE];
		wchar_t log_[FILES_MAX_NAMESIZE];
	};

};