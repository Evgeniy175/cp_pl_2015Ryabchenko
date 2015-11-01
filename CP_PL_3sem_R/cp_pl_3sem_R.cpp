// by Evgeniy Ryabchenko

// TODO: раздели имена и функции ; переделай под ООП ; переименовать графы ; вектор встроенных фций и пользовательских ; чек ERROR


#include "stdafx.h"
#include "cp_pl_3sem_R.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int startTime;
	unsigned int endTime;

	setlocale(LC_ALL, "Rus");
	Log::LOG log = Log::INITLOG;

	try
	{
		startTime = clock();
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		
		Log::writeLog(log);
		Log::writeParm(log, parm);
		
		In::IN in = In::getIn(log, parm.in); 
		LA::LexAnalyser la = LA::create(log, in);
		
		Log::writeIn(log, in);
		Log::writeLt(log, la);
		Log::writeIt(log, la);		

		LA::deleteLa(la);

		Log::close(log);
		endTime = clock();
		
	}
	catch (Error::ERROR e)
	{
		Log::writeError(log, e);
	};

	
	unsigned int searchTime = endTime - startTime;

	return 0;
};