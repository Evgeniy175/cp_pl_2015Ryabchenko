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

namespace Log{
	struct LOG{
		wchar_t logfile[PARM_MAX_SIZE];							// �������� ����
		std::ofstream* stream;									// ����� ������
	};

	static const LOG INITLOG = { L"", NULL };
	LOG getlog(wchar_t logfile[]);								// �������� ������
	void writeLine(LOG& log, char* c, ...);						// �������� ������ � ����(��� char*)
	void writeLine(LOG& log, wchar_t* c, ...);					// �������� ������ � ����(��� wchar_t*)
	void writeLog(LOG& log);									// �������� ����� log-����� � ����
	void writeParm(LOG& log, Parm::PARM& parm);					// ������ ����� �������� ������
	void writeIn(LOG& log, In::IN in);							// ������ ���������� � ������� �����
	void writeError(LOG& log, Error::ERROR& error);				// ������ ������
	void writeLt(LOG& log, LA::LexAnalyser* la);					// ������ ����������� �������
	void writeIt(LOG& log, LA::LexAnalyser* la);
	void close(LOG& log);										// �������� ������
};