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
		void			getlog(wchar_t logfile[]);			// �������� ������

		void setLogFile(wchar_t* logFile);					//
		void setStream(std::ofstream* stream);				// 

		void writeLine(char* c, ...);						// �������� ������ � ����(��� char*)
		void writeLine(wchar_t* c, ...);					// �������� ������ � ����(��� wchar_t*)
		void writeLog();									// �������� ����� log-����� � ����
		void writeParm(Parm::PARM& parm);					// ������ ����� �������� ������
		void writeIn(In::IN in);							// ������ ���������� � ������� �����
		void writeError(Error::ERROR& error);				// ������ ������
		void writeLt(LA::LexAnalyser* la);					// ����� ������� ������
		void writeAt(LA::LexAnalyser* la);					// ����� �������������� �������

		void close();										// ��������� �����

	private:
		wchar_t logFile_[PARM_MAX_SIZE];					// �������� ����
		std::ofstream* stream_;								// ����� ������
	};
};