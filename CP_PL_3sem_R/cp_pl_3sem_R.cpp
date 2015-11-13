// by Evgeniy Ryabchenko
// TODO: NULL_STR â stdafx.h

#include "stdafx.h"
#include "cp_pl_3sem_R.h"

int _tmain(int argc, _TCHAR* argv[]){
	setlocale(LC_ALL, "Rus");

	CP::Compiler compiler;
	PATH::Path initialPath;

	try{
		initialPath.initialExecute(argc, argv);
		compiler.execute(initialPath);
		compiler.executeIn();
		compiler.executeLa();
		compiler.polishNotation();
	}
	catch (ERROR::Error* err){
		compiler.writeError(err);
	};
	return 0;
};