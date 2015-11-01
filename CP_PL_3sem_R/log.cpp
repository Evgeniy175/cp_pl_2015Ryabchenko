#include "stdafx.h"
#include "log.h"

namespace Log
{
	LOG getlog(wchar_t logfile[]){
		LOG rc;
		rc.stream = new std::ofstream(logfile);

		if ((rc.stream)->good())
			wcscpy_s(rc.logfile, logfile);

		else
			throw ERROR_THROW(104);

		return rc;
	};

	void writeLine(LOG& log, char* c, ...){
		char **p = &c;

		for (int i = 0; *(p + i)[0] != NULL_STR; i++)
			*log.stream << *(p + i);

		*log.stream << std::endl;
	};

	void writeLine(LOG& log, wchar_t* c, ...){
		char temp[SIZE_CHAR_ARRAY];
		char *p;
		wchar_t **CyclePointer = &c;

		for (int i = 0; *(CyclePointer + i)[0] != NULL_STR; i++){
			wcstombs_s(0, temp, *(CyclePointer + i), sizeof(temp));
			p = temp;
			*log.stream << p << ' ';
		};

		*log.stream << std::endl;
	};

	void writeLog(LOG& log){
		time_t rawtime = time(NULL);
		struct tm timeinfo;
		char buf[SIZE_CHAR_ARRAY];
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		strftime(buf, sizeof(buf), "%d.%m.%Y  %H:%M:%S", &timeinfo);
		*log.stream << "--- Протокол ---" << std::endl << "Дата:  " << buf << std::endl;
	};

	void writeParm(LOG& log, Parm::PARM& parm){
		*log.stream << std::endl << "---Параметры---" << std::endl << "-log: ";
		writeLine(log, parm.log);
		*log.stream << "-out: ";
		writeLine(log, parm.out);
		*log.stream << "-in: ";
		writeLine(log, parm.in);
		*log.stream << std::endl;
	};

	void writeIn(LOG& log, In::IN in){
		*log.stream << std::endl << std::endl << "---Исходные данные---" << std::endl
			<< "Количество символов: "	<< in.getSizeCounter()	<< std::endl
			<< "Проигнорировано: "		<< in.getIgnorCounter()	<< std::endl 
			<< "Количество строк: "		<< in.getLinesCounter()	<< std::endl;
	};

	void writeError(LOG& log, Error::ERROR& error){
		*log.stream << std::endl << "Ошибка " << error.id << ": " << error.message;

		if (error.inext.line != -1)
			*log.stream << " строка " << error.inext.line;

		if (error.inext.col != -1)
			*log.stream << ", позиция " << error.inext.col;

		*log.stream << std::endl << std::endl;
	};

	void writeLt(LOG& log, LA::LexAnalyser& la){
		int currentLineNumber = 0;

		*log.stream << std::endl << std::endl << "---Lex table start---" << std::endl
			<< "Size: " << la.lexTable->getSize() << std::endl << std::endl
			<< "NUMBER\t\t" << "LEXEMA\t\t" << "LINE\t\t" << "TI INDEX" << std::endl;

		for (int i = 0; i < la.lexTable->getSize(); i++){
			*log.stream << i << "\t\t" << la.lexTable->getElem(i)->getLex()
				<< "\t\t" << la.lexTable->getElem(i)->getLineNumber() << "\t\t";

			if (la.lexTable->getElem(i)->getIdx() >= NULL) 
				*log.stream << la.lexTable->getElem(i)->getIdx() << std::endl;

			else *log.stream << "no_matches" << std::endl;
		};

		*log.stream << "---Lex table end---" << std::endl;
	};

	void writeIt(LOG& log, LA::LexAnalyser& la){
		char lexeme;

		*log.stream << std::endl << std::endl << "---Identificator table start---" 
			<< std::endl << "Help: NaL - Not a Literal\t" << std::endl << "Size: "
			<< la.auxTable->size_ << std::endl << std::endl << "NUMBER\t"
			<< std::setw(TI_ID_MAXSIZE) << "NAME\t\t" << "FUNC NAME\t\t" << "DATA TYPE\t" << "TYPE\t"
			<< "LT INDEX\t" << "VALUE" << std::endl;

		for (int i = 0; i < la.auxTable->size_; i++){
			lexeme = la.lexTable->getElem(la.auxTable->table_[i].getIdx() - 1)->getLex();

			*log.stream << i << '\t' 
				<< std::setw(TI_ID_MAXSIZE) << la.auxTable->table_[i].getName()
				<< "\t\t" << std::setw(TI_ID_MAXSIZE) << la.auxTable->table_[i].getFuncName()
				<< "\t\t" << IT::getDataName(la.auxTable->table_[i].getDataType())
				<< "\t\t" << la.auxTable->table_[i].getType()
				<< '\t' << la.auxTable->table_[i].getIdx() << "\t\t";

			if (lexeme == LEX_LITERAL){		//TODO: фция и массив 
				if (la.auxTable->table_[i].getDataType() == IT::DATATYPE::NUM)
					*log.stream << la.auxTable->table_[i].getIntVal();
 
				else if (la.auxTable->table_[i].getDataType() == IT::DATATYPE::LINE)
					*log.stream << la.auxTable->table_[i].getStrVal();
 			}
 			else *log.stream << "-NaL-";

			*log.stream << std::endl;
		};

		*log.stream << "---Identificator table end---";
	};

	void close(LOG& log){
		log.stream->close();
	};
};