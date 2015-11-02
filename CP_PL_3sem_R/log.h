#pragma once

#include "errors.h"
#include "in.h"
#include "lexAnalyser.h"
#include "parm.h"
#include "time.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <cwchar>

#define SIZE_CHAR_ARRAY 256

namespace LA{ class LexAnalyser; };
namespace In{ class IN; };

namespace LOG{
	class Log{
	public:
		Log();

		wchar_t*		getLogFile();						// 
		std::ofstream*	getStream();						// 
		void			getlog(wchar_t logfile[]);			// создание потока

		void setLogFile(wchar_t* logFile);					//
		void setStream(std::ofstream* stream);				// 

		void writeLine(char* c, ...);						// записать строку в файл(для char*)
		void writeLine(wchar_t* c, ...);					// записать строку в файл(для wchar_t*)
		void writeLog();									// записать шапку log-файла в файл
		void writeParm(Parm::PARM& parm);					// запись путей исходных файлов
		void writeIn(In::IN in);							// запись информации о входном файле
		void writeError(Error::ERROR& error);				// запись ошибки
		void writeLt(LA::LexAnalyser* la);					// вывод таблицы лексем
		void writeAt(LA::LexAnalyser* la);					// вывод дополнительной таблицы

		void close();										// завершить поток

	private:
		wchar_t logFile_[PARM_MAX_SIZE];					// выходной файл
		std::ofstream* stream_;								// поток вывода
	};
};