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

		void line(char* line);							// �������� ������ � ����(��� char*)
		void line(wchar_t* line);						// �������� ������ � ����(��� wchar_t*)
		void writeLine(char* c, ...);						// �������� ������ � ����(��� char*)
		void writeLine(wchar_t* c, ...);					// �������� ������ � ����(��� wchar_t*)
		void writeLog();									// �������� ����� log-����� � ����
		void writeCP(CP::Compiler* cp);						// ������ ����� �������� ������
		void writeIn(IN::In* in);							// ������ ���������� � ������� �����
		void writeError(ERROR::Error* error);				// ������ ������
		void writeLt(LA::LexAnalyser* la);					// ����� ������� ������
		void writeAt(LA::LexAnalyser* la);					// ����� �������������� �������

		void close();										// ��������� �����

	private:
		wchar_t logFile_[SIZE_CHAR_ARRAY];					// �������� ����
		std::ofstream* stream_;								// ����� ������
	};
};