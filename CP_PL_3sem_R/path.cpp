#include "stdafx.h"
#include "path.h"

namespace PATH{
	Path::Path(wchar_t* in, wchar_t* out, wchar_t* log){
		wcscpy_s(this->in_, in);
		wcscpy_s(this->out_, out);
		wcscpy_s(this->log_, log);
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

};