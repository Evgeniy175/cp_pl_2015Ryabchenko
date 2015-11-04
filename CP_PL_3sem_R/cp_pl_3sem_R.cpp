// by Evgeniy Ryabchenko

// TODO: раздели имена и функции ; переделай под ООП ; переименовать графы ; вектор встроенных фций и пользовательских ; чек ERROR

#include "stdafx.h"
#include "cp_pl_3sem_R.h"

int _tmain(int argc, _TCHAR* argv[]){

	unsigned int startTime, endTime, searchTime;
	startTime = clock();

	CP::Compiler* compiler = new CP::Compiler(argc, argv);

	setlocale(LC_ALL, "Rus");

	try{
		compiler->getLog()->writeLog();
		compiler->getLog()->writeCP(compiler);
		compiler->getLog()->writeIn(compiler->getIn());
		compiler->getLog()->writeLt(compiler->getLA());
		compiler->getLog()->writeAt(compiler->getLA());
		compiler->getLog();

		compiler->polishNotation();

		compiler->getLog()->writeLt(compiler->getLA());
		compiler->getLog()->writeAt(compiler->getLA());

		compiler->getLog()->close();
	}
	catch (ERROR::Error* err){
		compiler->getLog()->writeError(err);
	};

	endTime = clock();
	searchTime = endTime - startTime;

	return 0;
};