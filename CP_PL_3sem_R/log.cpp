#include "stdafx.h"
#include "log.h"

namespace Log
{
	LOG getlog(wchar_t logfile[])
	{
		LOG rc;
		rc.stream = new std::ofstream(logfile);

		if ((rc.stream)->good())	wcscpy_s(rc.logfile, logfile);
		else	throw ERROR_THROW(104);

		return rc;
	};

	void writeLine(LOG& log, char* c, ...)
	{
		char **p = &c;

		for (int i = 0; *(p + i)[0] != NULL_STR; i++)	*log.stream << *(p + i);

		*log.stream << std::endl;
	};

	void writeLine(LOG& log, wchar_t* c, ...)
	{
		char temp[SIZE_CHAR_ARRAY];
		char *p;
		wchar_t **CyclePointer = &c;

		for (int i = 0; *(CyclePointer + i)[0] != NULL_STR; i++)
		{
			wcstombs_s(0, temp, *(CyclePointer + i), sizeof(temp));
			p = temp;
			*log.stream << p << ' ';
		};

		*log.stream << std::endl;
	};

	void writeLog(LOG& log)
	{
		time_t rawtime = time(NULL);
		struct tm timeinfo;
		char buf[SIZE_CHAR_ARRAY];
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		strftime(buf, sizeof(buf), "%d.%m.%Y  %H:%M:%S", &timeinfo);
		*log.stream << "--- Протокол ---" << std::endl << "Дата:  " << buf << std::endl;
	};

	void writeParm(LOG& log, Parm::PARM& parm)
	{
		*log.stream << std::endl << "---Параметры---" << std::endl << "-log: ";
		writeLine(log, parm.log);
		*log.stream << "-out: ";
		writeLine(log, parm.out);
		*log.stream << "-in: ";
		writeLine(log, parm.in);
		*log.stream << std::endl;
	};

	void writeIn(LOG& log, In::IN in)
	{
		*log.stream << std::endl << std::endl << "---Исходные данные---" << std::endl << "Количество символов: " << in.getSizeCounter() << std::endl;
		*log.stream << "Проигнорировано: " << in.getIgnorCounter() << std::endl << "Количество строк: " << in.getLinesCounter() << std::endl;
	};

	void writeError(LOG& log, Error::ERROR& error)
	{
		*log.stream << std::endl << "Ошибка " << error.id << ": " << error.message;
		if (error.inext.line != -1)	*log.stream << "   в строке номер " << error.inext.line;
		if (error.inext.col != -1)	*log.stream << ", позиция " << error.inext.col;
		*log.stream << std::endl << std::endl;
	};

	void writeLt(LOG& log, LT::LexTable* lt)
	{
		int currentLineNumber = 0;
		*log.stream << std::endl << "---Lex table start---";

		for (int i = 0; i < lt->size_; i++)
		{
			if (i == 0)	*log.stream << std::endl << "00" << currentLineNumber++ << "   ";

			if (lt->table_[i].lexema_ == LEX_NEWLINE)
			{
				if (currentLineNumber < 10)	*log.stream << std::endl << "00" << currentLineNumber++ << "   ";
				else if (currentLineNumber < 100)	*log.stream << std::endl << "0" << currentLineNumber++ << "   ";
				else if (currentLineNumber < 1000)	*log.stream << std::endl << "" << currentLineNumber++ << "   ";
			}
			else *log.stream << lt->table_[i].lexema_;
		};

		*log.stream << std::endl << "---Lex table end---" << std::endl;
	};

	void writeIt(LOG& log, IT::IdTable* it)
	{
		IT::IdDataType* temp = new IT::IdDataType();

		*log.stream << std::endl << "---Identificator table start---" << std::endl;

		for (int i = 0; i < it->size_; i++)
		{
			*log.stream << it->table_[i].id_ << '\t';
			*log.stream << IT::getIdDataName(it->table_[i].idDataType_) << '\t';
			*log.stream << it->table_[i].idType_ << std::endl;
		};

		*log.stream << "---Identificator table end---" << std::endl;
		delete temp;
	};

	void close(LOG& log)
	{
		log.stream->close();
	};
};