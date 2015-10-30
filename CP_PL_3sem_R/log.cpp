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
		*log.stream << std::endl << std::endl << "---Lex table start---" << std::endl
			<< "Size: " << lt->size_ << std::endl << std::endl
			<< "NUMBER\t\t" << "LEXEMA\t\t" << "LINE\t\t" << "TI INDEX" << std::endl;

		for (int i = 0; i < lt->size_; i++)
		{
			*log.stream << i << "\t\t" << lt->table_[i].lexema_ << "\t\t" << lt->table_[i].lineNumber_ << "\t\t";
			if (lt->table_[i].TI_Index_ >= NULL) *log.stream << lt->table_[i].TI_Index_ << std::endl;
			else *log.stream << "no_matches" << std::endl;
		};

		*log.stream << "---Lex table end---" << std::endl;
	};

	void writeIt(LOG& log, IT::IdTable* it)
	{
		*log.stream << std::endl << std::endl << "---Identificator table start---" << std::endl
			<< "Help: NaL - Not a Literal\t" << std::endl << "Size: " << it->size_ << std::endl << std::endl
			<< "NUMBER\t" << "ID\t\t\t" << "TYPE\t\t" << "ID TYPE\t\t" << "ELEMENT TYPE\t\t"
			<< "LT INDEX\t" << "LITERAL VALUE" << std::endl;

		for (int i = 0; i < it->size_; i++)
		{
			*log.stream << i << "\t" << std::setw(8) << it->table_[i].id_ << "\t\t"
				<< IT::getDataName(it->table_[i].idDataType_) << "\t\t" << it->table_[i].idType_
				<< "\t\t";
			if (it->table_[i].elementType_ == IT::ELEMENTTYPE::ID)
				*log.stream << "identificator";
			else if (it->table_[i].elementType_ == IT::ELEMENTTYPE::LITERAL)
				*log.stream << "literal\t";
				
			*log.stream << "\t\t" << it->table_[i].idxFirstLE_ << "\t\t";

 			if (it->table_[i].elementType_ == IT::ELEMENTTYPE::LITERAL)
 			{
 				if (it->table_[i].idDataType_ == IT::IDDATATYPE::NUM)
 					*log.stream << it->table_[i].value_.intValue_;
 
 				else if (it->table_[i].idDataType_ == IT::IDDATATYPE::LINE)
 					*log.stream << it->table_[i].value_.vstr_.str_;
 			}
 			else *log.stream << "-NaL-";

			*log.stream << std::endl;
		};

		*log.stream << "---Identificator table end---";
	};

	void close(LOG& log)
	{
		log.stream->close();
	};
};