// by Evgeniy Ryabchenko
// параметры/переменные
// TODO: enum дл€ условий, мб и дл€ действий (делать отдельное поле или нет, т.к. сейчас хранитс€ char*) ; объ€вление функций(прототипы, количество параметров)
// reset compiler
// add begin to auxt
//
#include "stdafx.h"
#include "cp_pl_3sem_R.h"

int _tmain(int argc, _TCHAR* argv[]){
	setlocale(LC_ALL, "Rus");
// 	char** path = new char*[3];
// 	path[0] = "-in:D:\\\\in.txt";
// 	path[1] = "-log:D:\\\\log.txt";
// 	path[2] = "-out:D:\\\\out.txt";

//	initialPath->initialExecute("-in:D:\\\\in.txt");

//	initialPath->initialExecute(L"-in:D:\\\\in.txt");

	PATH::Path* initialPath = new PATH::Path();
	initialPath->initialExecute(argc, argv);

	CP::Compiler* compiler = new CP::Compiler(initialPath);

	try{
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
	delete compiler;
	return 0;
};