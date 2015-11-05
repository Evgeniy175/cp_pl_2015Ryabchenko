// by Evgeniy Ryabchenko
// TODO:  переименовать графы ; вектор встроенных фций и пользовательских

#include "stdafx.h"
#include "cp_pl_3sem_R.h"

void createParm(int argc, _TCHAR* argv[], wchar_t* in, wchar_t* out, wchar_t* log){
	wchar_t* temp = new wchar_t;

	if (argc < 2)
		throw ERROR_THROW(100);

	if (argc > 1){
		for (int i = 1; i < argc; i++){
			if (wcsstr(argv[i], L"in")){
				temp = wcsstr(argv[i], L":");
				temp++;
				wcscpy(in, temp);
				wcscpy(out, temp);
				wcscpy(log, temp);
				wcscat(out, FILES_OUT_POSTFIX);
				wcscat(log, FILES_LOG_POSTFIX);
			}
			else if (wcsstr(argv[i], L"out")){
				wcscpy(out, argv[i]);
			}
			else if (wcsstr(argv[i], L"log")){
				wcscpy(log, argv[i]);
			};
		};
	};
};

int _tmain(int argc, _TCHAR* argv[]){
	wchar_t* inPath = new wchar_t[FILES_MAX_NAMESIZE];
	wchar_t* outPath = new wchar_t[FILES_MAX_NAMESIZE];
	wchar_t* logPath = new wchar_t[FILES_MAX_NAMESIZE];

	setlocale(LC_ALL, "Rus");

	createParm(argc, argv, inPath, outPath, logPath);

	CP::Compiler* compiler = new CP::Compiler(inPath, outPath, logPath);

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