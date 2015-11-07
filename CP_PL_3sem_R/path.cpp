#include "stdafx.h"
#include "path.h"

namespace PATH{
	Path::Path(){}

	wchar_t* Path::getIn(){
		return this->in_;
	};

	wchar_t* Path::getOut(){
		return this->out_;
	};

	wchar_t* Path::getLog(){
		return this->log_;
	};

	void Path::pushPath(wchar_t* dest, wchar_t* source){
		wchar_t* temp = new wchar_t;
		temp = wcsstr(source, PATH_SEPARATOR);
		temp++;
		wcscpy(dest, temp);
	};

	void Path::initialExecute(int linesNumber, char** lines){
		wchar_t tempPath[PATH_MAX_NAMESIZE];
		if (linesNumber < 1){
			throw ERROR_THROW(100);
		}
		else{
			for (int i = 0; i < linesNumber; i++){
				mbstowcs(tempPath, lines[i], static_cast<int> (strlen(lines[i])) + 1);
				if (strstr(lines[i], PATH_IN)){
					pushPath(this->in_, tempPath);
					if (!wcsstr(this->out_, PATH_OUT_WCHAR)){
						wcscpy_s(this->out_, this->in_);
						wcscat_s(this->out_, PATH_OUT_POSTFIX);
					}
					if (!wcsstr(this->log_, PATH_LOG_WCHAR)){
						wcscpy_s(this->log_, this->in_);
						wcscat_s(this->log_, PATH_LOG_POSTFIX);
					}
				}
				else if (strstr(lines[i], PATH_OUT)){
					pushPath(this->out_, tempPath);
				}
				else if (strstr(lines[i], PATH_LOG)){
					pushPath(this->log_, tempPath);
				};
			};
		};
	};

	void Path::initialExecute(int linesNumber, wchar_t* lines[]){
		if (linesNumber < 1){
			throw ERROR_THROW(100);
		}
		else{
			for (int i = 0; i < linesNumber; i++){
				if (wcsstr(lines[i], PATH_IN_WCHAR)){
					pushPath(this->in_, lines[i]);
					if (!wcsstr(this->out_, PATH_OUT_WCHAR)){
						wcscpy_s(this->out_, this->in_);
						wcscat_s(this->out_, PATH_OUT_POSTFIX);
					}
					if (!wcsstr(this->log_, PATH_LOG_WCHAR)){
						wcscpy_s(this->log_, this->in_);
						wcscat_s(this->log_, PATH_LOG_POSTFIX);
					}
				}
				else if (wcsstr(lines[i], PATH_OUT_WCHAR)){
					pushPath(this->out_, lines[i]);
				}
				else if (wcsstr(lines[i], PATH_LOG_WCHAR)){
					pushPath(this->log_, lines[i]);
				};
			};
		};
	};

	void Path::initialExecute(char* inFile, char* outFile, char* logFile){
		wchar_t tempPath[PATH_MAX_NAMESIZE];
		mbstowcs(tempPath, inFile, static_cast<int> (strlen(inFile)) + 1);
		pushPath(this->in_, tempPath);
		wcscpy_s(this->out_, this->in_);
		wcscat_s(this->out_, PATH_OUT_POSTFIX);
		wcscpy_s(this->log_, this->in_);
		wcscat_s(this->log_, PATH_LOG_POSTFIX);
		if (outFile != NULL_STR){
			mbstowcs(tempPath, outFile, static_cast<int> (strlen(outFile)) + 1);
			pushPath(this->in_, tempPath);
		};
		if (logFile != NULL_STR){
			mbstowcs(tempPath, logFile, static_cast<int> (strlen(logFile)) + 1);
			pushPath(this->in_, tempPath);
		};
	};

	void Path::initialExecute(wchar_t* inFile, wchar_t* outFile, wchar_t* logFile){
		pushPath(this->in_, inFile);
		wcscpy_s(this->out_, this->in_);
		wcscat_s(this->out_, PATH_OUT_POSTFIX);
		wcscpy_s(this->log_, this->in_);
		wcscat_s(this->log_, PATH_LOG_POSTFIX);
		if (outFile != NULL_STR){
			pushPath(this->in_, outFile);
		};
		if (logFile != NULL_STR){
			pushPath(this->in_, logFile);
		};
	};
};