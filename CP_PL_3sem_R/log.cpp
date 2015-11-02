#include "stdafx.h"
#include "log.h"

namespace LOG
{
	Log::Log() { }

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

	void Log::getlog(wchar_t logFile[]){
		this->setStream(new std::ofstream(logFile));

		if ((this->getStream())->good())
			this->setLogFile(logFile);

		else
			throw ERROR_THROW(104);
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
		*(this->getStream()) << "--- �������� ---" << std::endl << "����:  " << buf << std::endl;
	};

	void Log::writeParm(Parm::PARM& parm){
		*(this->getStream()) << std::endl << "---���������---" << std::endl << "-log: ";
		writeLine(parm.log);
		*(this->getStream()) << "-out: ";
		writeLine(parm.out);
		*(this->getStream()) << "-in: ";
		writeLine(parm.in);
		*(this->getStream()) << std::endl;
	};

	void Log::writeIn(In::IN in){
		*(this->getStream()) << std::endl << std::endl << "---�������� ������---" << std::endl
			<< "���������� ��������: "	<< in.getSizeCounter()	<< std::endl
			<< "���������������: "		<< in.getIgnorCounter()	<< std::endl 
			<< "���������� �����: "		<< in.getLinesCounter()	<< std::endl;
	};

	void Log::writeError(Error::ERROR& error){
		*(this->getStream()) << std::endl << "������ " << error.id << ": " << error.message;

		if (error.inext.line != -1)
			*(this->getStream()) << " ������ " << error.inext.line;

		if (error.inext.col != -1)
			*(this->getStream()) << ", ������� " << error.inext.col;

		*(this->getStream()) << std::endl << std::endl;
	};

	void Log::writeLt(LA::LexAnalyser* la){
		int currentLineNumber = 0;

		*(this->getStream()) << std::endl << std::endl << "---Lex table start---" << std::endl
			<< "Size: " << la->getLT()->getSize() << std::endl << std::endl
			<< "NUMBER\t\t" << "LEXEMA\t\t" << "LINE\t\t" << "TI INDEX" << std::endl;

		for (int i = 0; i < la->getLT()->getSize(); i++){
			*(this->getStream()) << i << "\t\t" << la->getLT()->getElem(i)->getLex()
				<< "\t\t" << la->getLT()->getElem(i)->getLineNumber() << "\t\t";

			if (la->getLT()->getElem(i)->getIdx() >= NULL)
				*(this->getStream()) << la->getLT()->getElem(i)->getIdx() << std::endl;

			else 
				*(this->getStream()) << "no_matches" << std::endl;
		};

		*(this->getStream()) << "---Lex table end---" << std::endl;
	};

	void Log::writeAt(LA::LexAnalyser* la){
		char lexeme;

		*(this->getStream()) << std::endl << std::endl << "---Identificator table start---"
			<< std::endl << "Help: NaL - Not a Literal\t" << std::endl << "Size: "
			<< la->getAT()->getSize() << std::endl << std::endl << "NUMBER\t"
			<< std::setw(AT_NAME_MAXSIZE) << "NAME\t\t" << "FUNC NAME\t\t" << "DATA TYPE\t" << "TYPE\t"
			<< "LT INDEX\t" << "VALUE" << std::endl;

		for (int i = 0; i < la->getAT()->getSize(); i++){
			lexeme = la->getLT()->getElem(la->getAT()->getElem(i)->getIdx() - 1)->getLex();

			*(this->getStream()) << i << '\t'
				<< std::setw(AT_NAME_MAXSIZE) << la->getAT()->getElem(i)->getName()
				<< "\t\t" << std::setw(AT_NAME_MAXSIZE) << la->getAT()->getElem(i)->getFuncName()
				<< "\t\t" << la->getDataName(la->getAT()->getElem(i)->getDataType())
				<< "\t\t" << la->getAT()->getElem(i)->getType()
				<< '\t' << la->getAT()->getElem(i)->getIdx() << "\t\t";

			if (lexeme == LEX_LITERAL){		//TODO: ���� � ������ 
				if (la->getAT()->getElem(i)->getDataType() == AT::DATATYPE::NUM)
					*(this->getStream()) << la->getAT()->getElem(i)->getIntVal();
 
				else if (la->getAT()->getElem(i)->getDataType() == AT::DATATYPE::LINE)
					*(this->getStream()) << la->getAT()->getElem(i)->getStrVal();
 			}
 			else 
				*(this->getStream()) << "-NaL-";

			*(this->getStream()) << std::endl;
		};

		*(this->getStream()) << "---Identificator table end---";
	};

	void Log::close(){
		this->getStream()->close();
	};
};