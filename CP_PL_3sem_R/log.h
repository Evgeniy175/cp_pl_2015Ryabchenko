#pragma once

#include "errors.h"
#include "in.h"
#include "lexAnalyser.h"
#include "compiler.h"
#include "time.h"
#include <iomanip>
#include <fstream>
#include <iostream>
#include <cwchar>

#define SIZE_CHAR_ARRAY 256

namespace CP{ class Compiler; };
namespace LA{ class LexAnalyser; };
namespace IN{ class In; };

namespace LOG{
	class Log{
	public:
		Log(wchar_t logFile[]);

		wchar_t*		getLogFile();						// 
		std::ofstream*	getStream();						//

		void setLogFile(wchar_t* logFile);					//
		void setStream(std::ofstream* stream);				// 

		void line(char* line);							// записать строку в файл(для char*)
		void line(wchar_t* line);						// записать строку в файл(для wchar_t*)
		void writeLine(char* c, ...);						// записать строку в файл(для char*)
		void writeLine(wchar_t* c, ...);					// записать строку в файл(для wchar_t*)
		void writeLog();									// записать шапку log-файла в файл
		void writeCP(CP::Compiler* cp);						// запись путей исходных файлов
		void writeIn(IN::In* in);							// запись информации о входном файле
		void writeError(ERROR::Error* error);				// запись ошибки
		void writeLt(LA::LexAnalyser* la);					// вывод таблицы лексем
		void writeAt(LA::LexAnalyser* la);					// вывод дополнительной таблицы

		void close();										// завершить поток

	private:
		wchar_t logFile_[SIZE_CHAR_ARRAY];					// выходной файл
		std::ofstream* stream_;								// поток вывода
	};
};