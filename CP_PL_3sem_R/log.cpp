#include "stdafx.h"
#include "log.h"

namespace LOG
{
	Log::Log(wchar_t logFile[]){
		this->setStream(new std::ofstream(logFile));

		if ((this->getStream())->good())
			this->setLogFile(logFile);

		else
			throw ERROR_THROW(104);

		this->writeLog();
	}

	wchar_t* Log::getLogFile(){
		return this->logFile_;
	};

	std::ofstream* Log::getStream(){
		return this->stream_;
	};

	void Log::setLogFile(wchar_t* logFile){
		wcscpy_s(this->logFile_, logFile);
	};

	void Log::setStream(std::ofstream* stream){
		this->stream_ = stream;
	}; 
	
	void Log::line(char* line){
		*(this->getStream()) << line;
	};

	void Log::line(wchar_t* line){
		*(this->getStream()) << line << ' ';
	};

	void Log::newLine(){
		*(this->getStream()) << std::endl;
	};

	void Log::writeLine(char* c, ...){
		char **p = &c;

		for (int i = 0; *(p + i)[0] != NULL_STR; i++)
			*(this->getStream()) << *(p + i);
	};

	void Log::writeLine(wchar_t* c, ...){
		char temp[SIZE_CHAR_ARRAY];
		char *p;
		wchar_t **CyclePointer = &c;

		for (int i = 0; *(CyclePointer + i)[0] != NULL_STR; i++){
			wcstombs_s(0, temp, *(CyclePointer + i), sizeof(temp));
			p = temp;
			*(this->getStream()) << p << ' ';
		};

		*(this->getStream()) << std::endl;
	};

	void Log::writeLog(){
		time_t rawtime = time(NULL);
		struct tm timeinfo;
		char buf[SIZE_CHAR_ARRAY];
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		strftime(buf, sizeof(buf), "%d.%m.%Y  %H:%M:%S", &timeinfo);
		*(this->getStream()) << "--- Протокол ---" << std::endl << "Дата:  " << buf << std::endl;
	};

	void Log::writeCp(CP::Compiler* cp){
		*(this->getStream()) << std::endl << "---Параметры---" << std::endl << "-log: ";
		writeLine(cp->getLogName());
		*(this->getStream()) << "-out: ";
		writeLine(cp->getOutName());
		*(this->getStream()) << "-in: ";
		writeLine(cp->getInName());
		*(this->getStream()) << std::endl;
	};

	void Log::writeIn(IN::In* in){
		*(this->getStream()) << "---Исходные данные---" << std::endl
			<< "Количество символов: "	<< in->getSizeCounter()	 << std::endl
			<< "Проигнорировано: "		<< in->getIgnorCounter() << std::endl 
			<< "Количество строк: "		<< in->getLinesCounter() << std::endl << std::endl;
	};

	void Log::writeError(ERROR::Error* error){
		*(this->getStream()) << std::endl << "Ошибка " << error->getId() << ": " 
			<< error->getMessage();

		if (error->getPosition()->getLine() != -1)
			*(this->getStream()) << " строка " << error->getPosition()->getLine();

		if (error->getPosition()->getLinePosition() != -1)
			*(this->getStream()) << ", позиция "
				<< error->getPosition()->getLinePosition();

		*(this->getStream()) << std::endl << std::endl;
	};

	void Log::writeLt(LA::LexAnalyser* la){
		int currentLineNumber = 0;

		*(this->getStream()) << std::endl << "---Lexeme table start---"
			<< std::endl << "Size: " << la->getLtSize() << std::endl << std::endl
			<< "NUMBER\t\t" << "LEXEME\t\t" << "LINE\t\t" << "AUX_INDEX\t\t"
			<< "PARAMETER_COUNTER"<< std::endl;

		for (int i = 0; i < la->getLtSize(); i++){
			*(this->getStream()) << i << "\t\t";
			
			if (la->getElemLt(i)->getLex() != LT_NULL_LEX)
				*(this->getStream()) << la->getElemLt(i)->getLex() << "\t\t";
			else
				*(this->getStream()) << "null\t\t";

			if (la->getElemLt(i)->getLine() >= NULL)
				*(this->getStream()) << la->getElemLt(i)->getLine() << "\t\t";
			else
				*(this->getStream()) << "null\t\t";

			if (la->getElemLt(i)->getIndex() >= NULL)
				*(this->getStream()) << la->getElemLt(i)->getIndex() << "\t\t\t";
			else
				*(this->getStream()) << "null\t\t\t";

			if (la->getElemLt(i)->getParmCount() > NULL)
				*(this->getStream()) << la->getElemLt(i)->getParmCount() << std::endl;
			else
				*(this->getStream()) << "null" << std::endl;
		};

		*(this->getStream()) << "---Lexeme table end---" << std::endl;
	};

	void Log::writeAt(LA::LexAnalyser* la){
		char lexeme;

		*(this->getStream()) << std::endl << std::endl << "---Auxiliary table start---"
			<< std::endl << "Size: "
			<< la->getAtSize() << std::endl << std::endl << "NUMBER\t"
			<< std::setw(AT_NAME_MAXSIZE) << std::left << "NAME"
			<< std::setw(AT_NAME_MAXSIZE) << std::left << "\tFUNC_NAME"
			<< "\tDATA_TYPE\t" << "TYPE\t\t"	<< "LT_INDEX\t" << "VALUE" << std::endl;

		for (int i = 0; i < la->getAtSize(); i++){
			lexeme = la->getElemLt(la->getElemAt(i)->getIndex())->getLex();

			*(this->getStream()) << i << '\t'
				<< std::setw(AT_NAME_MAXSIZE) << std::left << la->getElemAt(i)->getName()
				<< '\t' << std::setw(AT_NAME_MAXSIZE) << std::left
				<< la->getElemAt(i)->getFuncName() << '\t'
				<< la->getPrimTypeName(la->getElemAt(i)->getDataType()) << "\t\t"
				<< la->getElemTypeName(la->getElemAt(i)->getType());

			*(this->getStream()) << '\t' << la->getElemAt(i)->getIndex() << "\t\t";

			if (lexeme == LEX_LITERAL){		//TODO: фция
				if (la->getElemAt(i)->getDataType() == AT::DATATYPE::NUM)
					*(this->getStream()) << la->getElemAt(i)->getNumVal();
 
				else if (la->getElemAt(i)->getDataType() == AT::DATATYPE::LINE)
					*(this->getStream()) << la->getElemAt(i)->getLineVal();
 			}
			else if (lexeme == LEX_OPERATION || lexeme == LEX_COMPARE){
				*(this->getStream()) << la->getElemAt(i)->getOtherVal();
			}
 			else *(this->getStream()) << "none";

			*(this->getStream()) << std::endl;
		};

		*(this->getStream()) << "---Auxiliary table end---";
	};

	void Log::close(){
		this->getStream()->close();
	};

	Log::~Log(){
		this->close();
	}
};