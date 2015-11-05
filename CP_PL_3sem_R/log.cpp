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

	void Log::writeLine(char* c, ...){
		char **p = &c;

		for (int i = 0; *(p + i)[0] != NULL_STR; i++)
			*(this->getStream()) << *(p + i);

		*(this->getStream()) << std::endl;
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

	void Log::writeCP(CP::Compiler* cp){
		*(this->getStream()) << std::endl << "---Параметры---" << std::endl << "-log: ";
		writeLine(cp->getLogName());
		*(this->getStream()) << "-out: ";
		writeLine(cp->getOutName());
		*(this->getStream()) << "-in: ";
		writeLine(cp->getInName());
		*(this->getStream()) << std::endl;
	};

	void Log::writeIn(IN::In* in){
		*(this->getStream()) << std::endl << std::endl << "---Исходные данные---" << std::endl
			<< "Количество символов: "	<< in->getSizeCounter()	 << std::endl
			<< "Проигнорировано: "		<< in->getIgnorCounter() << std::endl 
			<< "Количество строк: "		<< in->getLinesCounter() << std::endl;
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

		*(this->getStream()) << std::endl << std::endl << "---Lex table start---"
			<< std::endl << "Size: " << la->getLT()->getSize() << std::endl << std::endl
			<< "NUMBER\t\t" << "LEXEMA\t\t" << "LINE\t\t" << "AUX_INDEX\t\t"
			<< "PARAMERER_COUNTER"<< std::endl;

		for (int i = 0; i < la->getLT()->getSize(); i++){
			*(this->getStream()) << i << "\t\t";
			
			if (la->getLT()->getElem(i)->getLex() != LT_NULL_LEX)
				*(this->getStream()) << la->getLT()->getElem(i)->getLex() << "\t\t";
			else
				*(this->getStream()) << "null\t\t";

			if (la->getLT()->getElem(i)->getLineNumber() >= NULL)
				*(this->getStream()) << la->getLT()->getElem(i)->getLineNumber() << "\t\t";
			else
				*(this->getStream()) << "null\t\t";

			if (la->getLT()->getElem(i)->getIdx() >= NULL)
				*(this->getStream()) << la->getLT()->getElem(i)->getIdx() << "\t\t\t";
			else
				*(this->getStream()) << "null\t\t\t";

			if (la->getLT()->getElem(i)->getParmCount() > NULL)
				*(this->getStream()) << la->getLT()->getElem(i)->getParmCount() << std::endl;
			else
				*(this->getStream()) << "null" << std::endl;
		};

		*(this->getStream()) << "---Lex table end---" << std::endl;
	};

	void Log::writeAt(LA::LexAnalyser* la){
		char lexeme;

		*(this->getStream()) << std::endl << std::endl << "---Auxiliary table start---"
			<< std::endl << "Size: "
			<< la->getAT()->getSize() << std::endl << std::endl << "NUMBER\t"
			<< std::setw(AT_NAME_MAXSIZE) << "NAME\t\t" << "FUNC_NAME\t\t" << "DATA_TYPE\t" 
			<< "TYPE\t"	<< "LT_INDEX\t" << "VALUE" << std::endl;

		for (int i = 0; i < la->getAT()->getSize(); i++){
			lexeme = la->getLT()->getElem(la->getAT()->getElem(i)->getIdx())->getLex();

			*(this->getStream()) << i << '\t'
				<< std::setw(AT_NAME_MAXSIZE) << la->getAT()->getElem(i)->getName()
				<< "\t\t" << std::setw(AT_NAME_MAXSIZE) << la->getAT()->getElem(i)->getFuncName()
				<< "\t\t" << la->getDataName(la->getAT()->getElem(i)->getDataType())
				<< "\t\t" << la->getAT()->getElem(i)->getType()
				<< '\t' << la->getAT()->getElem(i)->getIdx() << "\t\t";

			if (lexeme == LEX_LITERAL){		//TODO: фция и массив 
				if (la->getAT()->getElem(i)->getDataType() == AT::DATATYPE::NUM)
					*(this->getStream()) << la->getAT()->getElem(i)->getIntVal();
 
				else if (la->getAT()->getElem(i)->getDataType() == AT::DATATYPE::LINE)
					*(this->getStream()) << la->getAT()->getElem(i)->getStrVal();
 			}
			else if (lexeme == LEX_OPERATION){
				*(this->getStream()) << la->getAT()->getElem(i)->getOperation();
			}
 			else
				*(this->getStream()) << "none";

			*(this->getStream()) << std::endl;
		};

		*(this->getStream()) << "---Auxiliary table end---";
	};

	void Log::close(){
		this->getStream()->close();
	};
};