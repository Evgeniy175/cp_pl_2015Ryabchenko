// by Evgeniy Ryabchenko

// TODO: раздели имена и функции ; переделай под ООП ; переименовать графы ; вектор встроенных фций и пользовательских ; чек ERROR

#include "stdafx.h"
#include "cp_pl_3sem_R.h"

int _tmain(int argc, _TCHAR* argv[]){

	unsigned int startTime, endTime, searchTime;

	setlocale(LC_ALL, "Rus");
	Log::LOG log = Log::INITLOG;

	try{
		
		Parm::PARM parm = Parm::getparm(argc, argv);
		log = Log::getlog(parm.log);
		
		Log::writeLog(log);
		Log::writeParm(log, parm);

		startTime = clock();
		In::IN in = In::getIn(log, parm.in); 
		LA::LexAnalyser* la = new LA::LexAnalyser(in.getNumOfChains(), log, in);		
		endTime = clock();

		Log::writeIn(log, in);
		Log::writeLt(log, la);
		Log::writeIt(log, la);		
		
		Log::close(log);

		delete la;
	}
	catch (Error::ERROR e){
		Log::writeError(log, e);
	};
	
	searchTime = endTime - startTime;

	return 0;
};