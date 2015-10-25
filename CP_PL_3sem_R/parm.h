#pragma once

#include "errors.h"
#include <cwchar>
#include <iostream>
#include <string>

#define PARM_MAX_SIZE 300
#define PARM_OUT_DEFAULT_EXT L".out"
#define PARM_LOG_DEFAULT_EXT L".log"

namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];
		wchar_t out[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
	};

	PARM getparm(int argc, _TCHAR* argv[]);
};