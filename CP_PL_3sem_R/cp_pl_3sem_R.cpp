// by Evgeniy Ryabchenko

// TODO: раздели имена и функции ; переделай под ООП ; переименовать графы ; вектор встроенных фций и пользовательских ; чек ERROR

#include "stdafx.h"
#include "cp_pl_3sem_R.h"

int _tmain(int argc, _TCHAR* argv[]){

	unsigned int startTime, endTime, searchTime;

	setlocale(LC_ALL, "Rus");
	LOG::Log* log = new LOG::Log();

	try{
		startTime = clock();

		Parm::PARM parm = Parm::getparm(argc, argv);
		log->getlog(parm.log);
		
		log->writeLog();
		log->writeParm(parm);
		
		In::IN in = In::getIn(log, parm.in); 
		LA::LexAnalyser* la = new LA::LexAnalyser(in.getNumOfChains(), log, in);		

		log->writeIn(in);
		log->writeLt(la);
		log->writeAt(la);
		
		log->close();

		delete la;

		endTime = clock();
	}
	catch (Error::ERROR err){
		log->writeError(err);
	};
	
	searchTime = endTime - startTime;

	return 0;
};