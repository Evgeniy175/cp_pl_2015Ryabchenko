// by Evgeniy Ryabchenko
// параметры/переменные
// TODO: enum для условий, мб и для действий

#include "stdafx.h"
#include "cp_pl_3sem_R.h"

int _tmain(int argc, _TCHAR* argv[]){
	setlocale(LC_ALL, "Rus");
// 	char** path = new char*[3];
// 	path[0] = "-in:D:\\\\in.txt";
// 	path[1] = "-log:D:\\\\log.txt";
// 	path[2] = "-out:D:\\\\out.txt";
//	initialPath->initialExecute("-in:D:\\\\in.txt");

	PATH::Path* initialPath = new PATH::Path();
	initialPath->initialExecute(argc, argv);

	CP::Compiler* compiler = new CP::Compiler(initialPath);

	try{
		compiler->writeLog();
		compiler->writeCp();

		compiler->executeIn();

		compiler->writeIn();

		compiler->executeLa();

		compiler->writeLt();
		compiler->writeAt();

		compiler->polishNotation();
		compiler->modifyAt();

		compiler->writeLt();
		compiler->writeAt();
		compiler->closeLog();
	}
	catch (ERROR::Error* err){
		compiler->writeError(err);
	};
	return 0;
};