#include "stdafx.h"
#include "path.h"

namespace PATH{
	Path::Path(){}

	Path::Path(Path* filesPath){
		wcscpy_s(this->in_, filesPath->in_);
		wcscpy_s(this->out_, filesPath->out_);
		wcscpy_s(this->log_, filesPath->log_);
	}

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

	void Path::initialExecute(int argc, _TCHAR* argv[]){
		if (argc < 2)
			throw ERROR_THROW(100);

		if (argc > 1){
			for (int i = 1; i < argc; i++){
				if (wcsstr(argv[i], PATH_IN)){
					pushPath(this->in_, argv[i]);
					if (!wcsstr(this->out_, PATH_OUT)){
						wcscpy(this->out_, this->in_);
						wcscat(this->out_, PATH_OUT_POSTFIX);
					}
					if (!wcsstr(this->log_, PATH_LOG)){
						wcscpy(this->log_, this->in_);
						wcscat(this->log_, PATH_LOG_POSTFIX);
					}
				}
				else if (wcsstr(argv[i], PATH_OUT)){
					pushPath(this->out_, argv[i]);
				}
				else if (wcsstr(argv[i], PATH_LOG)){
					pushPath(this->log_, argv[i]);
				};
			};
		};
	};
};