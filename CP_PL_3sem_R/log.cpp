#include "stdafx.h"
#include "log.h"

namespace LOG
{
	Log::Log(wchar_t logFile[]){
		this->setStream(new std::ofstream(logFile));

		if ((this->stream_)->good())
			this->setLogFile(logFile);

		else
			throw ERROR_THROW(104);

		this->writeLog();
	}

	wchar_t* Log::getLogFile(){
		return this->logFile_;
	};

	void Log::setLogFile(wchar_t* logFile){
		wcscpy_s(this->logFile_, logFile);
	};

	void Log::setStream(std::ofstream* stream){
		this->stream_ = stream;
	}; 
	
	void Log::line(char* line){
		*(this->stream_) << line;
	};

	void Log::line(wchar_t* line){
		*(this->stream_) << line << ' ';
	};

	void Log::newLine(){
		*(this->stream_) << std::endl;
	};

	void Log::writeLine(char* c, ...){
		char **p = &c;

		for (int i = 0; *(p + i)[0] != NULL_STR; i++)
			*(this->stream_) << *(p + i);
	};

	void Log::writeLine(wchar_t* c, ...){
		char temp[SIZE_CHAR_ARRAY];
		char *p;
		wchar_t **CyclePointer = &c;

		for (int i = 0; *(CyclePointer + i)[0] != NULL_STR; i++){
			wcstombs_s(0, temp, *(CyclePointer + i), sizeof(temp));
			p = temp;
			*(this->stream_) << p << ' ';
		};

		*(this->stream_) << std::endl;
	};

	void Log::writeLog(){
		time_t rawtime = time(NULL);
		struct tm timeinfo;
		char buf[SIZE_CHAR_ARRAY];
		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);
		strftime(buf, sizeof(buf), "%d.%m.%Y  %H:%M:%S", &timeinfo);
		*(this->stream_) << "--- Протокол ---" << std::endl << "Дата:  " << buf << std::endl;
	};

	void Log::writeCp(CP::Compiler* cp){
		*(this->stream_) << std::endl << "---Параметры---" << std::endl << "-log: ";
		writeLine(cp->getLogName());
		*(this->stream_) << "-out: ";
		writeLine(cp->getOutName());
		*(this->stream_) << "-in: ";
		writeLine(cp->getInName());
		*(this->stream_) << std::endl;
	};

	void Log::writeIn(IN::In* in){
		*(this->stream_) << "---Исходные данные---" << std::endl
			<< "Количество символов: "	<< in->getSizeCounter()	 << std::endl
			<< "Проигнорировано: "		<< in->getIgnorCounter() << std::endl 
			<< "Количество строк: "		<< in->getLinesCounter() << std::endl << std::endl;
	};

	void Log::writeError(ERROR::Error* error){
		*(this->stream_) << std::endl << "Ошибка " << error->getId() << ": " 
			<< error->getMessage();

		if (error->getPosition()->getLine() != -1)
			*(this->stream_) << " строка " << error->getPosition()->getLine();

		if (error->getPosition()->getLinePosition() != -1)
			*(this->stream_) << ", позиция "
				<< error->getPosition()->getLinePosition();

		*(this->stream_) << std::endl << std::endl;
	};

	void Log::writeLt(LA::LexAnalyzer* la){
		int currentLineNumber = 0;

		*(this->stream_) << std::endl << "---Lexeme table start---"
			<< std::endl << "Size: " << la->getLtSize() << std::endl << std::endl
			<< "NUMBER\t\t" << "LEXEME\t\t" << "LINE\t\t" << "AUX_INDEX\t\t"
			<< "PARAMETER_COUNTER"<< std::endl;

		for (int i = 0; i < la->getLtSize(); i++){
			*(this->stream_) << i << "\t\t";
			
			if (la->getElemLt(i)->getLex() != LT_NULL_LEX)
				*(this->stream_) << la->getElemLt(i)->getLex() << "\t\t";
			else
				*(this->stream_) << "null\t\t";

			if (la->getElemLt(i)->getLine() >= NULL)
				*(this->stream_) << la->getElemLt(i)->getLine() << "\t\t";
			else
				*(this->stream_) << "null\t\t";

			if (la->getElemLt(i)->getIndex() >= NULL)
				*(this->stream_) << la->getElemLt(i)->getIndex() << "\t\t\t";
			else
				*(this->stream_) << "null\t\t\t";

			if (la->getElemLt(i)->getParmCount() > NULL)
				*(this->stream_) << la->getElemLt(i)->getParmCount() << std::endl;
			else
				*(this->stream_) << "null" << std::endl;
		};

		*(this->stream_) << "---Lexeme table end---" << std::endl;
	};

	void Log::writeAt(LA::LexAnalyzer* la){
		char lexeme;

		*(this->stream_) << std::endl << std::endl << "---Auxiliary table start---"
			<< std::endl << "Size: "
			<< la->getAtSize() << std::endl << std::endl << "NUMBER\t"
			<< std::setw(AT_NAME_MAXSIZE) << std::left << "NAME"
			<< std::setw(AT_NAME_MAXSIZE) << std::left << "\tFUNC_NAME"
			<< "\tDATA_TYPE\t" << "TYPE\t\t"	<< "LT_INDEX\t" << "VALUE" << std::endl;

		for (int i = 0; i < la->getAtSize(); i++){
			lexeme = la->getElemLt(la->getElemAt(i)->getIndex())->getLex();

			*(this->stream_) << i << '\t'
				<< std::setw(AT_NAME_MAXSIZE) << std::left << la->getElemAt(i)->getName()
				<< '\t' << std::setw(AT_NAME_MAXSIZE) << std::left
				<< la->getElemAt(i)->getFunctionName() << '\t'
				<< la->getPrimTypeName(la->getElemAt(i)->getDataType()) << "\t\t"
				<< la->getElemTypeName(la->getElemAt(i)->getType());

			*(this->stream_) << '\t' << la->getElemAt(i)->getIndex() << "\t\t";

			if (lexeme == LEX_LITERAL){		//TODO: фция
				if (la->getElemAt(i)->getDataType() == AT::DATATYPE::NUM)
					*(this->stream_) << la->getElemAt(i)->getNumVal();
 
				else if (la->getElemAt(i)->getDataType() == AT::DATATYPE::LINE)
					*(this->stream_) << la->getElemAt(i)->getLineVal();
 			}
			else if (lexeme == LEX_OPERATION){
				*(this->stream_) << la->getOperationName(la->getElemAt(i)->getNumVal());
			}
			else if (lexeme == LEX_COMPARE){
				*(this->stream_) << la->getCompareName(la->getElemAt(i)->getNumVal());
			}
 			else *(this->stream_) << "none";

			*(this->stream_) << std::endl;
		};

		*(this->stream_) << "---Auxiliary table end---";
	};

	void Log::close(){
		if (this != nullptr){
			this->stream_->close();
		};
	};

	Log::~Log(){
		this->close();
	}
};