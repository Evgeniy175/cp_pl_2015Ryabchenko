#include "stdafx.h"
#include "parm.h"

namespace Parm
{
	struct PARM;

	PARM getparm(int argc, _TCHAR* argv[])
	{
		PARM rc;
		wchar_t* temp;
		wchar_t temp2[PARM_MAX_SIZE];
		int i = 1;

		if (argc < 2)	throw ERROR_THROW(100);

		while (i < argc)	if (wcslen(argv[i++])>PARM_MAX_SIZE) throw ERROR_THROW(101);

		for (i = 1; i < argc; i++)
		{
			temp = wcsstr(argv[i], L":");
			wcscpy_s(temp2, temp + 1);

			if (i == 1) wcscpy_s(rc.in, temp2);
			else
			{
				if (strstr((char*)(argv[i][1]), "out")) wcscpy_s(rc.out, temp2);
				else if (strstr((char*)(argv[i][1]), "log")) wcscpy_s(rc.log, temp2);
			};
		};

		if (argc < 4)
		{
			if (argc == 3)
			{
				if (strstr((char*)(argv[2][1]), "log"))
				{
					wcscpy_s(rc.out, rc.in);
					wcsncat_s(rc.out, PARM_OUT_DEFAULT_EXT, 4);
				}
				else if (strstr((char*)(argv[2][1]), "out"))
				{
					wcscpy_s(rc.log, rc.in);
					wcsncat_s(rc.log, PARM_LOG_DEFAULT_EXT, 4);
				};
			}
			else if (argc == 2)
			{
				wcscpy_s(rc.out, rc.in);
				wcsncat_s(rc.out, PARM_OUT_DEFAULT_EXT, 4);
				wcscpy_s(rc.log, rc.in);
				wcsncat_s(rc.log, PARM_LOG_DEFAULT_EXT, 4);
			};
		};

		return rc;
	};
};