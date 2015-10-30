// by Evgeniy Ryabchenko

// TODO: литералы заносить в таблицу лексем ; переименовать графы


#include "stdafx.h"
#include "cp_pl_3sem_R.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int startTime = clock();

	setlocale(LC_ALL, "Rus");
	Log::LOG log = Log::INITLOG;

	try
	{
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);

		Log::writeLog(log);
		Log::writeParm(log, parm);

		In::IN in = In::getIn(log, parm.in);
		LA::lexAnalyser la = LA::create(log, in);

		Log::writeIn(log, in);
		Log::writeLt(log, la.lexTable);
		Log::writeIt(log, la.itTable);

		LA::deleteLa(la);

		Log::close(log);
	}
	catch (Error::ERROR e)
	{
		Log::writeError(log, e);
	};

	unsigned int endTime = clock();
	unsigned int searchTime = endTime - startTime;

	return 0;
};