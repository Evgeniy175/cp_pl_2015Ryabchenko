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

		wchar_t*		getLogFile();						// return current path of log file
		std::ofstream*	getStream();						// return current stream

		void			setLogFile(wchar_t* logFile);		// set a new path for log file
		void			setStream(std::ofstream* stream);	// set a new stream

		void			line(char* line);					// write 1 line to a file
		void 			line(wchar_t* line);				// write 1 line to a file
		void 			writeLine(char* c, ...);			// write a set of lines to a file
		void 			writeLine(wchar_t* c, ...);			// write a set of lines to a file
		void 			writeLog();							// write log info to a file
		void 			writeCP(CP::Compiler* cp);			// write path's of input file to a file
		void 			writeIn(IN::In* in);				// write info about input data to a file
		void 			writeError(ERROR::Error* error);	// write error to a file
		void 			writeLt(LA::LexAnalyser* la);		// write lexeme table to a file
		void 			writeAt(LA::LexAnalyser* la);		// write auxiliary table to a file

		void close();										// close a stream

		~Log();

	private:
		wchar_t logFile_[SIZE_CHAR_ARRAY];					// file path for logging
		std::ofstream* stream_;								// out stream
	};
};