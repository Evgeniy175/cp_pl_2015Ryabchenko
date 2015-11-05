// by Evgeniy Ryabchenko
// TODO:  переименовать графы ; вектор встроенных фций и пользовательских

#include "stdafx.h"
#include "cp_pl_3sem_R.h"

int _tmain(int argc, _TCHAR* argv[]){
	setlocale(LC_ALL, "Rus");
	FilesPath* filesPath = new FilesPath(argc, argv);
	CP::Compiler* compiler = new CP::Compiler(filesPath);

	try{
		compiler->writeLog();
		compiler->writeCP();

		compiler->executeIn();
		compiler->executeLa();

		compiler->writeIn();
		compiler->writeLt();
		compiler->writeAt();
		compiler->getLog();

		compiler->polishNotation();
		compiler->modifyAT();

		compiler->writeLt();
		compiler->writeAt();

		compiler->closeLog();
	}
	catch (ERROR::Error* err){
		compiler->writeError(err);
	};

	return 0;
};