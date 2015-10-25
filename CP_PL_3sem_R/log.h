#pragma once

#include "errors.h"
#include "in.h"
#include "lexAnalyser.h"
#include "idTable.h"
#include "parm.h"
#include "time.h"
#include <fstream>
#include <iostream>
#include <cwchar>

#define SIZE_CHAR_ARRAY 256

namespace Log
{
	struct LOG
	{
		wchar_t logfile[PARM_MAX_SIZE];							// выходной файл
		std::ofstream* stream;									// поток вывода
	};

	static const LOG INITLOG = { L"", NULL };
	LOG getlog(wchar_t logfile[]);								// создание потока
	void writeLine(LOG& log, char* c, ...);						// записать строку в файл(для char*)
	void writeLine(LOG& log, wchar_t* c, ...);					// записать строку в файл(для wchar_t*)
	void writeLog(LOG& log);									// записать шапку log-файла в файл
	void writeParm(LOG& log, Parm::PARM& parm);					// запись путей исходных файлов
	void writeIn(LOG& log, In::IN in);							// запись информации о входном файле
	void writeError(LOG& log, Error::ERROR& error);				// запись ошибки
	void writeLt(LOG& log, LT::LexTable* lt);					// запись лексической таблицы
	void writeIt(LOG& log, IT::IdTable* it);
	void close(LOG& log);										// закрытие потока
};